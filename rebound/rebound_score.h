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
#ifndef REBOUND_SCORE_H_
#define REBOUND_SCORE_H_

#include "rebound.h"

class ReboundScore
{
public:
    ReboundScore( ReboundBoard * board );

    void update();

//private:
    D7402   c8;
    D7410   f6;
    D7408   f7;
    D7410   f8;
    D7404   f9;
    D7427   h6;
    D7410   h7;
    D7430   h8;
    D74107  h9;
    D7486   j5;
    D7400   j6;
    D7410   j7;
    D74153  j8;
    D7490   j9;
    D7402   k6;
    D7448   k7;
    D74153  k8;
    D7490   k9;
};

#endif // REBOUND_SCORE_H_
