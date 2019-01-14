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
#include "dx_app.h"

DxApp::DxApp( HWND hWindow )
{
    hWindow_ = hWindow;

    // Initialize variables        
    dwActiveFlags_ = 0;
    nClientWidth_ = 0;
    nClientHeight_ = 0;
    nSizeFactor_ = 1;
    directDraw_ = 0;
    ddPrimarySurface_ = 0;
    ddClipper_ = 0;
    ddBackSurface_ = 0;
    directSound_ = 0;
    directSoundBuf_ = 0;
    directInput_ = 0;
    diJoystickCount_ = 0;
    dibCanvas_ = 0;
    nSamplesPerSecond_ = 0;
    nBytesPerSample_ = 2;       // 16-bit audio
    nFrameBuffers_ = 3;         // Triple buffering (to avoid latency problems)
}

DxApp::~DxApp()
{
    delete dibCanvas_;
    terminateDirectDraw();
    terminateDirectSound();
    terminateDirectInput();
}

bool DxApp::initialize()
{
    if( hWindow_ != NULL ) {
        dwActiveFlags_ |= AfWindowOk;
        if( hWindow_ == GetActiveWindow() ) {
            dwActiveFlags_ |= AfAppActive;
        }
    }

    // Initialize for using the DirectDraw library
    if( initializeDirectDraw() == DD_OK ) {
        dwActiveFlags_ |= AfDirectDrawOk;
    }

    // Initialize for using the DirectSound library
    if( initializeDirectSound() == DD_OK ) {
        dwActiveFlags_ |= AfDirectSoundOk;
    }

    // Initialize the DirectInput library
    if( initializeDirectInput() == DD_OK ) {
        // Nothing special to do!!!
    }

    return isInitialized();
}

BOOL FAR PASCAL DxInputDeviceCallback( LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef )
{
    DxApp * gapp = (DxApp *) pvRef;

    gapp->onInputDeviceEnum( pdinst );

    return DIENUM_CONTINUE; 
}

bool DxApp::onInputDeviceEnum( LPCDIDEVICEINSTANCE pdinst )
{
    if( diJoystickCount_ < MaxJoysticks ) {
        DxJoystick * joy = DxJoystick::create( hWindow_, directInput_, pdinst->guidInstance );

        if( joy != 0 ) {
            diJoystick_[ diJoystickCount_ ] = joy;
            diJoystickCount_++;
        }
    }

    return true;
}

HRESULT DxApp::initializeDirectInput()
{
    // Create direct draw interface
    HRESULT res = createDxObject( &directInput_, DirectInputCreate( GetModuleHandle(NULL), DIRECTINPUT_VERSION, &directInput_, NULL ) );

    // Set cooperative level
    if( res == DD_OK ) {
        res = directDraw_->SetCooperativeLevel( hWindow_, DDSCL_NORMAL );
    }

    // Enumerate devices
    if( res == DD_OK ) {
        directInput_->EnumDevices( DIDEVTYPE_JOYSTICK, DxInputDeviceCallback, this, DIEDFL_ATTACHEDONLY );
    }

    return res;
}

void DxApp::terminateDirectInput()
{
    if( directInput_ != 0 ) {
        for( int i=0; i<diJoystickCount_; i++ ) {
            delete diJoystick_[ i ];
        }

        directInput_->Release();
        directInput_ = 0;
    }
}

HRESULT DxApp::createDxObject( LPVOID lppObject, HRESULT hRes )
{
    if( hRes != DD_OK ) {
        *((LPVOID *) lppObject) = 0;
    }

    return hRes;
}

bool DxApp::setAudioParams( unsigned samplesPerFrame, unsigned samplesPerSecond )
{
    bool result = false;

    if( (nSamplesPerSecond_ == samplesPerSecond) && (nSamplesPerFrame_ == samplesPerFrame) ) {
        result = true;
    }
    else {
        nSamplesPerSecond_ = samplesPerSecond;
        nSamplesPerFrame_ = samplesPerFrame;
        nBytesPerSample_ = 2;
        nSampleBytesPerFrame_ = nBytesPerSample_ * nSamplesPerFrame_;

        result = DD_OK == createSoundBuffer();
    }

    return result;
}

