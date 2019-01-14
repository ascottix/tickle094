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
#ifndef EMU_IOSTREAM_H_
#define EMU_IOSTREAM_H_

class TInputStream
{
public:
    TInputStream() {
    }

    /** Destructor. */
    virtual ~TInputStream() {
    }

    virtual int read();

    virtual unsigned read( void * buf, unsigned len ) = 0;

    virtual unsigned skip( unsigned n );
};

class TOutputStream
{
public:
    TOutputStream() {
    }
    
    /** Destructor. */
    virtual ~TOutputStream() {
    }

    virtual int write( int c );
    
    virtual unsigned write( const char * buf, unsigned len ) = 0;
    
    virtual void flush() {
    }
};

#endif // EMU_IOSTREAM_H_
