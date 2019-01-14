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
#include "emu_iostream.h"

int TInputStream::read()
{
    char b;

    return read( &b, 1 ) == 1 ? (int)b : -1;
}

unsigned TInputStream::skip( unsigned n )
{
    unsigned t = n;

    while( n > 0 ) {
        if( read() < 0 ) {
            break;
        }
        n--;
    }

    return t - n;
}

int TOutputStream::write( int c )
{
    char b = (char) c;

    return write( &b, 1 ) == 1 ? c : -1;
}