bool DxApp::setVideoParams( unsigned width, unsigned height )
{
    bool result = false;

    if( (nClientWidth_ == width) && (nClientHeight_ == height) ) {
        result = true;
    }
    else {
        if( directDraw_ != 0 ) {
            // Release existing back buffer
            if( ddBackSurface_ != 0 ) {
                ddBackSurface_->Release();
            }

            // Create a new back buffer
            DDSURFACEDESC sd;

            ZeroMemory( &sd, sizeof(sd) );
            sd.dwSize = sizeof(sd);
            sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
            sd.dwWidth = width;
            sd.dwHeight = height;
            sd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

            result = DD_OK == createDxObject( &ddBackSurface_, directDraw_->CreateSurface( &sd, &ddBackSurface_, NULL ) );
        }

        if( result ) {
            nClientWidth_ = width;
            nClientHeight_ = height;

            delete dibCanvas_;

            dibCanvas_ = new Dib32( width, height );

            setWindowSize( nSizeFactor_ );
        }
    }

    return result;
}

void DxApp::terminateDirectDraw()
{
    if( directDraw_ != 0 ) {
        if( ddBackSurface_ != 0 ) {
            ddBackSurface_->Release();
        }

        if( ddClipper_ != 0 ) {
            ddClipper_->Release();
        }

        if( ddPrimarySurface_ != 0 ) {
            ddPrimarySurface_->Release();
        }

        directDraw_->Release();
        directDraw_ = 0;
    }
}

HRESULT DxApp::initializeDirectDraw()
{
    HRESULT res;

    // Create direct draw interface
    res = createDxObject( &directDraw_, DirectDrawCreate( NULL, &directDraw_, NULL ) );

    // Set cooperative level
    if( res == DD_OK ) {
        res = directDraw_->SetCooperativeLevel( hWindow_, DDSCL_NORMAL );
    }

    // Create primary surface
    if( res == DD_OK ) {
        DDSURFACEDESC sd;

        ZeroMemory( &sd, sizeof(sd) );
        sd.dwSize = sizeof(sd);
        sd.dwFlags = DDSD_CAPS;
        sd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

        res = createDxObject( &ddPrimarySurface_, directDraw_->CreateSurface( &sd, &ddPrimarySurface_, NULL ) );
    }

    // Create clipper for the primary surface
    if( res == DD_OK ) {
        res = createDxObject( &ddClipper_, directDraw_->CreateClipper( 0, &ddClipper_, NULL ) );
    }

    if( res == DD_OK ) {
        res = ddClipper_->SetHWnd( 0, hWindow_ );
    }

    if( res == DD_OK ) {
        res = ddPrimarySurface_->SetClipper( ddClipper_ );
    }

    // Free resources if some error occurred
    if( res != DD_OK ) {
        terminateDirectDraw();
    }

    return res;
}

HRESULT DxApp::initializeDirectSound()
{
    HRESULT res;

    // Create direct sound interface
    res = createDxObject( &directSound_, DirectSoundCreate(NULL, &directSound_, NULL) );

    if( res == DS_OK ) {
        res = directSound_->SetCooperativeLevel( hWindow_, DSSCL_NORMAL );
    }

    return res;
}

void DxApp::deleteSoundBuffer()
{
    if( directSoundBuf_ != 0 ) {
        directSoundBuf_->Stop();
        directSoundBuf_->Release();
        directSoundBuf_ = 0;
    }
}

