/*
     Tickle front-end
     User interface for Mac OS X
     
     Copyright (c) 2006 Alessandro Scotti
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
#ifndef TICKLE_JOYSTICK_H_
#define TICKLE_JOYSTICK_H_

#include <sys/errno.h>
#include <sysexits.h>
#include <mach/mach.h>
#include <mach/mach_error.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hid/IOHIDUsageTables.h>
#include <IOKit/usb/USB.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>

const int MaxGamePadButtons = 8;

struct GamePadInfo 
{
    GamePadInfo() {
        xAxisCookie = 0;
        yAxisCookie = 0;
        for( int i=0; i<MaxGamePadButtons; i++ ) {
            buttonCookie[i] = 0;
        }
    }
    
    bool isOk() const {
        return xAxisCookie != 0 && buttonCookie[0] != 0;
    }
    
    IOHIDElementCookie xAxisCookie;
    IOHIDElementCookie yAxisCookie;
    IOHIDElementCookie buttonCookie[8];
};

class HidGamePad
{
public:
    HidGamePad( const GamePadInfo & info, IOHIDDeviceInterface ** hidDeviceInterface ) {
        gp_ = info;
        hidDeviceInterface_ = hidDeviceInterface;
        poll();
    }
    
    void poll();
    
    int x() const {
        return x_;
    }
    
    int y() const {
        return y_;
    }
    
    unsigned buttons() const {
        return buttons_;
    }
    
    IOHIDDeviceInterface ** deviceInterface() {
        return hidDeviceInterface_;
    }
    
private:
    GamePadInfo gp_;
    IOHIDDeviceInterface ** hidDeviceInterface_;
    int x_;
    int y_;
    unsigned buttons_;
};

int initJoystickLib();
int termJoystickLib();

HidGamePad * getJoystick( int index );

int getJoystickCount();

#endif // TICKLE_JOYSTICK_H_
