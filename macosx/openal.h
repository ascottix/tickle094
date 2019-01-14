/*
     Tickle front-end
     OpenAL sound manager
     
     Copyright (c) 2006 Alessandro Scotti
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
#ifndef TICKLE_OPENAL_H_
#define TICKLE_OPENAL_H_

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

class OpenALPlayback
{
public:
    OpenALPlayback( int samplingRate, int samplesPerFrame, const char * device = 0 );

    ~OpenALPlayback();

    int start();

    int stop();

    int synchAndPlay( const int * buf, int voices );

    int getLastError() {
        return last_error_;
    }

private:
    int clearBufferData();
        
    int sampling_rate_;
    int samples_per_frame_;
    ALCdevice * device_;
    ALCcontext * context_;
    ALuint * buffers_;
    ALuint * source_;
    signed short * sound_buffer_;
    int last_error_;
};

#endif // TICKLE_OPENAL_H_
