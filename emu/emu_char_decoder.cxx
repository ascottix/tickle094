/*
    Tickle class library

    Copyright (c) 2011 Alessandro Scotti
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
#include "emu_char_decoder.h"

static const unsigned char BitMask[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

void decodeChar8x8( unsigned char * buf, const TDecodeCharInfo8x8 & info, const unsigned char * source )
{
    for( int y=0; y<8; y++ ) {
        for( int x=0; x<8; x++ ) {
            unsigned baseofs = info.yofs[y] + info.xofs[x];
            
            unsigned char b = 0;
            
            for( int plane=0; plane<info.nplanes; plane++ ) {
                unsigned bitofs = baseofs + info.planeofs[plane];
                
                if( source[bitofs / 8] & BitMask[bitofs % 8] ) {
                    b |= BitMask[plane];
                }
            }
            
            *buf++ = b;
        }
    }
}

void decodeCharSet8x8( unsigned char * buf, const TDecodeCharInfo8x8 & info, const unsigned char * source, int total, int delta )
{
    for( int i=0; i<total; i++ ) {
        decodeChar8x8( buf, info, source );
        buf += 8*8;
        source += delta; // Here delta is in bytes (use the "full" version if this is not the case)
    }
}

void decodeChar( unsigned char * buf, const TDecodeCharInfo & info, const unsigned char * source, int offset_bits )
{
    for( int y=info.height-1; y>=0; y-- ) {
        for( int x=info.width-1; x>=0; x-- ) {
            unsigned baseofs = info.yofs[y] + info.xofs[x] + offset_bits;
            
            unsigned char b = 0;
            
            for( int plane=0; plane<info.nplanes; plane++ ) {
                unsigned bitofs = baseofs + info.planeofs[plane];
                
                if( source[bitofs / 8] & BitMask[bitofs % 8] ) {
                    b |= BitMask[plane];
                }
            }
            
            *buf++ = b;
        }
    }
}

void decodeCharSet( unsigned char * buf, const TDecodeCharInfo & info, const unsigned char * source, int total, int delta_bits )
{
    int offset_bits = 0;
    int buf_delta = info.width * info.height;
    
    for( int i=0; i<total; i++ ) {
        decodeChar( buf, info, source, offset_bits );
        buf += buf_delta;
        offset_bits += delta_bits;
    }
}
