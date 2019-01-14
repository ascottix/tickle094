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
#ifndef ASE_BANDPASS_FILTER_H_
#define ASE_BANDPASS_FILTER_H_

#include "ase.h"
#include "ase_filter.h"

#include "complex.h"

/*
    2nd order Butterworth bandpass filter.
*/
class AButterworthBandPassFilter : public AFilter
{
public:
    AButterworthBandPassFilter( AChannel & source );

    void clear();

    void setBand( double lo_freq, double hi_freq );

    AFloat gain() {
        return gain_;
    }

    void setGain( AFloat gain ) {
        gain_ = gain;
    }

protected:
    virtual void updateBuffer( AFloat * buf, unsigned len, unsigned ofs );

private:
    AFloat gain_;
    AFloat x[5];
    AFloat y[5];
    AFloat a[5];
    AFloat b[5]; // For simmetry, actually b[0] is never used
};

/*
    Second-order active pass-band filter.
    
    This class implements a filter of the following type:


                +--------+------------+
                |        |            |
                C1      R3   \        |
                |        |   | \      |
    Vin ---R1---+---C2---+---| - \    |
                |            |    >---+--- Vout
                R2       +---| + /
                |        |   | / 
                |        |   /
               GND     Vref  


    As I haven't yet worked out the math to implement the above circuit,
    at present it is remapped to a 2nd-order Butterworth filter.
*/
class AActiveBandPassFilter : public AButterworthBandPassFilter
{
public:
    AActiveBandPassFilter( AChannel & source, double r1, double r2, double r3, double c1, double c2 );

    void setR1( double r1 ) {
        r1_ = r1;
    }

    void setBand();

private:
    double r1_;
    double r2_;
    double r3_;
    double c1_;
    double c2_;
};

#endif // ASE_BANDPASS_FILTER_H_