HRESULT DxApp::createSoundBuffer()
{
    HRESULT res;

    // Get rid of existing buffer if any
    deleteSoundBuffer();

    // Create sound buffer
    DSBUFFERDESC bufdesc = { 0 };
    WAVEFORMATEX wfx = { 0 };

    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1;
    wfx.nSamplesPerSec = nSamplesPerSecond_;
    wfx.wBitsPerSample = nBytesPerSample_ * 8;
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    bufdesc.dwSize = sizeof(bufdesc);
    bufdesc.dwFlags = DSBCAPS_STICKYFOCUS;
    bufdesc.dwBufferBytes = wfx.nBlockAlign * nSamplesPerFrame_ * nFrameBuffers_;
    bufdesc.lpwfxFormat = &wfx;

    res = createDxObject( &directSoundBuf_, directSound_->CreateSoundBuffer( &bufdesc, &directSoundBuf_, NULL) );

    if( res == DD_OK ) {
        LPVOID  lpBuf;
        DWORD   dwSize;

        if( SUCCEEDED(directSoundBuf_->Lock(0, bufdesc.dwBufferBytes, &lpBuf, &dwSize, 0, 0, 0)) ) {
            ZeroMemory( lpBuf, dwSize );

            directSoundBuf_->Unlock( lpBuf, dwSize, 0, 0 );
        }
    }

    return res;
}

void DxApp::terminateDirectSound()
{
    if( directSound_ != 0 ) {
        deleteSoundBuffer();
        directSound_->Release();
        directSound_ = 0;
    }
}

void DxApp::blitGameScreen()
{
    RECT rc;
    POINT p;

    p.x = 0; 
    p.y = 0;
    ClientToScreen( hWindow_, &p );
    GetClientRect( hWindow_, &rc );
    OffsetRect( &rc, p.x, p.y );
    ddPrimarySurface_->Blt( &rc, ddBackSurface_, NULL, 0, NULL); // No wait, or use DDBLT_WAIT
}

void DxApp::prepareGameScreen()
{
    HDC hDc;
    
    if( DD_OK == ddBackSurface_->GetDC(&hDc) ) {
        dibCanvas_->stretch( hDc, 0, 0, dibCanvas_->getWidth(), dibCanvas_->getHeight() );
        ddBackSurface_->ReleaseDC( hDc );
    }
}

void DxApp::setActiveFlags( DWORD flags )
{
    if( flags != dwActiveFlags_ ) {
        // Set flags
        dwActiveFlags_ = flags;

        if( directSoundBuf_ != 0 ) {
            if( isActive() ) {
                directSoundBuf_->Play( 0, 0, DSBPLAY_LOOPING );
            }
            else {
                directSoundBuf_->Stop();
            }
        }
    }
}

void DxApp::copySnapshotToClipboard()
{
    if( dibCanvas_ != 0 ) {
        if( OpenClipboard( hWindow_ ) ) {
            EmptyClipboard();
            SetClipboardData( CF_BITMAP, dibCanvas_->toBitmap() );
            CloseClipboard();
        }
    }
}

