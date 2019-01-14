/*
    Namco 05xx (starfield generator) custom chip emulator

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
#ifndef NAMCO_05XX_
#define NAMCO_05XX_

#include <emu/emu_bitmap.h>

struct Namco05xx {
    unsigned char state_;
    int scroll_x_;
    int scroll_y_;
    
    Namco05xx();
    
    void reset();
    
    void writeRegister( unsigned index, unsigned char value );
    
    void update();
    
    void render( TBitmapIndexed * screen );
};

#endif // NAMCO_05XX_
