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
#ifndef EMU_EVENT2PORT_H_
#define EMU_EVENT2PORT_H_

enum TEventToPortItemType 
{
    ptNormal,
    ptInverted
};

class TEventToPortItem;

class TEventToPortHandler
{
public:
    TEventToPortHandler();

    /** Destructor. */
    ~TEventToPortHandler();

    void add( unsigned device, int type, unsigned char * port, unsigned char and_mask, unsigned char or_mask );

    void add( unsigned device, int type, unsigned char * port, unsigned char and_mask ) {
        add( device, type, port, and_mask, and_mask );
    }

    bool remove( unsigned device );

    bool handleInputEvent( unsigned device, unsigned param );

private:
    void expand();

    int count_;
    int capacity_;
    TEventToPortItem * items_;
};

#endif // EMU_EVENT2PORT_H_
