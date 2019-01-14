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
#include "ase_timer555_linear_ramp.h"

ATimer555LinearRamp::ATimer555LinearRamp( AFloat c )
{
    c_ = c;
    c_voltage_ = 0; // Initial charge
    flipflop_ = 1;  // Charging
    current_ = 0;
    c_dt_ = c * ASE::samplingRate;
    
    setVcc( 5.0 );
}

void ATimer555LinearRamp::setVcc( AFloat vcc )
{
    vcc_ = vcc;
    threshold_lo_ = (AFloat) (vcc_ / 3);
    threshold_hi_ = threshold_lo_ * 2;
}

void ATimer555LinearRamp::setCurrent( AChannel * current )
{
    current_ = current;
}

void ATimer555LinearRamp::updateBuffer( AFloat * buf, unsigned len, unsigned ofs )
{
    assert( current_ != 0 );

    current_->updateTo( ofs );

    AFloat * ctl = current_->stream() + streamSize();

    while( len > 0 ) {
        *buf++ = c_voltage_;

        AFloat s = *ctl++ / c_dt_;

        // Should check for zero voltage: for now, this check
        // must be performed somewhere else (i.e. when setting the
        // controlling current)
        c_voltage_ += s;

        if( c_voltage_ >= threshold_hi_ ) {
            // For practical purposes, we can assume voltage drop instantaneously
            // (actually it will take 25-30 microseconds or so)
            c_voltage_ = threshold_lo_;
        }

        len--;
    }
}
