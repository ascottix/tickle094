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
#ifndef ASE_H_
#define ASE_H_

#ifdef WIN32
#include <windows.h>

void trace( const char * fmt, ... );
#endif

#include <assert.h>
#include <math.h>
#include <string.h>

typedef double AFloat; // Doesn't seem to work with less than double precision!

class ASE
{
public:
    static unsigned samplingRate;
    static AFloat samplesPerMs;
    static unsigned initialBufferSize;

    static AFloat getRCFactor( AFloat r, AFloat c ) {
        return exp( -1.0 / (r * c * ASE::samplingRate) );
    }

    static AFloat resInParallel( AFloat r1, AFloat r2 ) {
        return 1.0 / (1.0 / r1 + 1.0 / r2);
    }
};

class AChannel
{
public:
    AChannel();

    virtual ~AChannel();

    virtual void updateTo( unsigned offset );

    virtual void updateToTime( double time );

    void resetStream() {
        updateTo( 0 );
    }

    AFloat * stream() {
        return buffer_;
    }

    void mixStream( int * dest, AFloat gain );

    void mixStream( int * dest, AFloat gain, int outMin, int outMax );

    unsigned streamSize() {
        return bufofs_;
    }

    bool isEnabled() {
        return enabled_;
    }

    void setEnabled( bool enabled ) {
        enabled_ = enabled;
    }

protected:
    virtual void updateBuffer( AFloat * buf, unsigned len, unsigned ofs ) = 0;

private:
    AFloat * buffer_;
    unsigned buflen_;
    unsigned bufofs_;

    bool enabled_;
};

inline AFloat Mega( AFloat x ) {
    return x*1e+6;
}

inline AFloat Kilo( AFloat x ) {
    return x*1e+3;
}

inline AFloat Micro( AFloat x ) {
    return x*1e-6;
}

inline AFloat Pico( AFloat x ) {
    return x*1e-9;
}

#endif // ASE_H_
