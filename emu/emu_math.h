/*
    Tickle class library

    Copyright (c) 2003,2004 Alessandro Scotti
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
#ifndef EMU_MATH_H_
#define EMU_MATH_H_

// Get rid of annoying macros that may be defined elsewhere
#undef min
#undef max

class TMath
{
public:
    static int abs( int n ) {
        return (n >= 0) ? n : -n;
    }

    static int min( int a, int b ) {
        return (a < b) ? a : b;
    }

    static int max( int a, int b ) {
        return (a > b) ? a : b;
    }

    static int clip( int value, int lo, int hi ) {
        return (value < lo) ? lo : (value > hi) ? hi : value;
    }

    static bool inRange( int value, int lo, int hi ) {
        return (value >= lo) && (value <= hi);
    }

private:
    TMath();
};

#endif // EMU_MATH_H_
