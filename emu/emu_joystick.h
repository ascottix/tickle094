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
#ifndef EMU_JOYSTICK_H_
#define EMU_JOYSTICK_H_

#include "emu_input.h"
#include "emu_machine.h"

class TJoystick
{
public:
    TJoystick( unsigned id );

    void setPosition( int x, int y );

    void setButtons( unsigned buttons ) {
        buttons_ = buttons;
    }

    void setDeadZone( int lo, int hi ) {
        dead_area_lo_ = lo;
        dead_area_hi_ = hi;
    }

    void bindButtonToKey( unsigned button, unsigned key ) {
        if( button < MaxButtons ) {
            button2key_[button] = key;
        }
    }

    void handleArrowKey( int key, unsigned pressed );

    void notify( TMachine * machine );

    unsigned id() const {
        return id_;
    }

    enum {
        MaxButtons = 12,
        KeyLeft = 1,
        KeyRight,
        KeyUp,
        KeyDown
    };

private:
    unsigned id_;
    int x_;
    int y_;
    unsigned buttons_;
    int last_x_;
    int last_y_;
    unsigned last_buttons_;
    int dead_area_lo_;
    int dead_area_hi_;
    unsigned equivalent_keys_state_;
    unsigned button2key_[MaxButtons];
};

#endif // EMU_JOYSTICK_H_
