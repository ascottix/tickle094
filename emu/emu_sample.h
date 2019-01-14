/*
    Tickle class library

    Copyright (c) 2003,2004 Alessandro Scotti
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
#ifndef EMU_SAMPLE_H_
#define EMU_SAMPLE_H_

#include "emu_iostream.h"

class TSample
{
public:
    /** Destructor. */
    virtual ~TSample();

    unsigned size() const {
        return size_;
    }

    unsigned samplingRate() const {
        return sampling_rate_;
    }

    const int * data() const {
        return data_;
    }

    static TSample * createFromWave( TInputStream * is );

private:
    TSample( int * data, unsigned size, unsigned sampling_rate );

    unsigned size_;
    unsigned sampling_rate_;
    int * data_;
};

#endif // EMU_SAMPLE_H_
