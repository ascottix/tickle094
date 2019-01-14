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
#ifndef DIB32_H_
#define DIB32_H_

#include <windows.h>

class TBitmap;

/**
    Device Independent Bitmap in RGB format (32 bits per pixel).
*/
class Dib32
{
public:
    /** Constructor. */
    Dib32( int width, int height );
    
    /** Copy constructor. */
    Dib32( const Dib32 & );
    
    /** Destructor. */
    virtual ~Dib32();

    /** Clears the bitmap setting all pixels to black. */
    void clear();

    /** 
        Copies the bitmap into a device context.

        @param hdc handle of device context to receive bitmap copy
        @param x x destination of bitmap in device context
        @param y y destination of bitmap in device context
        @param width width of copied bitmap in device context
        @param height width of copied bitmap in device context
    */
    void stretch( HDC hdc, int x, int y, int width, int height );

    /** Retrieves a pixel at the specified coordinates, with no range checking. */
    DWORD getFastPixel( int x, int y ) {
        LPDWORD pb = (LPDWORD)(bitmapbits_ + y*scanlength_ + x*4);
        return *pb;
    }

    /** Sets a pixel at the specified coordinates, with no range checking. */
    void setFastPixel( int x, int y, DWORD value ) {
        LPDWORD pb = (LPDWORD)(bitmapbits_ + y*scanlength_ + x*4);
        *pb = value;
    }

    /** Retrieves a pixel at the specified coordinates (safe method). */
    DWORD getPixel( int x, int y );

    /** Sets a pixel at the specified coordinates (safe method). */
    void setPixel( int x, int y, DWORD value );

    /** Returns the address of the specified bitmap row. */
    unsigned char * getScanLine( int row ) {
        if( row >= 0 && row < height_ )
            return bitmapbits_ + row*scanlength_;
        return 0;
    }

    /** Returns the width of the bitmap. */
    int getWidth() const {
        return width_;
    }

    /** Returns the height of the bitmap. */
    int getHeight() const {
        return height_;
    }

    /** Returns the bitmap device context. */
    HDC getDeviceContext() {
        return hdc_;
    }

    /** Returns the bitmap handle. */
    HANDLE getBitmapHandle() {
        return hbm_;
    }

    /** Converts the DIB into a DDB. */
    HBITMAP toBitmap() {
        return CreateDIBitmap( hdc_,
            &bitmapinfo_.bmiHeader,
            CBM_INIT,
            bitmapbits_,
            &bitmapinfo_,
            DIB_RGB_COLORS );
    }

    bool assign( TBitmap * bitmap, bool flip = false );

private:
    void createBitmap( int width, int height );

    BITMAPINFO  bitmapinfo_;
    HANDLE      hbm_;
    LPBYTE      bitmapbits_;
    int         scanlength_;
    int         width_;
    int         height_;
    HDC         hdc_;
};

#endif // DIB32_H_
