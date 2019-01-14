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
#include "rebound_v_sync.h"

ReboundVerticalSync::ReboundVerticalSync( ReboundBoard * board )
{
    j3.connectLine(  1, j3, 12 );
    j3.connectLine(  2, j2,  8 );
    j3.connectLine(  3, j2,  8 );
    j3.connectLine( 14, board->s_HRESET );
    // J3,12    -> 1V
    // J3, 9    -> 2V
    // J3, 8    -> 4V
    // J3,11    -> 8V

    h3.connectLine(  1, h3, 12 );
    h3.connectLine(  2, j2,  8 );
    h3.connectLine(  3, j2,  8 );
    h3.connectLine( 14, j3, 11 );
    // H3,12    ->  16V
    // H3, 9    ->  32V
    // H3, 8    ->  64V
    // H3,11    -> 128V

    h4.connectLine(  8, board->s_POWER );
    h4.connectLine( 11, board->s_POWER );
    h4.connectLine(  9, h3, 11 ); // 128V
    h4.connectLine( 10, j2,  9 );
    // H4, 5    -> 256V
    // H4, 6    -> NOT 256V

    h2.connectLine(  1, h4,  5 ); // 256V
    h2.connectLine(  2, j3,  8 ); // 4V
    h2.connectLine(  3, j3, 11 ); // 8V
    h2.connectLine(  4, j3,  9 ); // 2V
    h2.connectLine(  5, j3,  8 ); // 4V
    h2.connectLine( 13, j3, 12 ); // 1V
    // H2, 6    -> to H1,13 (from input 3, 4, 5)
    // H2,12    -> 60.08 Hz (from input 1, 2, 13)

    j2.connectLine( 10, board->s_POWER );
    j2.connectLine( 11, board->s_HRESET );
    j2.connectLine( 12, h2, 12 );
    j2.connectLine( 13, board->s_POWER );
    // J2, 8    -> VRESET
    // J2, 9    -> NOT VRESET

    h1.connectLine(  1, j2,  9 );
    h1.connectLine(  2, h1, 11 );
    h1.connectLine( 12, h1,  3 );
    h1.connectLine( 13, h2,  6 );
    // H1,11    -> NOT VSYNC
 
    j5.connectLine(  5, board->s_HSYNC );
    j5.connectLine(  4, h1, 11 );
    // J5, 6    -> NOT COMPOSITE SYNC

    // Board interface
    board->s_1V.connectTo( j3, 12 );
    board->s_2V.connectTo( j3, 9 );
    board->s_4V.connectTo( j3, 8 );
    board->s_8V.connectTo( j3, 11 );
    board->s_16V.connectTo( h3, 12 );
    board->s_32V.connectTo( h3, 9 );
    board->s_64V.connectTo( h3, 8 );
    board->s_128V.connectTo( h3, 11 );
    board->s_256V.connectTo( h4, 5 );
    board->s_NOT_256V.connectTo( h4, 6 );
    board->s_VRESET.connectTo( j2, 8 );
    board->s_NOT_VRESET.connectTo( j2, 9 );
    board->s_NOT_VSYNC.connectTo( h1, 11 );
    board->s_NOT_COMPOSITE_SYNC.connectTo( j5, 6 );
}

void ReboundVerticalSync::update()
{
    j2.update(); // Same clock input as J3
    j3.update();
    h3.update();
    h4.update();
    h2.update();
//    j2.update(); // Update again for non-edge effects
    h1.update();
    h1.update(); // Allow feedback to kick in
    h1.update(); // Allow feedback to kick in
    j5.update();
}
