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
#include "emu_joystick2port.h"

enum {
    jpNone = -1
};

void TJoystickToPortHandler::initialize( unsigned device, int type, TJoystickMode mode )
{
    device_ = device;
    type_ = type;
    mode_ = mode;
    xpos_ = 0;
    ypos_ = 0;

    for( int i=0; i<4; i++ ) {
        port_[i] = 0;
    }
}

TJoystickToPortHandler::TJoystickToPortHandler( unsigned device, int type, TJoystickMode mode )
{
    initialize( device, type, mode );
}

TJoystickToPortHandler::TJoystickToPortHandler( unsigned device, int type, TJoystickMode mode, unsigned char * port, unsigned masks )
{
    initialize( device, type, mode );
    setPortAndMasks( port, masks );
}

TJoystickToPortHandler::~TJoystickToPortHandler()
{
}

void TJoystickToPortHandler::setPort( int index, unsigned char * port, unsigned char and_mask, unsigned char or_mask )
{
    if( (index >= 0) && (index < 4) && (and_mask != 0) ) {
        port_[index] = port;
        and_mask_[index] = and_mask;
        or_mask_[index] = (or_mask != 0) ? or_mask : and_mask;
    }
}

void TJoystickToPortHandler::setDirection( int direction1, int direction2 )
{
    for( int i=0; i<4; i++ ) {
        if( port_[i] ) {
            *(port_[i]) &= ~and_mask_[i];

            bool enabled = (i == direction1) || (i == direction2);

            if( (enabled && (type_ == ptNormal)) || (!enabled && (type_ == ptInverted)) ) {
                *(port_[i]) |= or_mask_[i];
            }
        }
    }
}

static int triChoice( int value, int on_negative, int on_zero, int on_positive )
{
    return (value > 0) ? on_positive : (value == 0) ? on_zero : on_negative;
}

bool TJoystickToPortHandler::handleInputEvent( unsigned device, unsigned param )
{
    bool result = false;

    if( device == device_ ) {
        int x = TInput::getXPosFromParam( param );
        int y = TInput::getYPosFromParam( param );

        if( (x != xpos_) || (y != ypos_) ) {
            if( mode_ == jm4Way ) {
                // 4-way joystick, pick one direction
                int direction;

                int xforce = TMath::abs( x );
                int yforce = TMath::abs( y );

                if( (xforce > yforce) || ((xforce == yforce) && (x != xpos())) ) {
                    // Joystick moved on the X axis
                    direction = triChoice( x, jpLeft, jpNone, jpRight );
                }
                else {
                    // Joystick moved on the Y axis
                    direction = triChoice( y, jpUp, jpNone, jpDown );
                }

                setDirection( direction, jpNone );
            }
            else {
                // 8-way joystick, pick two directions
                int direction1 = triChoice( x, jpLeft, jpNone, jpRight );
                int direction2 = triChoice( y, jpUp, jpNone, jpDown );

                setDirection( direction1, direction2 );
            }

            xpos_ = x;
            ypos_ = y;
        }

        result = true;
    }

    return result;
}

void TJoystickToPortHandler::setPortAndMasks( unsigned char * port, unsigned masks )
{
    for( int i=0; i<4; i++ ) {
        setPort( i, port, masks & 0xFF );
        masks >>= 8;
    }
}
