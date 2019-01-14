/*
    Tickle front-end
    User interface for Mac OS X

    Copyright (c) 2006 Alessandro Scotti
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
#include "mac_frame.h"

MacFrame::MacFrame( int width, int height )
{
    width_ = width;
    height_ = height;
    data_offset_ = 0;
    data_count_ = (int) width*height*sizeof(unsigned);
    data_ = new unsigned char [ data_count_ ];
    screen_ = data_;
    max_voices_ = 0;

    memset( data_, 0, data_count_ );
}

MacFrame::~MacFrame()
{
    delete [] data_;
}

void MacFrame::setVideo( TBitmap * bitmap, bool flipped )
{
    if( bitmap != 0 ) {
        int width = TMath::min( bitmap->width(), width_ );
        int height = TMath::min( bitmap->height(), height_ );
        
        if( bitmap->format() == bfIndexed ) {
            TBitmapIndexed * bm = reinterpret_cast<TBitmapIndexed *>( bitmap );
            
            const unsigned * palette = bm->palette()->data();
            const unsigned char * bits = bm->bits()->data();
            
            // Fix byte order in palette
            unsigned x_palette[256];

            for( int i=0; i<bm->palette()->colors(); i++ ) {
                unsigned c = palette[i];
                
#ifdef __ppc__
                x_palette[i] = (c << 8) | (c >> 24);
#else
                x_palette[i] = ((c >> 16) & 0x0000FF) | (c & 0x00FF00) | ((c << 16) & 0xFF0000);
#endif
            }
            
            // Copy data
            if( flipped ) {
                bits += width*height;
                
                for( int y=0; y<height; y++ ) {
                    unsigned * pd = (unsigned *) (data_ + (height_-1-y) * width_ * sizeof(unsigned));
                    for( int x=width; x>0; x-- ) {
                        *pd++ = x_palette[ *--bits ];
                    }
                }
            }
            else {
                for( int y=0; y<height; y++ ) {
                    unsigned * pd = (unsigned *) (data_ + (height_-1-y) * width_ * sizeof(unsigned));
                    for( int x=width; x>0; x-- ) {
                        *pd++ = x_palette[ *bits++ ];
                    }
                }
            }
        }
        else if( bitmap->format() == bfRGB ) {
            TBitmapRGB * bm = reinterpret_cast<TBitmapRGB *>( bitmap );
            
            const unsigned * bits = bm->data();
            
            for( int y=0; y<height; y++ ) {
                unsigned char * dst = data_ + (height_-1-y) * width_ * sizeof(unsigned);
                
                const unsigned * src = bits;
                
                for( int x=width; x>0; x-- ) {
                    unsigned c = *src++;
                    
                    *dst++ = c >> 16;
                    *dst++ = (c >> 8) & 0xFF;
                    *dst++ = c & 0xFF;
                    *dst++ = 0;
                }
                
                bits += width;
            }
        }
    }
}
