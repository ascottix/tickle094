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
#ifndef EMU_OPTION2PORT_H_
#define EMU_OPTION2PORT_H_

#include "emu_input.h"
#include "emu_ui.h"

class TOptionToPortHandler
{
public:
    TOptionToPortHandler() {
    }

    ~TOptionToPortHandler();

    void add( unsigned id, unsigned param, unsigned char * port, unsigned char and_mask, unsigned char or_mask );

    void add( unsigned char * port, TUserInterface * ui, unsigned id );

    void add( unsigned char * port, TUserInterface * ui, unsigned id1, unsigned id2 );

    void add( unsigned char * port, TUserInterface * ui, unsigned id1, unsigned id2, unsigned id3 );

    void add( unsigned char * port, TUserInterface * ui, unsigned id1, unsigned id2, unsigned id3, unsigned id4 );

    void add( unsigned char * port, TUserInterface * ui, unsigned id1, unsigned id2, unsigned id3, unsigned id4, unsigned id5 );

    void resynch( TUserInterface * ui );

    void clear();

    bool handleInputEvent( unsigned device, unsigned param );

private:
    unsigned xlatDevice( unsigned device, unsigned param ) {
        return (TInput::getDeviceId(device) << 8) | (param & 0xFF);
    }

    TList options_;
};

#endif // EMU_OPTION2PORT_H_
