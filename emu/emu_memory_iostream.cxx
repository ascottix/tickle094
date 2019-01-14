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
#include "emu_memory_iostream.h"

TMemoryInputStream::TMemoryInputStream( const unsigned char * data, unsigned size ) 
    : data_(data), size_(size) 
{
    offset_ = 0;
}

unsigned TMemoryInputStream::read( void * buf, unsigned len ) {
    unsigned result = 0;

    if( offset_ < size_ ) {
        unsigned max_len = size_ - offset_;

        if( len > max_len ) {
            len = max_len;
        }

        memcpy( buf, data_+offset_, len );
        offset_ += len;

        result = len;
    }

    return result;
}
