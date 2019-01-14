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
#ifndef ASE_OPAMP_NONINV1_H_
#define ASE_OPAMP_NONINV1_H_

#include "ase.h"
#include "ase_filter.h"

/*
    This class implements a non-inverting operational amplifier
    with the following configuration:

                Vcc
                 |
                 R1
                 |
        +---R2---+-----R3-----+
        |        |            |
        |        |   \        |
        |        |   | \      |
       GND       +---| - \    |
                     |    >---+--- Vout
             Vin +---| + /
                     | / 
                     /
*/
class AOpAmp_NonInv1 : public AFilter
{
public:
    AOpAmp_NonInv1( AChannel & source, AFloat r1, AFloat r2, AFloat r3, AFloat vcc );

protected:
    virtual void updateBuffer( AFloat * buf, unsigned len, unsigned ofs );

private:
    AFloat r1_;
    AFloat r2_;
    AFloat r3_;
    AFloat vcc_;
    AFloat a_;
    AFloat b_;
};

#endif // ASE_OPAMP_NONINV1_H_
