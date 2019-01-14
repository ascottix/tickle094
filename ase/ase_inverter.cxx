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
#include "ase_inverter.h"

AInverter::AInverter( AChannel & source, AFloat hi, AFloat lo, AFloat threshold )
    : AFilter( source )
{
    hi_ = hi;
    lo_ = lo;
    threshold_ = threshold;
}

void AInverter::updateBuffer( AFloat * buf, unsigned len, unsigned ofs )
{
    source().updateTo( ofs );

    AFloat * src = source().stream() + streamSize();

    while( len > 0 ) {
        if( *src++ > threshold_ ) {
            *buf = lo_;
        }
        else {
            *buf = hi_;
        }

        buf++;

        len--;
    }
}
