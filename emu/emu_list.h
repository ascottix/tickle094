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
#ifndef EMU_LIST_H_
#define EMU_LIST_H_

class TList {
public:
    TList();

    /** Destructor. */
    virtual ~TList();

    int add( void * item );

    void clear();

    void remove( int index );

    void exchange( int index1, int index2 );

    int indexOf( void * item ) const;

    void insert( int index, void * item );

    void * item( int index ) const;

    void * operator [] (int index) const {
        return item( index );
    }

    int count() const {
        return count_;
    }

private:
    TList( const TList & list );
    TList & operator = ( const TList & list );

    void expand();

    void ** items_;
    int capacity_;
    int count_;
};

#endif // EMU_LIST_H_
