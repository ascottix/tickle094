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
#ifndef ASE_LOWPASS_FILTER_H_
#define ASE_LOWPASS_FILTER_H_

#include "ase.h"
#include "ase_filter.h"

class ALowPassRCFilter : public AFilter
{
public:
    ALowPassRCFilter( AChannel & source, AFloat r, AFloat c );

    void setCharge( AFloat y ) {
        y_ = y;
    }

protected:
    virtual void updateBuffer( AFloat * buf, unsigned len, unsigned ofs );

private:
    AFloat r_;
    AFloat c_;
    AFloat a_;
    AFloat b_;
    AFloat y_;
};

#endif // ASE_LOWPASS_FILTER_H_
