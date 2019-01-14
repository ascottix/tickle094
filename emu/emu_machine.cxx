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
#include <assert.h>
#include <string.h>

#include "emu_machine.h"
#include "emu_memory_iostream.h"

TMachine::TMachine()
{
    info_ = 0;
}

TMachine::~TMachine()
{
    delete info_;
}

const char * TMachine::getResourceName( int index ) const
{
    TMachineInfo * mi = (TMachineInfo *) drivers_.item( index );

    return (mi != 0) ? mi->driver : 0;
}

void TMachine::registerDriver( const TMachineInfo & driver )
{
    drivers_.insert( 0, const_cast<TMachineInfo *>(&driver) );
}

TMachine * TMachine::createInstance( TMachineFactoryFunc factoryFunc )
{
    TMachine * result = factoryFunc();

    assert( result != 0 );

    TMachineInfo * mi = (TMachineInfo *) result->drivers_.item( 0 );

    assert( mi != 0 );

    result->info_ = new TMachineDriverInfo( mi );

    if( result->initialize( result->info_ ) ) {
        result->info_->userInterface()->notify( result );
    }

    return result;
}

bool TMachine::copyResourceFile( unsigned char * dst, unsigned dst_size, const unsigned char * src, unsigned src_size )
{
    bool result = false;

    if( (dst_size >= src_size) && (src != 0) ) {
        memcpy( dst, src, src_size );
        result = true;
    }

    return result;
}

bool TMachine::replaceMemory( unsigned char * address, const unsigned char * original, const unsigned char * replacement, unsigned len )
{
    bool result = false;

    if( memcmp( address, original, len ) == 0 ) {
        memcpy( address, replacement, len );
        result = true;
    }

    return result;
}

void TMachine::replaceByte( unsigned char * address, unsigned char original, unsigned char replacement, unsigned len )
{
    while( len > 0 ) {
        if( *address == original ) {
            *address = replacement;
        }
        address++;
        len--;
    }
}

TSample * TMachine::copySample( const unsigned char * buf, unsigned len )
{
    TMemoryInputStream is( buf, len );

    TSample * result = TSample::createFromWave( &is );

    return result;
}
