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
#ifndef EMU_PNG_H_
#define EMU_PNG_H_

#include "emu_bitmap.h"
#include "emu_iostream.h"

/**
    Reads a PNG file from the specified input stream.

    @return null on error, or a TBitmap object that may be an
    instance of either TBitmapIndexed or TBitmapRGB.
*/
TBitmap * createBitmapFromPNG( TInputStream * );

#endif // EMU_PNG_H_
