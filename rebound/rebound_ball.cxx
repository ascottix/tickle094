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
#include "rebound_ball.h"

ReboundBall::ReboundBall( ReboundBoard * board )
{
    // Ball direction and speed
    b4.connectLine(  1, d5,  3 ); // NOT HIT
    b4.connectLine(  2, board->s_64V );
    b4.connectLine(  3, board->s_POWER );
    b4.connectLine(  4, board->s_POWER );
    b4.connectLine(  5, board->s_GROUND );
    b4.connectLine(  6, board->s_GROUND );
    b4.connectLine(  7, board->s_POWER );
    b4.connectLine(  9, b5,  8 );
    b4.connectLine( 10, board->s_POWER );

    b5.connectLine(  5, c5,  8 );
    b5.connectLine(  9, b4, 15 );

    c5.connectLine(  1, d6,  9 ); // CP
    c5.connectLine(  2, c6,  5 ); // DP
    c5.connectLine(  4, board->s_PADDLE_COUNT_1 ); // B6, 4 from paddle circuit
    c5.connectLine(  5, board->s_NOT_256H );
    c5.connectLine(  9, board->s_SCORE_LSB1 );
    c5.connectLine( 10, board->s_SCORE_LSB2 );
    c5.connectLine( 12, c6,  5 ); // DP
    c5.connectLine( 13, c6,  9 ); // BP

    c6.connectLine(  1, d5, 11 );
    c6.connectLine(  2, board->s_PADDLE_COUNT_1 ); // B6, 4 from paddle circuit
    c6.connectLine(  3, e6, 10 ); // HIT
    c6.connectLine(  4, d5,  8 );
    c6.connectLine( 10, d5,  8 );
    c6.connectLine( 11, e6, 10 ); // HIT
    c6.connectLine( 12, board->s_PADDLE_COUNT_4 ); // B6,12 from paddle circuit
    c6.connectLine( 13, d5, 11 );
    // C6, 5    -> DP
    // C6, 8    -> NOT BP
    // C6, 9    -> BP

    d5.connectLine(  1, e6,  4 ); // BALL
    d5.connectLine(  2, board->s_DISP_PAD_COMP );
    d5.connectLine(  9, c5,  8 );
    d5.connectLine( 10, board->s_SERVE );
    d5.connectLine( 12, board->s_SERVE );
    d5.connectLine( 13, b5,  6 );

    d6.connectLine(  1, board->s_NOT_SERVE_TIME );
    d6.connectLine(  2, c5,  6 );
    d6.connectLine(  3, e6, 10 ); // HIT
    d6.connectLine(  4, board->s_POWER );
    d6.connectLine( 10, d5, 11 );
    d6.connectLine( 11, e6, 10 ); // HIT
    d6.connectLine( 12, board->s_PADDLE_COUNT_2 ); // B6, 7 from paddle circuit
    d6.connectLine( 13, d5,  8 );
    // D6, 5    -> NOT BACKWARDS
    // D6, 8    -> NOT CP
    // D6, 9    -> CP

    e4.connectLine(  1, e5,  6 );
    e4.connectLine(  4, b4, 15 );
    e4.connectLine(  5, board->s_POWER );
    e4.connectLine(  9, board->s_POWER );
    e4.connectLine( 10, board->s_NOT_SERVE );
    e4.connectLine( 11, e5, 11 );
    e4.connectLine( 14, board->s_GROUND );
    e4.connectLine( 15, c5, 11 );
    // E4, 3    -> Av
    // E4, 2    -> Bv
    // E4, 6    -> Cv
    // E4, 7    -> Dv

    e5.connectLine(  4, c5,  3 );
    e5.connectLine(  5, board->s_NOT_SERVE );
    e5.connectLine( 12, board->s_NOT_SERVE_TIME );
    e5.connectLine( 13, e6, 13 );

    e6.connectLine(  8, d5,  3 ); // NOT HIT
    e6.connectLine(  9, board->s_1H );
    e6.connectLine( 11, e6, 10 );
    e6.connectLine( 12, board->s_GROUND );
    // E6,10    -> HIT

    // Vertical ball motion
    c4.connectLine(  1, board->s_NOT_SERVE ); // MR
    c4.connectLine(  2, board->s_NOT_HSYNC );
    c4.connectLine(  3, board->s_GROUND );
    c4.connectLine(  4, board->s_GROUND );
    c4.connectLine(  5, board->s_GROUND );
    c4.connectLine(  6, board->s_GROUND );
    c4.connectLine(  7, d4, 15 );
    c4.connectLine(  9, d5,  6 );
    c4.connectLine( 10, board->s_POWER );
    // C4,15

    d4.connectLine(  1, board->s_NOT_SERVE ); // MR
    d4.connectLine(  2, board->s_NOT_HSYNC );
    d4.connectLine(  3, e4,  3 ); // Av
    d4.connectLine(  4, e4,  2 ); // Bv
    d4.connectLine(  5, e4,  6 ); // Cv
    d4.connectLine(  6, e4,  7 ); // Dv
    d4.connectLine(  7, board->s_POWER );
    d4.connectLine(  9, d5,  6 );
    d4.connectLine( 10, board->s_NOT_VSYNC );
    // D4,11
    // D4,12
    // D4,15

    d5.connectLine(  4, c4, 15 );
    d5.connectLine(  5, d4, 15 );

    c3.connectLine(  1, d4, 12 );
    c3.connectLine(  2, d4, 11 );
    c3.connectLine( 13, c4, 15 );
    // C3,12    -> V VIDEO (TP 4)

    // Horizontal ball motion
    f1.connectLine(  1, d6,  5 ); // NOT BACKWARDS
    f1.connectLine(  2, f1,  6 ); // Self-feedback
    f1.connectLine(  3, board->s_VRESET );
    f1.connectLine(  4, board->s_POWER );

    f2.connectLine(  1, c6,  8 ); // NOT BP
    f2.connectLine(  2, f2,  6 );
    f2.connectLine(  4, board->s_VRESET );
    f2.connectLine(  5, f1,  6 );
    f2.connectLine( 12, f2,  6 );
    f2.connectLine( 13, d6,  8 ); // NOT CP

    h1.connectLine(  9, c6,  5 ); // DP
    h1.connectLine( 10, f2,  6 );

    e1.connectLine( 12, h1,  8 );
    e1.connectLine( 13, h1,  8 );

    e2.connectLine(  1, board->s_GROUND );
    e2.connectLine(  3, h1,  8 );
    e2.connectLine(  4, board->s_POWER );
    e2.connectLine(  7, board->s_POWER );
    e2.connectLine(  8, f2, 11 );
    e2.connectLine( 10, f2,  3 );
    e2.connectLine( 11, e1, 11 );
    // TODO: hack
    e2.connectLine( 11, board->s_POWER );
    e2.connectLine( 13, board->s_GROUND );
    e2.connectLine( 16, board->s_GROUND );

    e3.connectLine(  1, board->s_NOT_SERVE );
    e3.connectLine(  2, board->s_CLOCK );
    e3.connectLine(  3, e2,  9 );
    e3.connectLine(  4, e2,  6 );
    e3.connectLine(  5, e2,  2 );
    e3.connectLine(  6, e2, 15 );
    e3.connectLine(  7, board->s_POWER );
    e3.connectLine(  9, c3,  6 ); // Indirect feedback thru C3
    e3.connectLine( 10, board->s_POWER );

    d3.connectLine(  1, board->s_NOT_SERVE );
    d3.connectLine(  2, board->s_CLOCK );
    d3.connectLine(  3, board->s_POWER );
    d3.connectLine(  4, board->s_POWER );
    d3.connectLine(  5, board->s_GROUND );
    d3.connectLine(  6, board->s_GROUND );
    d3.connectLine(  7, e3, 15 );
    d3.connectLine(  9, c3,  6 );
    d3.connectLine( 10, board->s_POWER );

    f3.connectLine(  1, board->s_POWER );
    f3.connectLine(  4, board->s_POWER );
    f3.connectLine( 12, d3, 15 );
    f3.connectLine( 13, board->s_NOT_SERVE );

    c3.connectLine(  3, d3, 15 );
    c3.connectLine(  4, f3,  3 );
    c3.connectLine(  5, e3, 15 );
    // C3, 6    -> NOT BALL LOAD

    f4.connectLine(  9, f3,  3 );
    f4.connectLine( 10, d3, 15 );
    f4.connectLine( 12, e3, 11 );
    f4.connectLine( 13, e3, 12 );
    // F4, 8    -> NOT H VIDEO (TP 5)

    // Ball summing
    e6.connectLine(  2, e6,  4 );
    e6.connectLine(  3, e6,  4 );
    e6.connectLine(  5, f4,  8 );
    e6.connectLine(  6, c3, 12 );
    // E6, 4    -> BALL
    // E6, 1    -> NOT BALL

    // Board connections
    board->s_NOT_BACKWARDS.connectTo( d6, 5 );
    board->s_HIT.connectTo( e6, 10 );
    board->s_NOT_HIT.connectTo( d5, 3 );
    board->s_BALL.connectTo( e6, 4 );
    board->s_NOT_BALL.connectTo( e6, 1 );
}

void ReboundBall::update()
{
    int i;

    // Ball direction and speed
    c5.update();

    for( i=0; i<2; i++ ) {
        b4.update();
        b5.update();
        d5.update();
    }

    e6.update();
    e6.update(); // Self-feedback from 10 to 11

    c6.update();
    d6.update();

    c5.update();
    e5.update();
    e4.update();

    // Vertical ball motion
    for( i=0; i<1; i++ ) {
        // Note: C4 depends on D4 yet it is clocked by the same
        // source so it is *extremely* important that it is updated
        // first, otherwise it will be influenced by the output
        // of D4 even on the same clock pulse, which is wrong
        c4.update();
        d4.update();
        d5.update();
    }

    c3.update(); // Note: also updated in the horizontal ball motion circuit

    // Horizontal ball motion
    f1.update();
    f1.update(); // Self-feedback

    f2.update();
    f2.update(); // Self-feedback

    h1.update();

    e1.update();
    e2.update();

    for( i=0; i<2; i++ ) {
        d3.update();
        e3.update();
        f3.update();
        c3.update();
    }

    f4.update();

    // Ball summing
    e6.update();
    e6.update();
}
