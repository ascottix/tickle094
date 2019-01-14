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
#ifndef DX_APP_H_
#define DX_APP_H_

#include <windows.h>
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>
#include <stdio.h>
#include <assert.h>

#include "dib32.h"
#include "dx_joystick.h"

class DxApp
{
public:
    enum {
        MaxJoysticks = 4
    };

    DxApp( HWND hWindow );

    virtual ~DxApp();

    HWND getWindow() const {
        return hWindow_;
    }

    Dib32 * getCanvas() {
        return dibCanvas_;
    }

    bool isInitialized() const {
        return (dwActiveFlags_ & AfInitMask) == AfInitOk;
    }

    bool isActive() const {
        return (dwActiveFlags_ & AfActiveMask) == AfActiveOk;
    }

    bool getActive() const {
        return (dwActiveFlags_ & AfUserActive) ? true : false;
    }

    void setActive( bool active ) {
        setActiveFlags( active ? dwActiveFlags_ | AfUserActive : dwActiveFlags_ & ~AfUserActive );
    }

    void setWindowSize( unsigned factor );

    bool setAudioParams( unsigned samplesPerFrame, unsigned samplesPerSecond );

    bool setVideoParams( unsigned width, unsigned height );

    void blitGameScreen();

    void prepareGameScreen();

    void synchToSoundBuffer( unsigned index );

    void writeToSoundBuffer( unsigned index, const int * buf, int voices );

    bool handleWindowMessage( HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam, LRESULT * lResult );

    bool initialize();

    void showWindow() {
        ShowWindow( hWindow_, SW_SHOWNORMAL );
        UpdateWindow( hWindow_ );
    }

    unsigned getSamplingRate() const {
        return nSamplesPerSecond_;
    }

    unsigned getNextFrame( unsigned frame ) {
        return (frame + 1) % nFrameBuffers_;
    }

    void copySnapshotToClipboard();

    //
    bool onInputDeviceEnum( LPCDIDEVICEINSTANCE pdinst );

    DxJoystick * joystick( int index ) {
        return (index >= 0) && (index < diJoystickCount_) ? diJoystick_[index] : 0;
    }

private:
    HRESULT createDxObject( LPVOID lppObject, HRESULT hRes );

    HRESULT initializeDirectDraw();
    void terminateDirectDraw();

    HRESULT initializeDirectSound();
    void terminateDirectSound();
    HRESULT createSoundBuffer();
    void deleteSoundBuffer();

    HRESULT initializeDirectInput();
    void terminateDirectInput();

    void setActiveFlags( DWORD flags );

    enum {
        AfDirectDrawOk      = 0x01,
        AfDirectSoundOk     = 0x02,
        AfWindowOk          = 0x04,
        AfMenuLoop          = 0x08,
        AfSizeLoop          = 0x10,
        AfAppActive         = 0x20,
        AfUserActive        = 0x40,
        AfAllFlagsMask      = 0x4F,
        AfInitOk            = AfDirectDrawOk | AfDirectSoundOk | AfWindowOk,
        AfInitMask          = AfInitOk,
        AfActiveOk          = AfInitMask | AfAppActive | AfUserActive,
        AfActiveMask        = AfActiveOk | AfMenuLoop | AfSizeLoop
    };

    unsigned nClientWidth_;
    unsigned nClientHeight_;
    unsigned nSizeFactor_;
    DWORD dwActiveFlags_;
    HWND hWindow_;
    Dib32 * dibCanvas_;
    unsigned nSamplesPerSecond_;
    unsigned nBytesPerSample_;
    unsigned nSamplesPerFrame_;
    unsigned nSampleBytesPerFrame_;
    unsigned nFrameBuffers_;
   
    IDirectDraw * directDraw_;
    IDirectDrawSurface * ddPrimarySurface_;
    IDirectDrawClipper * ddClipper_;
    IDirectDrawSurface * ddBackSurface_;

    IDirectInput * directInput_;
    DxJoystick * diJoystick_[ MaxJoysticks ];
    int diJoystickCount_;
    
    IDirectSound * directSound_;
    IDirectSoundBuffer * directSoundBuf_;
};

#endif // DX_APP_H_
