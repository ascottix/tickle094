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
#ifndef EMU_PALETTE_H_
#define EMU_PALETTE_H_

enum TPaletteByteEncodingSchema 
{
    esDefault,
    esAlternate,
};

class TPalette
{
public:
    TPalette( int colors ) {
        colors_ = colors;
        data_ = new unsigned [colors];
    }

    /** Destructor. */
    virtual ~TPalette() {
        delete data_;
    }

    unsigned color( int index ) const {
        return data_[index];
    }

    void setColor( int index, unsigned color ) {
        if( (index >= 0) && (index < colors_) ) {
            data_[index] = color;
        }
    }

    unsigned colors() const {
        return colors_;
    }

    unsigned * data() {
        return data_;
    }

    int getNearestColor( unsigned color );

    static unsigned encodeColor( unsigned r, unsigned g, unsigned b );

    static void decodeColor( unsigned color, unsigned char * r, unsigned char * g, unsigned char * b );

    static unsigned decodeByte( unsigned char value, int schema = esDefault );

private:
    TPalette( const TPalette & );
    const TPalette & operator = ( const TPalette & );

    int colors_;
    unsigned * data_;
};

#endif // EMU_PALETTE_H_
