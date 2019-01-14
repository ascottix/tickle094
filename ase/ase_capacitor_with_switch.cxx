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
#include "ase_capacitor_with_switch.h"

const AFloat InputOnThreshold   = 1.0;
const AFloat ControlOnThreshold = 1.0;

ACapacitorWithSwitch ::ACapacitorWithSwitch( AChannel & input, AChannel & control, AFloat cr, AFloat dr, AFloat c )
    : input_(input), control_(control)
{
    c_ = c;
    r0_ = cr;
    r1_ = dr;

    y_ = 0; // No charge in the capacitor yet

    b0_ = ASE::getRCFactor( cr, c ); // Charge path
    a0_ = 1 - b0_;
    b1_ = ASE::getRCFactor( dr, c ); // Discharge path
}

void ACapacitorWithSwitch ::updateBuffer( AFloat * buf, unsigned len, unsigned ofs )
{
    input_.updateTo( ofs );
    control_.updateTo( ofs );

    AFloat * inp = input_.stream() + streamSize();
    AFloat * ctl = control_.stream() + streamSize();

    while( len > 0 ) {
        // Charge (avoid discharging thru this resistor)
        if( *inp > 0.1 ) {
            y_ = a0_ * (*inp) + b0_ * y_;
        }

        // Discharge and output
        if( *ctl > ControlOnThreshold ) {
            *buf = y_;

            y_ = y_ * b1_;
        }
        else {
            *buf = 0;
        }

        inp++;
        ctl++;

        buf++;

        len--;
    }
}
