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

#include "emu_list.h"

TList::TList()
{
    items_ = 0;
    capacity_ = 0;
    count_ = 0;
}

TList::~TList()
{
    clear();
}

int TList::add( void * item )
{
    expand();

    items_[count_++] = item;

    return count_-1;
}

void TList::clear()
{
    delete items_;
    items_ = 0;
    capacity_ = 0;
    count_ = 0;
}

void TList::remove( int index )
{
    if( (index >= 0) && (index < count_) ) {
        for( int i=index+1; i<count_; i++ ) {
            items_[i-1] = items_[i];
        }
        count_--;
    }
}

void TList::exchange( int index1, int index2 )
{
    if( (index1 >= 0) && (index1 < count_) && (index2 >= 0) && (index2 < count_) ) {
        void * temp = items_[index1];
        items_[index1] = items_[index2];
        items_[index2] = temp;
    }
}

int TList::indexOf( void * item ) const
{
    int result = -1;

    for( int i=0; i<count_; i++ ) {
        if( items_[i] == item ) {
            result = i;
            break;
        }
    }

    return result;
}

void TList::insert( int index, void * item )
{
    if( index == count_ ) {
        add( item );
    }
    else if( (index >= 0) && (index < count_) ) {
        expand();
        for( int i=count_; i>index; i-- ) {
            items_[i] = items_[i-1];
        }
        items_[index] = item;
        count_++;
    }
}

void * TList::item( int index ) const
{
    return (index >= 0) && (index < count_) ? items_[index] : 0;
}

void TList::expand()
{
    assert( count_ <= capacity_ );

    if( count_ == capacity_ ) {
        if( capacity_ <= 4 )
            capacity_ += 4;
        else if( capacity_ <= 8 )
            capacity_ += 8;
        else
            capacity_ += 16;

        void ** new_items_ = new void * [capacity_];

        for( int i=0; i<count_; i++ ) {
            new_items_[i] = items_[i];
        }

        delete items_;

        items_ = new_items_;
    }
}
