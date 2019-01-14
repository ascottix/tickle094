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
#include "dx_joystick.h"

DWORD DxJoystick::forceFeedbackStrength = 5000; // Default is half strength

DxJoystick * DxJoystick::create( HWND hWindow, IDirectInput * di, GUID guid )
{
    DxJoystick * result = new DxJoystick();

    if( result->initialize( hWindow, di, guid ) != DI_OK ) {
        delete result;
        result = 0;
    }

    return result;
}

DxJoystick::DxJoystick()
{
    device_ = 0;
    range_ = 0;
    effect_ = 0;
}

DxJoystick::~DxJoystick()
{
    stopEffect();

    if( device_ != 0 ) {
        device_->Release();
    }
}

BOOL CALLBACK onEnumEffectsCallback( LPCDIEFFECTINFO pdei, LPVOID pdata )
{
    return DIENUM_CONTINUE;
}

HRESULT DxJoystick::initialize( HWND hWindow, IDirectInput * di, GUID guid )
{
    IDirectInputDevice * temp_device;

    HRESULT res = di->CreateDevice( guid, &temp_device, 0 );

    // Upgrade to the IDirectInputDevice2 interface
    if( res == DI_OK ) {
        res = temp_device->QueryInterface( IID_IDirectInputDevice2, (void **) &device_ );
        // Note: we release the "old" interface anyway, even if the above conversion failed
        temp_device->Release();
    }

    // Get the device capabilities
    if( res == DI_OK ) {
        caps_.dwSize = sizeof(caps_);
        res = device_->GetCapabilities( &caps_ );
    }

    // Set the data format
    if( res == DI_OK ) {
        res = device_->SetDataFormat( &c_dfDIJoystick );
    }

    // Set the cooperative level
    if( res == DI_OK ) {
        if( hasForceFeedback() ) {
            // Force-feedback joystick: operate in exclusive mode
            res = device_->SetCooperativeLevel( hWindow, DISCL_FOREGROUND|DISCL_EXCLUSIVE );

            // Turn off autocenter
            DIPROPDWORD dipw;

            dipw.diph.dwSize = sizeof(dipw);
            dipw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
            dipw.diph.dwObj = 0;
            dipw.diph.dwHow = DIPH_DEVICE;
            dipw.dwData = 0;

            device_->SetProperty(DIPROP_AUTOCENTER, &dipw.diph);

            // Enumerate effects
            device_->EnumEffects( onEnumEffectsCallback, 0, DIEFT_ALL );
        }
        else {
            // Standard joystick
            res = device_->SetCooperativeLevel( hWindow, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE );
        }
    }

    // Set the device range
    if( res == DI_OK ) {
        DIPROPRANGE diprg; 

        diprg.diph.dwSize       = sizeof(diprg); 
        diprg.diph.dwHeaderSize = sizeof(diprg.diph); 
        diprg.diph.dwObj        = DIJOFS_X; 
        diprg.diph.dwHow        = DIPH_BYOFFSET;
        diprg.lMin              = -10000;
        diprg.lMax              = +10000;

        res = device_->SetProperty( DIPROP_RANGE, &diprg.diph );

        if( res == DI_OK ) {
            diprg.diph.dwObj = DIJOFS_Y;
            res = device_->SetProperty( DIPROP_RANGE, &diprg.diph );
        }

        if( res == DI_OK ) {
            diprg.diph.dwObj = DIJOFS_Z;
            res = device_->SetProperty( DIPROP_RANGE, &diprg.diph );
        }

        if( res == DI_OK ) {
            diprg.diph.dwObj = DIJOFS_RZ;
            res = device_->SetProperty( DIPROP_RANGE, &diprg.diph );
        }
    }

    // Acquire the device
    if( res == DI_OK ) {
        res = device_->Acquire();
    }

    // Make sure that everything went ok!
    if( res != DI_OK ) {
        if( device_ != 0 ) {
            device_->Release();
            device_ = 0;
        }
    }

    return res;
}

