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
#ifndef EMU_STANDARD_MACHINE_H_
#define EMU_STANDARD_MACHINE_H_

#include <stdio.h>
#include <string.h>

#include "emu_driver.h"

class TStandardMachine : public TMachine
{
public:
    virtual ~TStandardMachine();

    virtual bool handleInputEvent( unsigned device, unsigned param, void * data = 0 );

protected:
    TStandardMachine();

    void createScreen( int width, int height, int colors );

    TBitmapIndexed * screen() {
        return screen_;
    }

    TPalette * palette() {
        return palette_;
    }

    TEventToPortHandler * eventHandler() {
        return &event_handler_;
    }

    TOptionToPortHandler * optionHandler() {
        return &option_handler_;
    }

    TJoystickToPortHandler * joystickHandler( int index ) {
        return joystick_handler_[index];
    }

    TResourceHandler * resourceHandler() {
        return &resource_handler_;
    }

    void setJoystickHandler( int index, TJoystickToPortHandler * jh ) {
        joystick_handler_[index] = jh;
    }

private:
    TBitmapIndexed *   screen_;
    TPalette *  palette_;    
    TEventToPortHandler     event_handler_;
    TOptionToPortHandler    option_handler_;
    TJoystickToPortHandler *joystick_handler_[4];
    TResourceHandler        resource_handler_;
};

#endif // EMU_STANDARD_MACHINE_H_
