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
#ifndef ASE_TRIANGLE_WAVE_VCO1_H_
#define ASE_TRIANGLE_WAVE_VCO1_H_

#include "ase.h"

struct ATriangleWaveVCO
{
    ATriangleWaveVCO( double rampTime1, double rampTime2 );
    
    void play( double toTime = 0, double fadeTime = 0 );
    
    void fade( double fadeTime );
    
    void stop();
    
    void mix( int * buf, int len );
    
    bool stopped() {
        return stopped_;
    }
    
    double wf1_period_[2];
    int wf1_period_index_;
    double gain_;
    int o_;
    int p_;
    double step_;
    double vout_[2]; // v[0] from the "source" triangle wave generator, v[1] from the VCO generator (controlled by first triangle wave)
    bool asc_;
    int stopCount_;
    int fadeCount_;
    int fadeOffset_;
    bool stopped_;
};

#endif // ASE_TRIANGLE_WAVE_VCO1_H_
