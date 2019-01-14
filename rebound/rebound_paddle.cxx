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
#include "rebound_paddle.h"

ReboundPaddle::ReboundPaddle( ReboundBoard * board )
{
    // Paddle control (potentiometer-controlled timers)
    c9.setClock( board->s_CLOCK, board->clock_frequency );
    c9.setResCap( 100, 0.00000022 ); // 100 ohm, 220 pF 
    c9.connectLine( 2, board->s_NOT_HRESET );
    c9.connectLine( 4, f7, 8 ); // NOT STOP 1

    b9.setClock( board->s_CLOCK, board->clock_frequency );
    b9.setResCap( 100, 0.00000022 ); // 100 ohm, 220 pF 
    b9.connectLine( 2, board->s_NOT_HRESET );
    b9.connectLine( 4, f7, 6 ); // NOT STOP 2

    // Stop 
    f6.connectLine(  3, board->s_256H );
    f6.connectLine(  4, board->s_16H );
    f6.connectLine(  5, board->s_128H );
    f6.connectLine(  9, board->s_128H );
    f6.connectLine( 10, board->s_32H );
    f6.connectLine( 11, board->s_64H );

    f7.connectLine(  4, board->s_NOT_ATTRACT );
    f7.connectLine(  5, f6,  6 );
    f7.connectLine(  9, f6,  8 );
    f7.connectLine( 10, board->s_NOT_ATTRACT );
    // F7, 6    -> NOT STOP 2
    // F7, 8    -> NOT STOP 1

    // Paddle window 
    h2.connectLine(  9, board->s_128V );
    h2.connectLine( 10, board->s_32V );
    h2.connectLine( 11, board->s_64V );

    b1.connectLine( 12, board->s_16V );
    b1.connectLine( 13, board->s_8V );

    k6.connectLine(  8, b1, 11 );
    k6.connectLine(  9, h2,  8 );
    // K6,10    -> PADDLE 1&2

    // Paddle 1 
    c7.connectLine(  1, board->s_POWER );
    c7.connectLine(  2, board->s_1H );
    c7.connectLine(  3, board->s_GROUND );
    c7.connectLine(  4, board->s_GROUND );
    c7.connectLine(  5, board->s_GROUND );
    c7.connectLine(  6, board->s_GROUND );
    c7.connectLine(  7, d7, 11 );
    c7.connectLine(  9, c8,  4 );
    c7.connectLine( 10, board->s_POWER );
    // C7,11    -> 1D
    // C7,12    -> 1C
    // C7,13    -> 1B
    // C7,14    -> 1A
    // C7,15    -> Carry

    c8.connectLine(  5, c9,  3 );
    c8.connectLine(  6, board->s_HBLANK );

    d7.connectLine( 12, board->s_NOT_ATTRACT );
    d7.connectLine( 13, c7, 15 );

    b8.connectLine(  3, k6, 10 );
    b8.connectLine(  4, d7, 11 );
    b8.connectLine(  5, c8,  4 );
    // B8, 6    -> NOT DISP PAD 1

    // Paddle 2 
    b7.connectLine(  1, board->s_POWER );
    b7.connectLine(  2, board->s_1H );
    b7.connectLine(  3, board->s_GROUND );
    b7.connectLine(  4, board->s_GROUND );
    b7.connectLine(  5, board->s_GROUND );
    b7.connectLine(  6, board->s_GROUND );
    b7.connectLine(  7, d7,  8 );
    b7.connectLine(  9, c8, 10 );
    b7.connectLine( 10, board->s_POWER );
    // B7,11    -> 2D
    // B7,12    -> 2C
    // B7,13    -> 2B
    // B7,14    -> 2A
    // B7,15    -> Carry

    c8.connectLine(  8, board->s_NOT_256H );
    c8.connectLine(  9, b9,  3 );

    d7.connectLine(  9, b7, 15 );
    d7.connectLine( 10, board->s_NOT_ATTRACT );

    b8.connectLine(  9, c8, 10 );
    b8.connectLine( 10, d7,  8 );
    b8.connectLine( 11, k6, 10 );
    // B8, 8    -> NOT DISP PAD 2

    // Interface to ball direction and speed 
    b6.connectLine(  1, board->s_256H );
    b6.connectLine(  2, c7, 11 ); // 1D
    b6.connectLine(  3, b7, 11 ); // 2D
    b6.connectLine(  5, c7, 12 ); // 1C
    b6.connectLine(  6, b7, 12 ); // 2C
    b6.connectLine( 14, c7, 13 ); // 1B
    b6.connectLine( 13, b7, 13 ); // 2B
    b6.connectLine( 11, b8,  6 ); // NOT DISP PAD 1
    b6.connectLine( 10, b8,  8 ); // NOT DISP PAD 2
    b6.connectLine( 15, board->s_GROUND );

    b5.connectLine(  1, b6,  9 );

    // Video signal
    e5.connectLine(  9, board->s_4V );
    e5.connectLine( 10, b5,  2 );

    // Board interface
    board->s_PADDLES.connectTo( e5, 8 );
    board->s_DISP_PAD_COMP.connectTo( b5, 2 );
    board->s_PADDLE_COUNT_1.connectTo( b6,  4 );
    board->s_PADDLE_COUNT_2.connectTo( b6,  7 );
    board->s_PADDLE_COUNT_4.connectTo( b6, 12 );
}

void ReboundPaddle::update()
{
    // Stop circuit
    f6.update();
    f7.update();

    // Paddle window circuit
    h2.update();
    b1.update();
    k6.update();

    // Timers
    c9.update();
    b9.update();

    // Paddle 1 and 2 circuits
    c8.update();

    for( int i=0; i<3; i++ ) {
        c7.update();
        b7.update();
        d7.update();
    }

    b8.update();

    // Video and ball direction interface
    b6.update();
    b5.update();
    e5.update();
}

void ReboundPaddle::setPaddlePos( double pad1, double pad2 )
{
    const double MinPad = 0.3;
    const double MaxPad = 4.7;

    if( pad1 < MinPad ) pad1 = MinPad;
    if( pad1 > MaxPad ) pad1 = MaxPad;

    if( pad2 < MinPad ) pad2 = MinPad;
    if( pad2 > MaxPad ) pad2 = MaxPad;

    c9.setControlVoltage( pad1 );
    b9.setControlVoltage( pad2 );
}
