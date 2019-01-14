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
#include "ase_noise.h"

AWhiteNoise::AWhiteNoise()
{
    t_ = 0;
    t_step_ = 1;
    t_half_period_ = 0;

    setOutput( 1, 0 );
    setValue( 1 );
    setTapMask( 0x30009 );
}

AWhiteNoise::AWhiteNoise( AFloat frequency )
{
    t_ = 0;
    t_step_ = 1.0 / ASE::samplingRate;
    t_half_period_ = (1 / frequency) / 2;

    setOutput( 1, 0 );
    setValue( 1 );
    setTapMask( 0x30009 );
}

void AWhiteNoise::setValue( unsigned value )
{
    value_ = value;
    output_ = (value_ & 1) ? o_hi_ : o_lo_;
}

void AWhiteNoise::setTapMask( unsigned tap )
{
    tap_mask_ = tap;
}

void AWhiteNoise::setOutput( AFloat hi, AFloat lo )
{
    o_hi_ = hi;
    o_lo_ = lo;
}

void AWhiteNoise::updateBuffer( AFloat * buf, unsigned len, unsigned ofs )
{
    while( len > 0 ) {
        *buf = output_;

        t_ += t_step_;

        if( t_ >= t_half_period_ ) {
            t_ -= t_half_period_;

            if( value_ & 1 ) {
                value_ ^= tap_mask_;
                output_ = o_hi_;
            }
            else {
                output_ = o_lo_;
            }

            value_ >>= 1;

            assert( value_ != 0 );
        }

        buf++;
        len--;
    }
}
