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
#include "rebound_video_sync_fast.h"

ReboundVideoSyncFast::ReboundVideoSyncFast( ReboundBoard * board )
{
    last_clock = 0;
    curr_clock.connectTo( board->s_CLOCK );

    h_count = 0;
    v_count = 0;

    /*
    b_1H = 0;
    b_2H = 0;
    b_4H = 0;
    b_8H = 0;
    b_16H = 0;
    b_32H = 0;
    b_64H = 0;
    b_128H = 0;
    b_256H = 0;
    b_NOT_256H = 0;
    b_HRESET = 0;
    b_NOT_HRESET = 0;
    b_HSYNC = 0;
    b_NOT_HSYNC = 0;
    b_HBLANK = 0;
    b_NOT_HBLANK = 0;

    b_1V = 0;
    b_2V = 0;
    b_4V = 0;
    b_8V = 0;
    b_16V = 0;
    b_32V = 0;
    b_64V = 0;
    b_128V = 0;
    b_256V = 0;
    b_NOT_256V = 0;
    b_VRESET = 0;
    b_NOT_VRESET = 0;
    b_NOT_VSYNC = 0;
    b_NOT_COMPOSITE_SYNC = 0;
    */

    board->s_1H.assign( b_1H );
    board->s_2H.assign( b_2H );
    board->s_4H.assign( b_4H );
    board->s_8H.assign( b_8H );
    board->s_16H.assign( b_16H );
    board->s_32H.assign( b_32H );
    board->s_64H.assign( b_64H );
    board->s_128H.assign( b_128H );
    board->s_256H.assign( b_256H );
    board->s_NOT_256H.assign( b_NOT_256H );
    board->s_HRESET.assign( b_HRESET );
    board->s_NOT_HRESET.assign( b_NOT_HRESET );
    board->s_HSYNC.assign( b_HSYNC );
    board->s_NOT_HSYNC.assign( b_NOT_HSYNC );
    board->s_HBLANK.assign( b_HBLANK );
    board->s_NOT_HBLANK.assign( b_NOT_HBLANK );

    board->s_1V.assign( b_1V );
    board->s_2V.assign( b_2V );
    board->s_4V.assign( b_4V );
    board->s_8V.assign( b_8V );
    board->s_16V.assign( b_16V );
    board->s_32V.assign( b_32V );
    board->s_64V.assign( b_64V );
    board->s_128V.assign( b_128V );
    board->s_256V.assign( b_256V );
    board->s_NOT_256V.assign( b_NOT_256V );
    board->s_NOT_VSYNC.assign( b_NOT_VSYNC );
    board->s_VRESET.assign( b_VRESET );
    board->s_NOT_VRESET.assign( b_NOT_VRESET );
}

void ReboundVideoSyncFast::update()
{
    if( last_clock && ! curr_clock.state() ) {
        h_count++;

        if( h_count > 452 ) {
            h_count = 0;
        }

        if( h_count == 1 ) {
            v_count++;

            if( v_count > 261 ) {
                v_count = 0;
            }

            b_1V = (v_count & 1);
            b_2V = (v_count & 2) >> 1;
            b_4V = (v_count & 4) >> 2;
            b_8V = (v_count & 8) >> 3;
            b_16V = (v_count & 16) >> 4;
            b_32V = (v_count & 32) >> 5;
            b_64V = (v_count & 64) >> 6;
            b_128V = (v_count & 128) >> 7;
            b_256V = (v_count & 256) >> 8;
            b_NOT_256V = 1 ^ b_256V;

            b_NOT_VSYNC = v_count < 14 ? 0 : 1;
            b_VRESET = v_count == 0 ? 1 : 0;
            b_NOT_VRESET = 1 ^ b_VRESET;
        }

        b_1H = (h_count & 1);
        b_2H = (h_count & 2) >> 1;
        b_4H = (h_count & 4) >> 2;
        b_8H = (h_count & 8) >> 3;
        b_16H = (h_count & 16) >> 4;
        b_32H = (h_count & 32) >> 5;
        b_64H = (h_count & 64) >> 6;
        b_128H = (h_count & 128) >> 7;
        b_256H = (h_count & 256) >> 8;
        b_NOT_256H = 1 ^ b_256H;

        b_HRESET = h_count == 0 ? 1 : 0;
        b_NOT_HRESET = 1 ^ b_HRESET;
        b_HSYNC = (h_count >= 16) && (h_count < 48) ? 1 : 0;
        b_NOT_HSYNC = 1 ^ b_HSYNC;
        b_HBLANK = h_count < 80 ? 1 : 0;
        b_NOT_HBLANK = 1 ^ b_HBLANK;
    }

    last_clock = curr_clock.state();
}
