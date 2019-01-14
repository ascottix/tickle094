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
#ifndef EMU_CHAR_DECODER_H_
#define EMU_CHAR_DECODER_H_

// Full info (up to 16x16 and 4 bit planes)
struct TDecodeCharInfo
{
    int width;          // Width in pixels
    int height;         // Height in pixels
    int nplanes;        // Number of bit planes
    int planeofs[4];    // Offset of bit plane (in bits)
    int xofs[16];       // Offset of pixel x coordinate in bits relative to one bit plane (in bits)
    int yofs[16];       // Offset of pixel y coordinate in bits relative to one bit plane (in bits)
};

// Slightly simpler and smaller version for 8x8 characters and tiles
struct TDecodeCharInfo8x8
{
    int nplanes;        // Number of bit planes
    int planeofs[4];    // Offset of bit plane (in bits)
    int xofs[8];        // Offset of pixel x coordinate in bits relative to one bit plane (in bits)
    int yofs[8];        // Offset of pixel y coordinate in bits relative to one bit plane (in bits)
};

void decodeChar( unsigned char * buf, const TDecodeCharInfo & info, const unsigned char * source, int offset_bits );

void decodeCharSet( unsigned char * buf, const TDecodeCharInfo & info, const unsigned char * source, int total, int delta_bits );

void decodeChar8x8( unsigned char * buf, const TDecodeCharInfo8x8 & info, const unsigned char * source );

void decodeCharSet8x8( unsigned char * buf, const TDecodeCharInfo8x8 & info, const unsigned char * source, int total, int delta );

#endif // EMU_CHAR_DECODER_H_
