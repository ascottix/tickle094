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
#include <stdio.h>
#include <string.h>

#include "emu_registry.h"

TGameRegistry::~TGameRegistry()
{
    for( int i=0; i<count(); i++ ) {
        TGameRegistryItem * t = (TGameRegistryItem *) entries_[i];
        delete t;
    }
}

void TGameRegistry::add( TGameRegistryItem * item )
{
    entries_.add( item );
}

int TGameRegistry::find( const char * name ) const
{
    int index;

    for( index=count(); index>0; index-- ) {
        if( strcmp( item(index-1)->name(), name ) == 0 )
            break;
    }

    return index-1;
}

void TGameRegistry::sort()
{
    int n = count();

    for( int i=0; i<n-1; i++ ) {
        int k = i;

        for( int j=i+1; j<n; j++ ) {
            if( strcmp( item(j)->name(), item(k)->name() ) < 0 ) {
                k = j;
            }
        }

        if( k != i ) {
            entries_.exchange( i, k );
        }
    }
}

TString TGameRegistry::getAsJson()
{
    TString s;
    
    s.append( "[\n" );
    
    for( int i=0; i<count(); i++ ) {
        const TGameRegistryItem * item = (TGameRegistryItem *) entries_[i];
        const TMachineInfo * info = item->info();
        TMachine * machine = TMachine::createInstance( item->factory() );
        
        TString resources;

        for( int j=0; j<machine->getResourceCount(); j++ ) {
            if( j > 0 ) resources.append( ", " );
            resources.append( "\"" );
            resources.append( machine->getResourceName( j ) );
            resources.append( "\"" );
        }
        
        char buffer[1024];
        
        sprintf( buffer, "\t{\n"
                "\t\t\"driver\": \"%s\",\n"
                "\t\t\"name\": \"%s\",\n"
                "\t\t\"manufacturer\": \"%s\",\n"
                "\t\t\"resources\": [%s],\n"
                "\t\t\"year\": %d,\n"
                "\t\t\"width\": %d,\n"
                "\t\t\"height\": %d\n"
                "\t}",
                info->driver, info->name, info->manufacturer, resources.cstr(), info->year, info->screenWidth, info->screenHeight );
        
        s.append( buffer );
        if( (i+1) < count() ) s.append( "," );
        s.append( "\n");
        
        delete machine;
    }
    
    s.append( "]\n" );
    
    return s;
}

TGameRegistry & TGameRegistry::instance()
{
    static TGameRegistry theGameRegistry;

    return theGameRegistry;
}

TGameRegistrationHandler::TGameRegistrationHandler( const TMachineInfo * info, TMachineFactoryFunc factory )
{
    TGameRegistry::instance().add( new TGameRegistryItem( info, factory ) );
}
