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
#ifndef EMU_DATA_IOSTREAM_H_
#define EMU_DATA_IOSTREAM_H_

#include "emu_iostream.h"

class TDataInputStream
{
public:
    TDataInputStream( TInputStream * is ) : is_(is) {
    }

    /** Destructor. */
    virtual ~TDataInputStream() {
    }

    virtual int read() {
        return is_->read();
    }

    virtual int read( char * buf, int len ) {
        return is_->read( buf, len );
    }

    virtual void readBuffer( char * buf, int len );

    virtual unsigned readWord16();

    virtual unsigned readWord32();

private:
    TDataInputStream( const TDataInputStream & );
    TDataInputStream & operator = ( const TDataInputStream & );

    TInputStream * is_;
};

class TDataOutputStream
{
public:
    TDataOutputStream( TOutputStream * os ) : os_(os) {
    }

    virtual ~TDataOutputStream() {
        os_->flush();
    }

    virtual int write( int c ) {
        return os_->write( c );
    }

    virtual int write( char * buf, int len ) {
        return os_->write( buf, len );
    }

    virtual void writeBuffer( char * buf, int len );

    virtual void writeWord16( unsigned value );

    virtual void writeWord32( unsigned value );

private:
    TDataOutputStream( const TDataOutputStream & );
    TDataOutputStream & operator = ( const TDataOutputStream & );

    TOutputStream * os_;
};

#endif // EMU_DATA_IOSTREAM_H_
