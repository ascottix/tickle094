/*
    Analog sound emulation library

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
#include "ase_lowpass_filter.h"

ALowPassRCFilter::ALowPassRCFilter( AChannel & source, AFloat r, AFloat c )
    : AFilter( source )
{
    r_ = r;
    c_ = c;

    AFloat d = r_ * c_ * ASE::samplingRate;

    b_ = exp( -1.0 / d );
    a_ = 1.0 - b_;

    y_ = 0;
}

void ALowPassRCFilter::updateBuffer( AFloat * buf, unsigned len, unsigned ofs )
{
    source().updateTo( ofs );

    AFloat * src = source().stream() + streamSize();

    while( len > 0 ) {
        y_ = a_ * (*src++) + b_ * y_;

        *buf++ = y_;

        len--;
    }
}
