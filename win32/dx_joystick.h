/*
    Tickle front-end
    User interface for Windows

    Copyright (c) 2004 Alessandro Scotti
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
#ifndef DX_JOYSTICK_H_
#define DX_JOYSTICK_H_

#include <windows.h>
#include <dinput.h>

#ifndef IDirectInput
#define IDirectInput IDirectInputA
#define IDirectInputDevice IDirectInputDeviceA
#define IDirectInputDevice2 IDirectInputDevice2A
#endif

#include <emu/emu_frame.h>

class DxJoystick
{
public:
    enum {
        JoyRange = 10000,
        JoyInactiveRange = 250
    };

    ~DxJoystick();

    static DxJoystick * create( HWND hWindow, IDirectInput * di, GUID guid );

    bool poll();

    void setRange( int range ) {
        range_ = range;
    }

    int x() const {
        return state_.lX;
    }

    int y() const {
        return state_.lY;
    }

    int rx() const {
        return state_.lRz;
    }

    int ry() const {
        return state_.lZ;
    }

    unsigned buttons() const;

    bool hasForceFeedback() const {
        return (caps_.dwFlags & DIDC_FORCEFEEDBACK) != 0;
    }

    bool playEffect( int effect, unsigned param );

    void stopEffect();

    static void setForceFeedbackStrength( DWORD strength ) {
        forceFeedbackStrength = strength;
    }

    static DWORD getForceFeedbackStrength() {
        return forceFeedbackStrength;
    }

private:
    static DWORD forceFeedbackStrength;

    DxJoystick();

    HRESULT initialize( HWND hWindow, IDirectInput * di, GUID guid );

    IDirectInputDevice2 * device_;
    DIJOYSTATE state_;
    DIDEVCAPS caps_;
    int range_;
    IDirectInputEffect * effect_;
};

#endif // DX_JOYSTICK_H_
