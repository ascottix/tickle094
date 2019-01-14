/*
    RC filter

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
#include <math.h>

#include "rcfilter.h"

void RCFilter::apply( int * buf, unsigned len, unsigned samplingRate ) 
{
    // Recalc coefficients if needed
    if( samplingRate != sampling_rate_ ) {
        sampling_rate_ = samplingRate;

        double d = r_ * c_ * sampling_rate_;
        
        b_ = exp( -1/d );
        a_ = 1 - b_;
    }

    while( len > 0 ) {
        y_ = a_ * (*buf) + b_ * y_;
        *buf++ = (int) y_;
        len--;
    }
}
