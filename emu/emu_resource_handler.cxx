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
#include "emu_machine.h"
#include "emu_resource_handler.h"

struct TResourceHandlerItem : public TResourceFileInfo
{
    TResourceHandlerItem( int i, const char * n, unsigned c, unsigned s, TResourceFileType t, unsigned char * b );

    unsigned char * buf;
};

TResourceHandlerItem::TResourceHandlerItem( int i, const char * n, unsigned c, unsigned s, TResourceFileType t, unsigned char * b )
    : TResourceFileInfo( i, n, c, s, t ), buf( b )
{
}

TResourceHandler::~TResourceHandler()
{
    clear();
}

void TResourceHandler::clear()
{
    for( int i=0; i<items_.count(); i++ ) {
        TResourceHandlerItem * item = (TResourceHandlerItem *) items_.item(i);

        delete item;
    }

    items_.clear();
}

void TResourceHandler::add( int id, const char * name, unsigned len, TResourceFileType type, unsigned char * buf, unsigned crc )
{
    remove( id );

    TResourceHandlerItem * item = new TResourceHandlerItem( id, name, crc, len, type, buf );

    items_.add( item );
}

void TResourceHandler::replace( int id, const char * name, unsigned crc, unsigned char * buf )
{
    for( int i=0; i<items_.count(); i++ ) {
        TResourceHandlerItem * item = (TResourceHandlerItem *) items_.item(i);

        if( item->id == id ) {
            item->name = name;
            item->crc = crc;
            if( buf != 0 ) {
                item->buf = buf;
            }
            break;
        }
    }
}

void TResourceHandler::replace( TResourceHandlerReplaceInfo * info, int count )
{
    while( 1 ) {
        replace( info->id, info->name, info->crc );

        count--;

        if( count == 0 )
            break;

        info++;

        if( info->name == 0 )
            break;
    }
}

void TResourceHandler::remove( int id )
{
    for( int i=0; i<items_.count(); i++ ) {
        TResourceHandlerItem * item = (TResourceHandlerItem *) items_.item(i);

        if( item->id == id ) {
            items_.remove( i );
            delete item;
            break;
        }
    }
}

void TResourceHandler::addToMachineDriverInfo( TMachineDriverInfo * di )
{
    for( int i=0; i<items_.count(); i++ ) {
        TResourceHandlerItem * item = (TResourceHandlerItem *) items_.item(i);

        di->addResourceFile( item->id, item->name, item->crc, item->size, item->type );
    }
}

int TResourceHandler::handle( int id, const unsigned char * buf, unsigned len )
{
    int result = -1;

    for( int i=0; i<items_.count(); i++ ) {
        TResourceHandlerItem * item = (TResourceHandlerItem *) items_.item(i);

        if( item->id == id ) {
            if( TMachine::copyResourceFile( item->buf, item->size, buf, len ) ) {
                result = 0;
            }

            break;
        }
    }

    return result;
}
