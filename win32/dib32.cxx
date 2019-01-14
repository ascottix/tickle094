/*
    32 bit Device Independent Bitmap class
    
    Copyright (c) 2004 Alessandro Scotti
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
#include <assert.h>

#include <emu/emu_bitmap.h>
#include <emu/emu_math.h>

#include "dib32.h"

// Undefine two macros that may have been defined by a system header
#undef min
#undef max

void Dib32::createBitmap( int width, int height )
{
    assert( width > 0 );
    assert( height > 0 );

    hdc_ = CreateCompatibleDC( 0 );
    assert( hdc_ != 0 );

    // Create DIB section to hold bitmap data
    ZeroMemory( &bitmapinfo_, sizeof(bitmapinfo_) );
    bitmapinfo_.bmiHeader.biSize = sizeof(bitmapinfo_.bmiHeader);
    bitmapinfo_.bmiHeader.biWidth = width;
    bitmapinfo_.bmiHeader.biHeight = -height; // Create top-down bitmap
    bitmapinfo_.bmiHeader.biPlanes = 1;
    bitmapinfo_.bmiHeader.biBitCount = 32;
    bitmapinfo_.bmiHeader.biCompression = BI_RGB;
    bitmapinfo_.bmiHeader.biSizeImage = 0;
    bitmapinfo_.bmiHeader.biXPelsPerMeter = 1;
    bitmapinfo_.bmiHeader.biYPelsPerMeter = 1;
    bitmapinfo_.bmiHeader.biClrUsed = 0;
    bitmapinfo_.bmiHeader.biClrImportant = 0;

    LPVOID  pb;

    hbm_ = CreateDIBSection( hdc_,
      &bitmapinfo_,
      DIB_RGB_COLORS,
      &pb,
      0,
      0 );

    assert( hbm_ != 0 );

    SelectObject( hdc_, hbm_ );

    bitmapbits_ = (LPBYTE)pb;
    scanlength_ = width*4;
    width_ = width;
    height_ = height;
}

Dib32::Dib32( int width, int height )
{
    createBitmap( width, height );
    clear();
}

Dib32::Dib32( const Dib32 & dib )
{
    createBitmap( dib.width_, dib.height_ );
    memcpy( bitmapbits_, dib.bitmapbits_, scanlength_*height_ );
}

Dib32::~Dib32()
{
    DeleteDC( hdc_ );
    DeleteObject( hbm_ );
}

void Dib32::clear()
{
    ZeroMemory( bitmapbits_, scanlength_*height_ );
}

void Dib32::stretch( HDC hdc, int x, int y, int width, int height )
{
    StretchDIBits( hdc,
        x,
        y,
        width,
        height,
        0,
        0,
        width_,
        height_,
        bitmapbits_,
        &bitmapinfo_,
        DIB_RGB_COLORS,
        SRCCOPY );
}

DWORD Dib32::getPixel( int x, int y )
{
    if( x < 0 || x >= width_ || y < 0 || y >= height_ )
        return 0;

    return getFastPixel( x, y );
}

void Dib32::setPixel( int x, int y, DWORD value )
{
    if( x < 0 || x >= width_ || y < 0 || y >= height_ )
        return;

    setFastPixel( x, y, value );
}

bool Dib32::assign( TBitmap * bitmap, bool flip )
{
    bool result = false;

    if( bitmap != 0 ) {
        int width = TMath::min( bitmap->width(), width_ );
        int height = TMath::min( bitmap->height(), height_ );

        if( bitmap->format() == bfIndexed ) {
            TBitmapIndexed * bm = reinterpret_cast<TBitmapIndexed *>( bitmap );

            const unsigned * palette = bm->palette()->data();
            const unsigned char * bits = bm->bits()->data();

            if( flip ) {
                bits += height * width;
                for( int y=0; y<height; y++ ) {
                    LPDWORD pd = (LPDWORD) getScanLine( y );
                    for( int x=width; x>0; x-- ) {
                        *pd++ = palette[ *--bits ];
                    }
                }
            }
            else {
                for( int y=0; y<height; y++ ) {
                    LPDWORD pd = (LPDWORD) getScanLine( y );
                    for( int x=width; x>0; x-- ) {
                        *pd++ = palette[ *bits++ ];
                    }
                }
            }

            result = true;
        }
        else if( bitmap->format() == bfRGB ) {
            TBitmapRGB * bm = reinterpret_cast<TBitmapRGB *>( bitmap );

            assert( sizeof(DWORD) == sizeof(unsigned) );

            const unsigned * bits = bm->data();

            assert( ! flip ); // Flip not supported for RGB bitmaps

            for( int y=0; y<height; y++ ) {
                LPDWORD pd = (LPDWORD) getScanLine( y );

                memcpy( pd, bits, sizeof(unsigned)*width );

                bits += width;
            }

            result = true;
        }
    }

    return result;
}
