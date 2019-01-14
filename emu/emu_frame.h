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
#ifndef EMU_FRAME_H_
#define EMU_FRAME_H_

#include "emu_bitmap.h"
#include "emu_mixer.h"

enum TForceFeedbackEffect {
    fxFF_Explosion,
    fxFF_Bump
};

class TFrame
{
    unsigned flags_; // Flags are mainly used for debugging
    void * userData_;
    
public:
    TFrame() {
        flags_ = 0;
        userData_ = 0;
    }

    /** Destructor. */
    virtual ~TFrame() {
    }
    
    void setFlags( unsigned flags ) {
        flags_ = flags;
    }
    
    unsigned getFlags() const {
        return flags_;
    }
    
    void setUserData( void * data ) {
        userData_ = data;
    }
    
    void * getUserData() const {
        return userData_;
    }

    virtual void setVideo( TBitmap * screen, bool flipped = false ) = 0;

    virtual TMixer * getMixer() = 0;

    virtual void playForceFeedbackEffect( int player, TForceFeedbackEffect fx, unsigned param );

    virtual void stopForceFeedbackEffect( int player );

    virtual void setPlayerLight( int player, bool on );

    virtual void incrementCoinCounter( int counter );
};

#endif // EMU_FRAME_H_
