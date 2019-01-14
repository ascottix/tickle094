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
#ifndef MAC_FRAME_H_
#define MAC_FRAME_H_

#include <Carbon/Carbon.h>

#include <emu/emu_frame.h>
#include <emu/emu_math.h>
#include <emu/emu_mixer.h>

class MacFrame : public TFrame
{
public:
    MacFrame( int width, int height );
    
    virtual ~MacFrame();
    
    virtual void setVideo( TBitmap * bitmap, bool flipped = false );
    
    virtual TMixer * getMixer() {
        return &mixer_;
    }
    
    int maxVoices() {
        max_voices_ = TMath::max( mixer_.maxVoicesPerChannel(), max_voices_ );

        return max_voices_;
    }

    int width_;
    int height_;
    size_t data_offset_; // Used by data provider callback
    size_t data_count_;
    unsigned char * data_;
    CGDataProviderRef provider_;
    unsigned char * screen_;
    TMixerMono mixer_;
    int max_voices_;
};

#endif // MAC_FRAME_H_