bool DxApp::handleWindowMessage( HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam, LRESULT * lResult )
{
    if( this == 0 )
        return false;

    bool result = true;

    *lResult = 0; // A good default for many messages!

    switch( wMsg ) {
        // No need to erase the background
        case WM_ERASEBKGND:
            *lResult = 1; // Tell the system we erased the background (a lie, of course!)
            break;
        // Paint the window by just blitting the game video
        case WM_PAINT:
            if( dibCanvas_ != 0 ) {
                RECT rc;
                PAINTSTRUCT stPS;
                HDC hDC = BeginPaint( hWnd, &stPS );
                GetClientRect( hWindow_, &rc );
                dibCanvas_->stretch( hDC, 0, 0, rc.right-rc.left, rc.bottom-rc.top );
                EndPaint( hWnd, &stPS );
            }
            break;
        // Application is activating/deactivating
        case WM_ACTIVATEAPP:
            setActiveFlags( wParam ? dwActiveFlags_ | AfAppActive : dwActiveFlags_ & ~AfAppActive );
            break;
        // Application is performing a size/move loop
        case WM_ENTERSIZEMOVE:
            setActiveFlags( dwActiveFlags_ | AfSizeLoop );
            break;
        case WM_EXITSIZEMOVE:
            setActiveFlags( dwActiveFlags_ & ~AfSizeLoop );
            break;
        // Application is performing a menu loop
        case WM_ENTERMENULOOP:
            setActiveFlags( dwActiveFlags_ | AfMenuLoop );
            break;
        case WM_EXITMENULOOP:
            setActiveFlags( dwActiveFlags_ & ~AfMenuLoop );
            break;
        // Destroy window
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;
#if 0
        // Disable mouse sizing
        case WM_NCHITTEST:
            *lResult = DefWindowProc( hWnd, wMsg, wParam, lParam );
            switch( *lResult ) {
            case HTBOTTOM:
            case HTBOTTOMLEFT:
            case HTBOTTOMRIGHT:
            case HTLEFT:
            case HTRIGHT:
            case HTTOP:
            case HTTOPLEFT:
            case HTTOPRIGHT:
                *lResult = HTBORDER; // Disable window sizing
            }
            break;
        // Track size info
        case WM_GETMINMAXINFO:
            {
                LPMINMAXINFO pMMI = (LPMINMAXINFO) lParam;
                RECT rc;

                SetRect( &rc, 0, 0, nClientWidth_, nClientHeight_ );
                AdjustWindowRect( &rc, WS_VISIBLE | WS_OVERLAPPEDWINDOW, TRUE );

                pMMI->ptMinTrackSize.x = rc.right - rc.left;
                pMMI->ptMinTrackSize.y = rc.top - rc.bottom;

                int maxWidth = GetSystemMetrics( SM_CXMAXIMIZED );
                int maxHeight = GetSystemMetrics( SM_CYMAXIMIZED );

                if( (int)(maxWidth * nClientHeight_ / nClientWidth_) > maxHeight ) {
                    pMMI->ptMaxSize.x = maxHeight * nClientWidth_ / nClientHeight_;
                    pMMI->ptMaxSize.y = maxHeight;
                }
                else {
                    pMMI->ptMaxSize.x = maxWidth;
                    pMMI->ptMaxSize.y = maxWidth * nClientHeight_ / nClientWidth_;
                }

                pMMI->ptMaxPosition.x = (maxWidth - pMMI->ptMaxSize.x) / 2;
                pMMI->ptMaxPosition.y = 0;

                pMMI->ptMaxTrackSize.x = pMMI->ptMaxSize.x;
                pMMI->ptMaxTrackSize.y = pMMI->ptMaxSize.y;
            }
            break;
#endif
        case WM_SIZING:
            {
                LPRECT lprc = (LPRECT) lParam;
                RECT rc;

                // Get size of decorations
                SetRect( &rc, 0, 0, nClientWidth_, nClientHeight_ );
                AdjustWindowRect( &rc, WS_VISIBLE | WS_OVERLAPPEDWINDOW, TRUE );
                rc.right -= nClientWidth_ + rc.left;
                rc.bottom -= nClientHeight_ + rc.top;

                if( wParam == WMSZ_BOTTOMRIGHT ) {
                    lprc->bottom = lprc->top + (lprc->right-lprc->left-rc.right) * nClientHeight_ / nClientWidth_ + rc.bottom;
                }

                *lResult = TRUE;
            }
            break;
        // Message not handled!
        default:
            result = false;
            break;
    }

    return result;
}

void DxApp::synchToSoundBuffer( unsigned index )
{
    // Sync to audio buffer: this makes sure the play cursor is not in the part
    // of the buffer that we are going to write, but I'm not particularly
    // happy of this solution because GetCurrentPosition() returns often
    // wrong information. Triple buffering seems to help a little...
    DWORD dwPlayCursor;
    DWORD dwWriteCursor;
    DWORD dwLoBound = index * nSampleBytesPerFrame_;
    DWORD dwHiBound = dwLoBound + nSampleBytesPerFrame_;

    do {
        directSoundBuf_->GetCurrentPosition( &dwPlayCursor, &dwWriteCursor );
    } while( dwPlayCursor >= dwLoBound && dwPlayCursor < dwHiBound );
}

