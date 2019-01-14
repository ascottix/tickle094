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
#include "emu_palette.h"

inline int abs( int n ) {
    return n >= 0 ? n : -n;
}

int TPalette::getNearestColor( unsigned color )
{
    int min_distance = 31081900;
    int min_distance_index = 0;

    unsigned char r;
    unsigned char b;
    unsigned char g;

    decodeColor( color, &r, &g, &b );

    for( int index=0; index<colors_; index++ ) {
        unsigned char r2;
        unsigned char b2;
        unsigned char g2;

        decodeColor( data_[index], &r2, &b2, &g2 );

        int distance = abs(r-r2) + abs(g-g2) + abs(b-b2);

        if( distance < min_distance ) {
            min_distance = distance;
            min_distance_index = index;
        }
    }

    return min_distance_index;
}

unsigned TPalette::encodeColor( unsigned r, unsigned g, unsigned b )
{
    return b | (g << 8) | (r << 16);
}

void TPalette::decodeColor( unsigned color, unsigned char * r, unsigned char * g, unsigned char * b )
{
    if( b != 0 ) *b = (unsigned char)(0xFF & (color));
    if( g != 0 ) *g = (unsigned char)(0xFF & (color >> 8));
    if( r != 0 ) *r = (unsigned char)(0xFF & (color >> 16));
}

unsigned TPalette::decodeByte( unsigned char value, int schema )
{
    unsigned bit[8];
    unsigned r = 0;
    unsigned g = 0;
    unsigned b = 0;

    for( int i=0; i<8; i++ ) {
        bit[i] = (value >> i) & 0x01;
    }

    if( schema == esDefault ) {
        r = 0x21*bit[0] + 0x47*bit[1] + 0x97*bit[2];
        g = 0x21*bit[3] + 0x47*bit[4] + 0x97*bit[5];
        b =               0x47*bit[6] + 0x97*bit[7];
    }
    else if( schema == esAlternate ) {
        r = 0x21*bit[0] + 0x47*bit[1] + 0x97*bit[2];
        g = 0x21*bit[3] + 0x47*bit[4] + 0x97*bit[5];
        b =               0x51*bit[6] + 0xAE*bit[7];
    }

    return encodeColor( r, g, b );
}
