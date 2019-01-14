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
#ifndef ASE_CAPACITOR_WITH_SWITCH_H_
#define ASE_CAPACITOR_WITH_SWITCH_H_

#include "ase.h"

/*
    An emulation of the following circuit:

    
    Vin ---R1---C---|X|---R2---+--- Vout
                     |         |
                     |         |
                  Control     GND

    That is, capacitor C is charges thru R1 but only discharges thru R2 when
    the switch is open.
*/
class ACapacitorWithSwitch : public AChannel
{
public:
    ACapacitorWithSwitch( AChannel & input, AChannel & control, AFloat r1, AFloat r2, AFloat c );

    void setCharge( AFloat charge ) {
        y_ = charge;
    }

protected:
    virtual void updateBuffer( AFloat * buf, unsigned len, unsigned ofs );

private:
    AChannel & input_;
    AChannel & control_;
    AFloat c_;
    AFloat r0_;
    AFloat r1_;
    AFloat a0_;
    AFloat b0_;
    AFloat b1_;
    AFloat y_;
};

#endif // ASE_CAPACITOR_WITH_SWITCH_H_
