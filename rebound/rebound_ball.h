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
#ifndef REBOUND_BALL_H_
#define REBOUND_BALL_H_

#include "rebound.h"

class ReboundBall
{
public:
    ReboundBall( ReboundBoard * board );

    void update();

//private:
    // Ball direction and speed
    D9316   b4;
    D7404   b5;
    D7486   c5;
    D7474   c6;
    D7400   d5;
    D7474   d6;
    D74193  e4;
    D7408   e5;
    D7402   e6;

    // Horizontal ball motion
    D7410   c3;
    D9316   d3;
    D7400   e1;
    D7483   e2;
    D9316   e3;
    D7474   f1;
    D7408   f2;
    D74107  f3;
    D7420   f4;
    D7400   h1;

    // Vertical ball motion
    D9316   c4;
    D9316   d4;
};

#endif // REBOUND_BALL_H_
