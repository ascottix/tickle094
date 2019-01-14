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
#ifndef ASE_TIMER555_LINEAR_RAMP_H_
#define ASE_TIMER555_LINEAR_RAMP_H_

#include "ase.h"

/*
    555 timer in astable/linear-ramp configuration.

    Although this is still an astable setup, here the capacitor doesn't follow the
    usual charge/discharge path using resistors but is charged thru a current
    source (typically, a transistor).
*/
class ATimer555LinearRamp : public AChannel
{
public:
    ATimer555LinearRamp( AFloat c );

    void setVcc( AFloat vcc );

    void setCurrent( AChannel * current );

    void dischargeCapacitor() {
        c_voltage_ = 0;
    }

protected:
    virtual void updateBuffer( AFloat * buf, unsigned len, unsigned ofs );

private:
    void setCapacitorCoefficients();

    AFloat c_;
    AFloat vcc_;
    AFloat threshold_hi_;
    AFloat threshold_lo_;
    AFloat c_dt_;

    AChannel * current_;

    int flipflop_;

    AFloat c_voltage_;  // Voltage across capacitor
};

#endif // ASE_TIMER555_LINEAR_RAMP_H_
