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
#ifndef ASE_NOISE_H_
#define ASE_NOISE_H_

#include "ase.h"

class AWhiteNoise : public AChannel
{
public:
    AWhiteNoise();

    AWhiteNoise( AFloat frequency );

    void setValue( unsigned value );

    void setTapMask( unsigned tap );

    void setOutput( AFloat hi, AFloat lo );

protected:
    virtual void updateBuffer( AFloat * buf, unsigned len, unsigned ofs );

private:
    unsigned value_;
    unsigned tap_mask_;
    AFloat o_hi_;
    AFloat o_lo_;
    AFloat t_;
    AFloat t_step_;
    AFloat t_half_period_;
    AFloat output_;
};

#endif // ASE_NOISE_H_
