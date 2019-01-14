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
#include "emu_standard_machine.h"

TStandardMachine::TStandardMachine() 
{
    screen_ = 0;
    palette_ = 0;

    for( int i=0; i<4; i++ ) {
        joystick_handler_[i] = 0;
    }
}

TStandardMachine::~TStandardMachine() 
{
    delete screen_;
    delete palette_;

    for( int i=0; i<4; i++ ) {
        delete joystick_handler_[i];
    }
}

bool TStandardMachine::handleInputEvent( unsigned device, unsigned param, void * data ) 
{
    return 
        ((joystick_handler_[0] != 0) && (joystick_handler_[0]->handleInputEvent(device,param)) ) ||
        ((joystick_handler_[1] != 0) && (joystick_handler_[1]->handleInputEvent(device,param)) ) ||
        ((joystick_handler_[2] != 0) && (joystick_handler_[2]->handleInputEvent(device,param)) ) ||
        ((joystick_handler_[3] != 0) && (joystick_handler_[3]->handleInputEvent(device,param)) ) ||
        event_handler_.handleInputEvent(device,param) ||
        option_handler_.handleInputEvent(device,param);
}

void TStandardMachine::createScreen( int width, int height, int colors ) 
{
    palette_ = new TPalette( colors );
    screen_ = new TBitmapIndexed( width, height, palette_ );
}
