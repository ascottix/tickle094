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
#include "rebound_serve.h"

static DBit b_serve_time = 0;
static DBit b_not_serve_time;
static unsigned counter = 0;

ReboundServe::ReboundServe( ReboundBoard * board )
{
    // TODO k1.connectLine(  1, 47 uF );
    // TODO k1.connectLine(  2, 33 KOhm );
    k1.connectLine(  3, board->s_POWER );
    k1.connectLine(  4, board->s_START );
    // TODO k1.connectLine(  5, SCORE SOUND );

    // Note: in the schematics the NOT 64V signal comes from
    // the score circuit, but here it's better to regenerate it
    // in order to avoid unnecessary module dependencies (this
    // will allow us to turn off the score circuit if needed,
    // and still have the serve circuit working)
    c3.connectLine(  3, board->s_64V );
    c3.connectLine(  4, board->s_64V );
    c3.connectLine(  5, board->s_64V );
    // C3, 6    -> NOT 64V

    c3.connectLine(  9, board->s_NOT_256H );
    c3.connectLine( 10, board->s_128H );
    c3.connectLine( 11, board->s_64H );

    h6.connectLine(  9, board->s_128V );
    h6.connectLine( 10, c3,  6 ); // NOT 64V, see note above
    h6.connectLine( 11, c3,  8 );

    j1.connectLine( 10, board->s_POWER );
    j1.connectLine( 11, h6,  8 );
    /* TODO!!!
    j1.connectLine( 12, k1,  7 );
    j1.connectLine( 13, k1,  7 );
    */

    board->s_SERVE.connectTo( j1, 8 );
    board->s_NOT_SERVE.connectTo( j1, 9 );

    /*
    board->s_SERVE_TIME.connectTo( k1, 6 );
    board->s_NOT_SERVE_TIME.connectTo( k1, 7 );
    */
    board->s_SERVE_TIME.assign( b_serve_time );
    board->s_NOT_SERVE_TIME.assign( b_not_serve_time );

    // TODO!!! see J1 above
    j1.connectLine( 12, board->s_NOT_SERVE_TIME );
    j1.connectLine( 13, board->s_NOT_SERVE_TIME );
}

void ReboundServe::update()
{
    counter++;
    if( counter > 10000 ) b_serve_time = 1;
    if( counter > 56000 ) b_serve_time = 0;
    b_not_serve_time = 1 ^ b_serve_time;

    k1.update();
    c3.update();
    h6.update();
    j1.update();

    // B5,12 also produces NOT SERVE and is only used for easier assembling 
    // of the PCB: we don't need to emulate that as other modules can
    // simply use the global NOT SERVE signal
}
