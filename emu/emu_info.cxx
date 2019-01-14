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
#include "emu_info.h"

TMachineDriverInfo::~TMachineDriverInfo() 
{
    clearResources();
}

void TMachineDriverInfo::clearResources()
{
    for( int i=0; i<resourceFileCount(); i++ ) {
        delete (TResourceFileInfo *) resourceFile( i );
    }

    resource_files_.clear();
}

void TMachineDriverInfo::addResourceFile( int id,  const char * name, unsigned crc, unsigned size, TResourceFileType type ) 
{
    // Delete any existing item with the same identifier
    for( int i=0; i<resourceFileCount(); i++ ) {
        TResourceFileInfo * fi = (TResourceFileInfo *) resource_files_.item(i);

        if( fi->id == id ) {
            resource_files_.remove( i );
            delete fi;
            break;
        }
    }

    // Add the item
    resource_files_.add( new TResourceFileInfo( id, name, crc, size, type ) );
}
