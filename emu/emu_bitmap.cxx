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
#include "emu_bitmap.h"

TBitmap::TBitmap( int width, int height, TBitmapFormat format ) : 
    width_(width), height_(height), format_(format)
{
}

TBitmapIndexed::TBitmapIndexed( int width, int height, TPalette * palette ) : 
    TBitmap( width, height, bfIndexed ), palette_(palette) 
{
    bits_ = new TBitBlock( width, height );
    clear();
}

void TBitmapIndexed::clear()
{
    bits_->clear();
}

bool TBitmapIndexed::draw( int x, int y, TBitmap & bitmap )
{
    bool result = false;

    // Not yet implemented

    return result;
}

TBitmapRGB::TBitmapRGB( int width, int height ) :
    TBitmap( width, height, bfRGB )
{
    bits_ = new unsigned [width * height];
}
 
unsigned TBitmapRGB::pixel( int x, int y )
{
    unsigned result = 0;

    if( (x >= 0) && (x < width()) && (y >= 0) && (y < height()) ) {
        result = bits_[ y*width() + x ];
    }

    return result;
}

void TBitmapRGB::setPixel( int x, int y, unsigned color )
{
    if( (x >= 0) && (x < width()) && (y >= 0) && (y < height()) ) {
        bits_[ y*width() + x ] = color;
    }
}

void TBitmapRGB::clear()
{
    memset( bits_, 0, sizeof(unsigned)*width()*height() );
}

bool TBitmapRGB::draw( int x, int y, TBitmap & bitmap )
{
    bool result = false;

    // Not yet implemented

    return result;
}
