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
#ifndef EMU_FILE_IOSTREAM_H_
#define EMU_FILE_IOSTREAM_H_

#include <stdio.h>

#include "emu_iostream.h"

class TFileInputStream : public TInputStream
{
public:
    /** Destructor. */
    virtual ~TFileInputStream();

    virtual unsigned read( void * buf, unsigned len );

    virtual unsigned skip( unsigned n );

    static TFileInputStream * open( const char * name ) {
        FILE * f = fopen( name, "rb" );

        return (f != 0) ? new TFileInputStream(f) : 0;
    }

private:
    TFileInputStream( FILE * f ) : f_(f) {
    }

    TFileInputStream();
    TFileInputStream( const TFileInputStream & );
    TFileInputStream & operator = ( const TFileInputStream & );

    FILE * f_;
};

class TFileOutputStream : public TOutputStream
{
public:
    /** Destructor. */
    virtual ~TFileOutputStream();

    virtual unsigned write( const char * buf, unsigned len );
    
    virtual void flush();

    static TFileOutputStream * open( const char * name, bool append ) {
        FILE * f = fopen( name, append ? "ab" : "wb" );

        return (f != 0) ? new TFileOutputStream(f) : 0;
    }

private:
    TFileOutputStream( FILE * f ) : f_(f) {
    }
    
    TFileOutputStream();
    TFileOutputStream( const TFileInputStream & );
    TFileOutputStream & operator = ( const TFileInputStream & );

    FILE * f_;
};

#endif // EMU_FILE_IOSTREAM_H_
