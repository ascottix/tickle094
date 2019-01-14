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
#ifndef WIN_FRAME_H_
#define WIN_FRAME_H_

#include "dib32.h"

#include <emu/emu_frame.h>
#include <emu/emu_math.h>
#include <emu/emu_mixer.h>

class DxApp;

class WinFrame : public TFrame
{
public:
    WinFrame( Dib32 * screen, DxApp * app ) : screen_(screen), dx_app_(app) {
        max_voices_ = 0;
    }

    virtual ~WinFrame() {
    }

    virtual void setVideo( TBitmap * video, bool flipped ) {
        screen_->assign( video, flipped );
    }

    virtual TMixer * getMixer() {
        return &mixer_;
    }

    Dib32 * screen() {
        return screen_;
    }

    int maxVoices() {
        max_voices_ = TMath::max( mixer_.maxVoicesPerChannel(), max_voices_ );

        return max_voices_;
    }

    void playForceFeedbackEffect( int player, TForceFeedbackEffect fx, unsigned param );

    void stopForceFeedbackEffect( int player );

private:
    Dib32 *     screen_;
    DxApp *     dx_app_;
    TMixerMono  mixer_;
    int         max_voices_;
};

#endif // WIN_FRAME_H_
