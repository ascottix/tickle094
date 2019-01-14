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
#ifndef EMU_UI_H_
#define EMU_UI_H_

#include "emu_list.h"
#include "emu_string.h"

class TMachine;

class TUiOptionItem;

class TUiOption
{
public:
    TUiOption( unsigned id, const char * name, int def, unsigned char mask = 0 );

    /** Destructor. */
    virtual ~TUiOption();

    void add( const char * name, unsigned char value = 0 );

    unsigned id() const {
        return id_;
    }

    unsigned char mask() const {
        return mask_;
    }

    const char * name() const {
        return name_;
    }

    int selected() const {
        return selected_;
    }

    void restoreDefault() {
        select( default_ );
    }

    void select( int index );

    int count() const {
        return items_.count();
    }

    const char * item( int index ) const;

    unsigned char itemValue( int index ) const;

    void notify( TMachine * machine );

    void setName( const char * name ) {
        name_ = name;
    }

    void setDefault( int def ) {
        default_ = def;
    }

    void setItemName( int index, const char * name );

private:
    const char * name_;
    unsigned id_;
    unsigned char mask_;
    int default_;
    int selected_;
    TList items_;
};

class TUiOptionGroup
{
public:
    TUiOptionGroup( const char * name, unsigned type = 0 ) : name_(name), type_(type) {
    }

    /** Destructor. */
    virtual ~TUiOptionGroup();

    TUiOption * add( unsigned id, const char * name, int def, unsigned char mask = 0 );

    bool remove( unsigned id );

    int count() const {
        return items_.count();
    }

    TUiOption * item( int index ) {
        return (TUiOption *) items_[index];
    }

    void notify( TMachine * machine );

    const char * name() const {
        return name_;
    }

    void setName( const char * name ) {
        name_ = name;
    }

private:
    const char * name_;
    unsigned type_;
    TList items_;
};

class TUserInterface
{
public:
    TUserInterface() {
    }

    /** Destructor. */
    virtual ~TUserInterface();

    TUiOptionGroup * addGroup( const char * name, unsigned type = 0 );

    int groupCount() const {
        return items_.count();
    }

    TUiOptionGroup * group( int index ) {
        return (TUiOptionGroup *) items_[index];
    }

    TUiOptionGroup * group( const char * name );

    void notify( TMachine * machine );

    void removeOption( unsigned id ); 

    TUiOption * findOption( unsigned id );

    TUiOption * replaceOption( unsigned id, const char * name, int def, unsigned char mask = 0 );

    void renameOption( unsigned id, const char * name );

    void renameOptionItem( unsigned id, int index, const char * name );

    void setOptionDefault( unsigned id, int def, bool select = false );

    void clear();

private:
    TList items_;
};

#endif // EMU_UI_H_
