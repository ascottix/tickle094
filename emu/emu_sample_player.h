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
#ifndef EMU_SAMPLE_PLAYER_H_
#define EMU_SAMPLE_PLAYER_H_

#include "emu_mixer.h"
#include "emu_sample.h"

enum {
    pmLooping = 1
};

class TSamplePlayer
{
public:
    TSamplePlayer();

    /** Destructor. */
    virtual ~TSamplePlayer();

    void setSample( TSample * sample );

    void play( unsigned mode = 0 );

    void stop();
    
    void restart() {
        offset_ = 0;
    }

    bool mix( int * buf, unsigned len, unsigned samplingRate );

    bool mix( TMixer * mixer, unsigned channel, unsigned len, unsigned samplingRate );

    TSample * sample() {
        return sample_;
    }

private:
    unsigned status_;
    unsigned offset_;
    TSample * sample_;
};

#endif // EMU_SAMPLE_PLAYER_H_