void DxApp::writeToSoundBuffer( unsigned index, const int * buf, int voices )
{
    void * lpBuf;
    DWORD dwSize;

    if( DS_OK == directSoundBuf_->Lock( index*nSampleBytesPerFrame_,
        nSampleBytesPerFrame_, 
        &lpBuf, 
        &dwSize, 
        0, 0, 0 ) )
    {
        if( buf == 0 || voices <= 0 ) {
            memset( lpBuf, 0, dwSize );
        }
        else {
            // Write into the sound buffer
            BYTE * sndBuf = (BYTE *)lpBuf;

            int register mulFactor = 128 / voices;

            for( unsigned i=0; i<nSamplesPerFrame_; i++ ) {
                WORD w = (WORD)(*buf++ * mulFactor);
                *sndBuf++ = (BYTE) (w);
                *sndBuf++ = (BYTE) (w >> 8);
            }
        }

        directSoundBuf_->Unlock( lpBuf, dwSize, 0, 0 );
    }
}

void DxApp::setWindowSize( unsigned factor )
{
    RECT rc;
    WINDOWPLACEMENT wp;

    nSizeFactor_ = factor;

    // Restore window if needed
    wp.length = sizeof(wp);
    if( GetWindowPlacement( hWindow_, &wp ) ) {
        if( wp.showCmd != SW_SHOWNORMAL ) {
            ShowWindow( hWindow_, SW_SHOWNORMAL );
        }
    }

    // Resize window
    LONG width = factor*nClientWidth_;
    LONG height = factor*nClientHeight_;
    RECT workarea;
    RECT wndrect;

    SystemParametersInfo( SPI_GETWORKAREA, 0, &workarea, 0 );
    GetWindowRect( hWindow_, &wndrect );

    LONG x = wndrect.left;
    LONG y = wndrect.top;

    if( factor == 0 ) {
        // Compute max window dimension for current work area
        SetRect( &rc, 0, 0, 0, 0 );
        AdjustWindowRect( &rc, WS_VISIBLE | WS_OVERLAPPEDWINDOW, TRUE );

        // Now we have the size of decorations with respect to the client area
        LONG decow = rc.right-rc.left;
        LONG decoh = rc.bottom-rc.top;

        // Assume max width and see what happens
        width = workarea.right - workarea.left - decow; // Max width of client area
        height = width * nClientHeight_ / nClientWidth_ + decoh;

        if( height <= (workarea.bottom-workarea.top) ) {
            // Width can be maximized: keep it!
            width += decow;
        }
        else {
            // Must maximize height and adjust width accordingly
            height = workarea.bottom - workarea.top;
            width = (height - decoh) * nClientWidth_ / nClientHeight_ + decow;
        }

        x = workarea.left + (workarea.right - workarea.left - width) / 2;
        y = workarea.top + (workarea.bottom - workarea.top - height) / 2;
    }
    else {
        // Try to keep the window visible within the work area
        SetRect( &rc, 0, 0, width, height );
        AdjustWindowRect( &rc, WS_VISIBLE | WS_OVERLAPPEDWINDOW, TRUE );

        width = rc.right-rc.left;
        height = rc.bottom-rc.top;

        if( (x + width) > workarea.right ) {
            x = workarea.right - width;
        }

        if( (y + height) > workarea.bottom ) {
            y = workarea.bottom - height;
            if( y < 0 ) {
                y = 0;
            }
        }
    }

    SetWindowPos( hWindow_, HWND_TOP, x, y, width, height, SWP_NOCOPYBITS );
}
