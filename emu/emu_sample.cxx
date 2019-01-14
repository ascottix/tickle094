/*
    Tickle class library

    Copyright (c) 2003,2004 Alessandro Scotti
    http://www.ascotti.org/

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "emu_sample.h"

TSample::TSample( int * data, unsigned size, unsigned sampling_rate ) : size_(size), sampling_rate_(sampling_rate), data_(data)
{
}

TSample::~TSample()
{
    delete data_;
}

static bool isFourCC( const unsigned char * s, const char * cc )
{
    return ((char)s[0] == cc[0]) && ((char)s[1] == cc[1]) && ((char)s[2] == cc[2]) && ((char)s[3] == cc[3]);
}

static unsigned getUint( const unsigned char * s, int size )
{
    unsigned result = 0;
    
    for( int i=size-1; i>=0; i-- ) {
        result = (result << 8) | ((unsigned)s[i]);
    }

    return result;
}

TSample * TSample::createFromWave( TInputStream * is )
{
    TSample * result = 0;

    unsigned char header[12] = { 0 };

    if( is != 0 ) {
        is->read( header, sizeof(header) );
    }
    // ...else we exit because the next check fails

    if( isFourCC( header+0, "RIFF" ) && isFourCC( header+8, "WAVE" ) ) {
        unsigned sampling_rate = 0;
        unsigned block_align;

        while( 1 ) {
            // Read the chunk header
            if( is->read( header, 8 ) != 8 )
                break;

            unsigned chunk_length = getUint( header+4, 4 );

            if( isFourCC( header, "fmt " ) ) {
                // Format
                unsigned char format[14];

                // Read the most basic information and skip over the rest
                if( chunk_length < sizeof(format) )
                    break;
               
                if( is->read( format, sizeof(format) ) != sizeof(format) )
                    break;

                is->skip( chunk_length - sizeof(format) );

                // Parse the format data
                unsigned type = getUint( format+0, 2 );
                unsigned channels = getUint( format+2, 2 );
                sampling_rate = getUint( format+4, 4 );
                block_align = (int) getUint( format+12, 2 );

                // Exit if not WAVE_FORMAT_PCM with only one channel
                if( (type != 1) || (channels != 1) )
                    break;

            }
            else if( isFourCC( header, "data" ) ) {
                // Data (must come after the format tag)
                if( sampling_rate == 0 )
                    break;
            
                unsigned char * samples = new unsigned char [ chunk_length ];

                if( is->read( samples, chunk_length ) == chunk_length ) {
                    unsigned length;
                    int * data = 0;

                    // Convert the sample data into our format
                    if( block_align == 1 ) {
                        // 8-bit unsigned
                        length = chunk_length;
                        data = new int [length];

                        for( unsigned i=0; i<length; i++ ) {
                            data[i] = ((int) samples[i]) - 0x80;
                        }
                    }
                    else if( block_align == 2 ) {
                        // 16-bit signed (yep, 8 and 16 bit formats are different...)
                        length = chunk_length / 2;
                        data = new int [length];

                        for( unsigned i=0; i<length; i++ ) {
                            unsigned d = getUint( samples+i*2, 2 );

                            data[i] = (d >= 0x8000) ? (int) d - 0x10000 : (int) d;

                            // For now, work with 8 bits only...
                            data[i] /= 256;
                        }
                    }

                    // Create the sample if the conversion was successful
                    if( data != 0 ) {
                        result = new TSample( data, length, sampling_rate );
                    }
                }

                delete samples;

                break;
            }
            else {
                // Unknown type: skip it
                is->skip( chunk_length );
            }
        }
    }

    return result;
}
