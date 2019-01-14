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
#include "rebound_net.h"

ReboundNet::ReboundNet( ReboundBoard * board )
{
    // TODO: set resistor and capacitor
    // 10 uF -> 15
    // 47 K -> 14
    k1.connectLine( 11, board->s_NOT_SERVE );
    k1.connectLine( 12, board->s_BALL_RETURN );
    k1.connectLine( 13, board->s_POWER );
    // K1, 9

    j1.connectLine(  1, h1,  6 );
    j1.connectLine(  2, j1,  6 );
    j1.connectLine(  3, k1,  9 );
    j1.connectLine(  4, board->s_POWER );
    // J1, 5

    k2.connectLine(  2, board->s_SERVE );
    k2.connectLine(  3, board->s_SERVE );
    k2.connectLine( 14, j1,  6 );
    k2.connectLine(  1, k2, 12 );

    h1.connectLine(  4, k2,  9 );
    h1.connectLine(  5, k2, 11 );

    k3.connectLine(  2, board->s_4V );
    k3.connectLine(  3, board->s_POWER );
    k3.connectLine(  4, k2,  9 );
    k3.connectLine(  5, k2,  8 );
    k3.connectLine(  6, k2, 11 );
    k3.connectLine(  7, k4,  8 );
    k3.connectLine(  9, board->s_128V );
    k3.connectLine( 10, board->s_POWER );

    // From the paddle circuit...
    h2.connectLine(  9, board->s_128V );
    h2.connectLine( 10, board->s_32V );
    h2.connectLine( 11, board->s_64V );

    k4.connectLine(  4, h2,  8 );
    k4.connectLine(  5, k4,  8 );
    k4.connectLine(  9, k3, 15 );
    k4.connectLine( 10, board->s_128V );

    b5.connectLine( 11, k4,  6 );
    // B5,10    -> NOT NET HEIGHT

    b2.connectLine( 10, k4,  6 );
    b2.connectLine( 11, board->s_2H );
    b2.connectLine( 12, board->s_256H );
    b2.connectLine( 13, board->s_POWER );

    b8.connectLine(  1, board->s_256H );
    b8.connectLine(  2, board->s_4V );
    b8.connectLine( 13, b2,  8 );
    // B8,12    -> NOT NET

    // Board interface
    board->s_NOT_NET.connectTo( b8, 12 );
    board->s_NOT_NET_HEIGHT.connectTo( b5, 10 );
}

void ReboundNet::update()
{
    k1.update();

    for( int i=0; i<3; i++ ) {
        j1.update();
        k2.update();
        h1.update();
    }

    k3.update();
    k4.update();
    k3.update();    // K3/K4 feedback loop
    k4.update();

    h2.update();
    k4.update();    // Self-feedback

    b2.update();
    b5.update();
    b8.update();
}
