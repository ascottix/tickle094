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
#include "rebound_h_sync.h"

ReboundHorizontalSync::ReboundHorizontalSync( ReboundBoard * board )
{
    h5.connectLine(  1, h5, 12 );
    h5.connectLine(  2, j4,  8 );
    h5.connectLine(  3, j4,  8 );
    h5.connectLine( 14, board->s_CLOCK );
    // H5,12    -> 1H
    // H5, 9    -> 2H
    // H5, 8    -> 4H
    // H5,11    -> 8H

    f5.connectLine(  1, f5, 12 );
    f5.connectLine(  2, j4,  8 );
    f5.connectLine(  3, j4,  8 );
    f5.connectLine( 14, h5, 11 );
    // F5,12    ->  16H
    // F5, 9    ->  32H
    // F5, 8    ->  64H
    // F5,11    -> 128H

    h4.connectLine(  1, board->s_POWER );
    h4.connectLine(  4, board->s_POWER );
    h4.connectLine( 12, f5, 11 );
    h4.connectLine( 13, j4,  9 );
    // H4, 3    -> 256H
    // H4, 2    -> NOT 256H

    f4.connectLine(  1, f5,  8 );
    f4.connectLine(  2, f5, 11 );
    f4.connectLine(  4, h5,  8 );
    f4.connectLine(  5, h4,  3 );
    // F4, 6
    
    j4.connectLine( 10, board->s_POWER );
    j4.connectLine( 11, board->s_NOT_CLOCK );
    j4.connectLine( 12, f4,  6 );
    j4.connectLine( 13, board->s_POWER );
    // J4, 8    -> HRESET
    // J4, 9    -> NOT HRESET

    k4.connectLine(  1, f5,  8 ); // 64H
    k4.connectLine(  2, f5, 12 ); // 16H
    k4.connectLine( 12, b5,  4 ); // NOT 64H
    k4.connectLine( 13, f5,  9 ); // 32H
    // K4, 3    -> NAND(  1,  2 )
    // K4,11    -> NAND( 12, 13 )

    k5.connectLine(  1, k4,  3 );
    k5.connectLine(  2, board->s_GROUND );
    k5.connectLine(  3, board->s_GROUND );
    k5.connectLine(  4, j4,  9 );
    k5.connectLine( 10, board->s_POWER );
    k5.connectLine( 11, f5, 12 ); // 16H
    k5.connectLine( 12, k4, 11 );
    k5.connectLine( 13, f7,  3 );
    // K5, 5    -> HBLANK
    // K5, 6    -> NOT HBLANK
    // K5, 8    -> NOT HSYNC
    // K5, 9    -> HSYNC

    b5.connectLine(  3, f5,  8 ); // 64H
    // B5, 4    -> NOT 64H

    f7.connectLine(  1, b5,  4 );
    f7.connectLine(  2, k5,  5 );
    // F7, 3

    // Board interface
    board->s_1H.connectTo( h5, 12 );
    board->s_2H.connectTo( h5, 9 );
    board->s_4H.connectTo( h5, 8 );
    board->s_8H.connectTo( h5, 11 );
    board->s_16H.connectTo( f5, 12 );
    board->s_32H.connectTo( f5, 9 );
    board->s_64H.connectTo( f5, 8 );
    board->s_128H.connectTo( f5, 11 );
    board->s_256H.connectTo( h4, 3 );
    board->s_NOT_256H.connectTo( h4, 2 );
    board->s_HBLANK.connectTo( k5, 5 );
    board->s_NOT_HBLANK.connectTo( k5, 6 );
    board->s_HSYNC.connectTo( k5, 9 );
    board->s_NOT_HSYNC.connectTo( k5, 8 );
    board->s_HRESET.connectTo( j4, 8 );
    board->s_NOT_HRESET.connectTo( j4, 9 );
}

void ReboundHorizontalSync::update()
{
    j4.update(); // Same clock input as H5
    h5.update();
    f5.update();
    h4.update();
    b5.update();
    f4.update();
    j4.update(); // Update again for non-edge effects
    k4.update();
    k5.update();
    f7.update();
    k5.update(); // Update again because of indirect feedback thru F7
}
