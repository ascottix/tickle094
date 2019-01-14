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
#ifndef EMU_INPUT_MANAGER_H_
#define EMU_INPUT_MANAGER_H_

#include "emu_joystick.h"
#include "emu_list.h"
#include "emu_machine.h"

struct TEmuInputManagerItem;

class TEmuInputManager
{
public:
    TEmuInputManager();

    ~TEmuInputManager();

    void add( unsigned key, unsigned id, unsigned data = 0 );

    TJoystick * addJoystick( unsigned id, unsigned kleft, unsigned kright, unsigned kup, unsigned kdown );

    void clear();

    bool handle( unsigned key, unsigned param, TMachine * machine );

    void notifyJoysticks( TMachine * machine );

    TJoystick * joystick( int index ) {
        return (TJoystick *) joysticks_.item(index);
    }

private:
    void expand();

    int count_;
    int capacity_;
    TEmuInputManagerItem * items_;
    TList joysticks_;
};

#endif // EMU_INPUT_MANAGER_H_
