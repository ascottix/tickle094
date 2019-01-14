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
#include "ase_switch.h"

const AFloat ControlOnThreshold = 0.5;

ASwitch::ASwitch( AChannel & source, AChannel & control )
    : AFilter(source), control_(control)
{
}

void ASwitch::updateBuffer( AFloat * buf, unsigned len, unsigned ofs )
{
    source().updateTo( ofs );
    control_.updateTo( ofs );

    AFloat * src = source().stream() + streamSize();
    AFloat * ctl = control_.stream() + streamSize();

    while( len > 0 ) {
        if( *ctl > ControlOnThreshold ) {
            *buf = *src;
        }
        else {
            *buf = 0;
        }

        buf++;
        ctl++;
        src++;

        len--;
    }
}
