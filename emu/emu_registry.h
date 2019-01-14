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
#ifndef EMU_REGISTRY_H_
#define EMU_REGISTRY_H_

#include "emu_info.h"
#include "emu_list.h"
#include "emu_machine.h"

class TGameRegistryItem
{
public:
    TGameRegistryItem( const TMachineInfo * info, TMachineFactoryFunc factory ) : info_(info), factory_(factory) {
    }

    const char * name() const {
        return info_->name;
    }

    const TMachineInfo * info() const {
        return info_;
    }

    TMachineFactoryFunc factory() const {
        return factory_;
    }

private:
    const TMachineInfo * info_;
    TMachineFactoryFunc factory_;
};

class TGameRegistry
{
public:
    /** Destructor. */
    ~TGameRegistry();

    void add( TGameRegistryItem * item );

    int count() const {
        return entries_.count();
    }

    const TGameRegistryItem * item( int index ) const {
        return (TGameRegistryItem *) entries_[ index ];
    }

    int find( const char * name ) const;

    void sort();
    
    TString getAsJson();
    
    static TGameRegistry & instance();

private:
    TGameRegistry() {
    }

    TList entries_;
};

class TGameRegistrationHandler
{
public:
    TGameRegistrationHandler( const TMachineInfo * info, TMachineFactoryFunc factory );
};

#endif // EMU_REGISTRY_H_
