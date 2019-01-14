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
#ifndef REBOUND_NET_H_
#define REBOUND_NET_H_

#include "rebound.h"

class ReboundNet
{
public:
    ReboundNet( ReboundBoard * board );

    void update();

//private:
    D7474   b2;
    D7404   b5;
    D7410   b8;
    D7400   h1;
    D7410   h2; // Same chip as in the paddle circuit
    D7474   j1;
    D9602   k1;
    D7493   k2;
    D9316   k3;
    D7400   k4;
};

#endif // REBOUND_NET_H_
