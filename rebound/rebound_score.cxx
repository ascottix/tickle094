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
#include "rebound_score.h"

ReboundScore::ReboundScore( ReboundBoard * board )
{
    // Score window
    c8.connectLine( 11, board->s_64V );
    c8.connectLine( 12, board->s_64V );

    f7.connectLine( 12, board->s_32V );
    f7.connectLine( 13, f9,  6 );

    f9.connectLine(  5, board->s_128V );

    j5.connectLine(  9, board->s_NOT_256H );
    j5.connectLine( 10, board->s_64H );
    j5.connectLine( 12, board->s_128H );
    j5.connectLine( 13, board->s_256H );

    j6.connectLine(  4, j5, 11 );
    j6.connectLine(  5, j5,  8 );
    j6.connectLine(  1, f7, 11 );
    j6.connectLine(  2, c8, 13 );

    k6.connectLine(  5, j6,  3 );
    k6.connectLine(  6, j6,  6 );
    // K6, 4    -> to K7 BI/RBO line

    // Score counter
    h9.connectLine(  1, board->s_POWER );
    h9.connectLine(  4, board->s_POWER );
    h9.connectLine(  8, board->s_POWER );
    h9.connectLine(  9, k9, 11 );
    h9.connectLine( 10, board->s_NOT_START );
    h9.connectLine( 11, board->s_POWER );
    h9.connectLine( 12, j9, 11 );
    h9.connectLine( 13, board->s_NOT_START );

    j8.connectLine(  1, board->s_GROUND );
    j8.connectLine(  2, board->s_64H );
    j8.connectLine(  3, j9,  8 );
    j8.connectLine(  4, h9,  2 );
    j8.connectLine(  5, k9,  8 );
    j8.connectLine(  6, h9,  6 );
    j8.connectLine( 10, h9,  6 );
    j8.connectLine( 11, k9, 11 );
    j8.connectLine( 12, h9,  2 );
    j8.connectLine( 13, j9, 11 );
    j8.connectLine( 14, board->s_32H );
    j8.connectLine( 15, board->s_GROUND );

    j9.connectLine(  1, j9, 12 );
    j9.connectLine(  2, board->s_START );
    j9.connectLine(  3, board->s_START );
    j9.connectLine(  6, board->s_GROUND );
    j9.connectLine(  7, board->s_GROUND );

    k8.connectLine(  1, board->s_GROUND );
    k8.connectLine(  2, board->s_64H );
    k8.connectLine(  3, j9, 12 );
    k8.connectLine(  4, board->s_POWER );
    k8.connectLine(  5, k9, 12 );
    k8.connectLine(  6, board->s_POWER );
    k8.connectLine( 10, h9,  6 );
    k8.connectLine( 11, k9,  9 );
    k8.connectLine( 12, h9,  2 );
    k8.connectLine( 13, j9,  9 );
    k8.connectLine( 14, board->s_32H );
    k8.connectLine( 15, board->s_GROUND );

    k9.connectLine(  1, k9, 12 );
    k9.connectLine(  2, board->s_START );
    k9.connectLine(  3, board->s_START );
    k9.connectLine(  6, board->s_GROUND );
    k9.connectLine(  7, board->s_GROUND );

    k7.connectLine(  1, k8,  9 );
    k7.connectLine(  2, j8,  7 );
    k7.connectLine(  4, k6,  4 );   // From the score window circuit
    k7.connectLine(  6, j8,  9 );
    k7.connectLine(  7, k8,  7 );

    // Generation of SCORE signal
    f6.connectLine(  1, board->s_8V );
    f6.connectLine(  2, board->s_4V );
    f6.connectLine( 13, board->s_16H );
    // F6,12    -> to F9,9 (from 1, 2, 13)

    f8.connectLine(  9, h6, 12 );
    f8.connectLine( 10, k7, 13 ); // Segment a
    f8.connectLine( 11, f9,  4 );

    f9.connectLine(  3, board->s_16V );
    f9.connectLine(  9, f6, 12 );
    // F9, 4    -> NOT 16V
    // F9, 8    -> NOT F6,12

    h6.connectLine(  1, board->s_8V );
    h6.connectLine(  2, k6,  1 );
    h6.connectLine(  3, k6,  1 );
    h6.connectLine(  4, board->s_4H );
    h6.connectLine(  5, board->s_8H );
    h6.connectLine( 13, board->s_4V );

    h7.connectLine(  1, f9,  4 );
    h7.connectLine(  2, k7, 14 ); // Segment g
    h7.connectLine(  3, f9,  4 );
    h7.connectLine(  4, k6, 13 );
    h7.connectLine(  5, k7, 12 ); // Segment b
    h7.connectLine(  9, k7, 10 ); // Segment d
    h7.connectLine( 10, board->s_16V );
    h7.connectLine( 11, f9,  8 );
    h7.connectLine( 13, f9,  8 );

    h8.connectLine(  1, board->s_NOT_NET );
    h8.connectLine(  2, h7,  8 );
    h8.connectLine(  3, h7,  6 );
    h8.connectLine(  4, j7, 12 );
    h8.connectLine(  5, j7,  6 );
    h8.connectLine(  6, j7,  8 );
    h8.connectLine( 11, h7, 12 );
    h8.connectLine( 12, f8,  8 );
    // H8, 8    -> SCORE

    j6.connectLine( 12, board->s_8H );
    j6.connectLine( 13, board->s_4H );
    // J6,11    -> to K6,11 (from 12, 13)

    j7.connectLine(  1, f9,  4 );
    j7.connectLine(  2, k7, 15 ); // Segment f
    j7.connectLine(  3, k6, 13 );
    j7.connectLine(  4, board->s_16V );
    j7.connectLine(  5, k7, 11 ); // Segment c
    j7.connectLine(  9, k7,  9 ); // Segment e
    j7.connectLine( 10, h6,  6 );
    j7.connectLine( 11, board->s_16V );
    j7.connectLine( 13, h6,  6 );

    k6.connectLine(  2, board->s_16H );
    k6.connectLine(  3, board->s_GROUND );
    k6.connectLine( 11, j6, 11 );
    k6.connectLine( 12, k6,  1 );
    // K6, 1    -> to H6,2 (from 2, 3)
    // K6,13

    // Board interface
    board->s_STOP.connectTo( j6, 8 );
    board->s_SCORE.connectTo( h8, 8 );
    board->s_SCORE_LSB1.connectTo( k9,  1 );
    board->s_SCORE_LSB2.connectTo( j9,  1 );
}

void ReboundScore::update()
{
    // Score window
    c8.update();
    j5.update();
    f9.update();
    f7.update();
    j6.update();
    k6.update();

    // Score counter
    k9.update();
    j9.update();
    h9.update();
    k8.update();
    j8.update();

    k7.update();

    // Generation of SCORE signal
    j6.update();
    f6.update();
    k6.update();
    k6.update();    // Self-feedback from 1 to 12
    h6.update();
    f9.update();
    j7.update();
    h7.update();
    f8.update();
    h8.update();
}
