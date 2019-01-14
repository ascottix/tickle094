/*
    Rebound emulator

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
#ifndef REBOUND_VIDEO_FAST_H_
#define REBOUND_VIDEO_FAST_H_

#include "rebound.h"

class ReboundVideoSyncFast
{
public:
    ReboundVideoSyncFast( ReboundBoard * board );

    void update();

//private:
    DBit        last_clock;
    DLine       curr_clock;

    unsigned    h_count;
    unsigned    v_count;

    // Horizontal synchronization circuit
    DBit    b_1H;
    DBit    b_2H;
    DBit    b_4H;
    DBit    b_8H;
    DBit    b_16H;
    DBit    b_32H;
    DBit    b_64H;
    DBit    b_128H;
    DBit    b_256H;
    DBit    b_NOT_256H;
    DBit    b_HRESET;
    DBit    b_NOT_HRESET;
    DBit    b_HSYNC;
    DBit    b_NOT_HSYNC;
    DBit    b_HBLANK;
    DBit    b_NOT_HBLANK;

    // Vertical synchronization circuit
    DBit    b_1V;
    DBit    b_2V;
    DBit    b_4V;
    DBit    b_8V;
    DBit    b_16V;
    DBit    b_32V;
    DBit    b_64V;
    DBit    b_128V;
    DBit    b_256V;
    DBit    b_NOT_256V;
    DBit    b_VRESET;
    DBit    b_NOT_VRESET;
    DBit    b_NOT_VSYNC;
    DBit    b_NOT_COMPOSITE_SYNC;
};

#endif // REBOUND_VIDEO_FAST_H_
