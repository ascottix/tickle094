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
#ifndef REBOUND_PADDLE_H_
#define REBOUND_PADDLE_H_

#include "rebound.h"

class ReboundPaddle
{
public:
    ReboundPaddle( ReboundBoard * board );

    void update();

    // Typical values would be from 0.3 to 4.7
    void setPaddlePos( double pad1, double pad2 );

//private:
    D7400   b1;
    D7404   b5;
    D9322   b6;
    D9316   b7;
    D7410   b8;
    D9316   c7;
    D7402   c8;
    D7400   d7;
    D7408   e5;
    D7410   f6;
    D7408   f7;
    D7410   h2;
    D7402   k6;

    D555_Monostable c9;
    D555_Monostable b9;
};

#endif // REBOUND_PADDLE_H_
