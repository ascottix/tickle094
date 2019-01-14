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
#include "emu_file_iostream.h"

TFileInputStream::~TFileInputStream() 
{
    fclose( f_ );
}

unsigned TFileInputStream::read( void * buf, unsigned len ) 
{
    return fread( buf, 1, (size_t)len, f_ );
}

unsigned TFileInputStream::skip( unsigned n )
{
    unsigned result = 0;

    long offset = (long) n;

    if( offset > 0 ) {
        long curpos = ftell( f_ );
        result = (fseek( f_, offset, SEEK_CUR ) == 0) ? n : ftell(f_)-curpos;
    }

    return result;
}

TFileOutputStream::~TFileOutputStream() 
{
    flush();
    fclose( f_ );
}

unsigned TFileOutputStream::write( const char * buf, unsigned len )
{
    return fwrite( buf, 1, (size_t)len, f_ );
}

void TFileOutputStream::flush()
{
    fflush( f_ );
}
