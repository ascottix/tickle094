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
#ifndef EMU_RESOURCE_HANDLER_H_
#define EMU_RESOURCE_HANDLER_H_

#include "emu_info.h"

struct TResourceHandlerReplaceInfo
{
    int id;
    const char * name;
    unsigned crc;
};

class TResourceHandler
{
public:
    TResourceHandler() {
    }

    ~TResourceHandler();

    void add( int id, const char * name, unsigned len, TResourceFileType type, unsigned char * buf, unsigned crc = 0 );

    void replace( int id, const char * name, unsigned crc, unsigned char * buf = 0 );

    void replace( TResourceHandlerReplaceInfo * info, int count = -1 );

    void remove( int id );

    void clear();

    void addToMachineDriverInfo( TMachineDriverInfo * di );

    void assignToMachineDriverInfo( TMachineDriverInfo * di ) {
        di->clearResources();
        addToMachineDriverInfo( di );
    }

    int handle( int id, const unsigned char * buf, unsigned len );

private:
    TList items_;
};

#endif // EMU_RESOURCE_HANDLER_H_