bool DxJoystick::poll()
{
    bool result = false;

    if( device_ != 0 ) {
        device_->Poll();

        HRESULT res = device_->GetDeviceState( sizeof(state_), &state_ );

        if( res == DIERR_INPUTLOST ) {
            device_->Acquire();
            res = device_->GetDeviceState( sizeof(state_), &state_ );
        }

        if( res == DI_OK ) {
            if( (state_.lX >= -JoyInactiveRange) && (state_.lX <= +JoyInactiveRange) ) {
                state_.lX = 0;
            }

            if( (state_.lY >= -JoyInactiveRange) && (state_.lY <= +JoyInactiveRange) ) {
                state_.lY = 0;
            }

            if( (state_.lZ >= -JoyInactiveRange) && (state_.lZ <= +JoyInactiveRange) ) {
                state_.lZ = 0;
            }

            if( (state_.lRz >= -JoyInactiveRange) && (state_.lRz <= +JoyInactiveRange) ) {
                state_.lRz = 0;
            }

            if( range_ > 0 ) {
                // Rescale values to the specified range
                state_.lX = state_.lX * range_ / JoyRange;
                state_.lY = state_.lY * range_ / JoyRange;
                state_.lZ = state_.lZ * range_ / JoyRange;
                state_.lRz = state_.lRz * range_ / JoyRange;
            }

            result = true;
        }
    }

    return result;
}

unsigned DxJoystick::buttons() const
{
    unsigned result = 0;

    for( int i=0; i<8; i++ ) {
        if( state_.rgbButtons[i] & 0x80 ) {
            result |= (1 << i);
        }
    }

    return result;
}

bool DxJoystick::playEffect( int effect, unsigned param )
{
    bool result = false;

    if( (forceFeedbackStrength > 0) && hasForceFeedback() ) {
        stopEffect();

        GUID guid;
        DIEFFECT dieffect;
        DIPERIODIC diperiodic;
        DWORD diaxes[2];
        LONG didirection[2];
        // DIENVELOPE dienvelope;

        ZeroMemory( &dieffect, sizeof(dieffect) );

        if( effect == fxFF_Explosion ) {
            guid = GUID_SawtoothDown;

            diperiodic.dwMagnitude = 10000;
            diperiodic.lOffset = 0;
            diperiodic.dwPhase = 0;
            diperiodic.dwPeriod = 10*1000; // Microseconds

            diaxes[0] = DIJOFS_X;
            diaxes[1] = DIJOFS_Y;

            didirection[0] = 0;
            didirection[1] = 0;

            dieffect.dwSize = sizeof(dieffect);
            dieffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
            dieffect.dwDuration = INFINITE;
            dieffect.dwGain = forceFeedbackStrength;
            dieffect.dwTriggerButton = DIEB_NOTRIGGER;
            dieffect.cAxes = 2;
            dieffect.rgdwAxes = diaxes;
            dieffect.rglDirection = didirection;
            dieffect.cbTypeSpecificParams = sizeof(diperiodic);
            dieffect.lpvTypeSpecificParams = &diperiodic;
        }
        else {
            guid = GUID_SawtoothUp;

            diperiodic.dwMagnitude = 10000;
            diperiodic.lOffset = 0;
            diperiodic.dwPhase = 0;
            diperiodic.dwPeriod = 100*1000; // Microseconds

            diaxes[0] = DIJOFS_X;
            diaxes[1] = DIJOFS_Y;

            didirection[0] = 0;
            didirection[1] = 0;

            dieffect.dwSize = sizeof(dieffect);
            dieffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
            dieffect.dwDuration = 100*1000;
            dieffect.dwGain = forceFeedbackStrength;
            dieffect.dwTriggerButton = DIEB_NOTRIGGER;
            dieffect.cAxes = 2;
            dieffect.rgdwAxes = diaxes;
            dieffect.rglDirection = didirection;
            dieffect.cbTypeSpecificParams = sizeof(diperiodic);
            dieffect.lpvTypeSpecificParams = &diperiodic;
        }

        HRESULT res = device_->CreateEffect( guid, &dieffect, &effect_, 0 );

        if( res == DI_OK ) {
            res = effect_->Start( 1, 0 ); // One iteration, no flags

            result = (res == DI_OK);
        }
    }

    return result;
}

void DxJoystick::stopEffect()
{
    if( effect_ != 0 ) {
        effect_->Stop();
        effect_->Release();
        effect_ = 0;
    }
}
