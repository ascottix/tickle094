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
#include <unistd.h>

#include "openal.h"

#define TRACE( e )  printf e

OpenALPlayback::OpenALPlayback( int samplingRate, int samplesPerFrame, const char * deviceName )
{
    sampling_rate_ = samplingRate;
    samples_per_frame_ = samplesPerFrame;
    device_ = 0;
    context_ = 0;
    buffers_ = 0;
    source_ = 0;
    sound_buffer_ = new signed short [ samplesPerFrame ];
    last_error_ = -1;
    
    // Open device
    device_ = alcOpenDevice( deviceName );
    
    if( device_ == 0 ) {
        TRACE(( "OpenALPlayback: Cannot open preferred device!\n" ));
        return;
    }
    
    // Create context
    context_ = alcCreateContext( device_, 0 );
    
    if( context_ == 0 ) {
        TRACE(( "OpenALPlayback: Cannot create context!\n" ));
        return;
    }
    
    alcMakeContextCurrent( context_ );
    
    // Create buffers (the short buffers we play need triple buffering)
    alGetError();
    
    ALuint buffers[3];
    
    alGenBuffers( 3, buffers );
    
    if( alGetError() != AL_NO_ERROR ) {
        TRACE(( "OpenALPlayback: Cannot generate buffers!\n" ));
        return;
    }
    
    buffers_ = new ALuint [3];
    
    buffers_[0] = buffers[0];
    buffers_[1] = buffers[1];
    buffers_[2] = buffers[2];
    
    // Create sources
    ALuint sources[1];
    
    alGenSources( 1, sources );
    
    if( alGetError() != AL_NO_ERROR ) {
        TRACE(( "OpenALPlayback: Cannot generate sources!\n" ));
        return;
    }
    
    source_ = new ALuint;
    
    *source_ = sources[0];
    
    // Queue buffers
    clearBufferData();
    
    // Initialization successful
    last_error_ = 0;
}

OpenALPlayback::~OpenALPlayback()
{
    if( source_ != 0 ) {
        alSourceStop( *source_ );
        alDeleteSources( 1, source_ );
        delete source_;
    }
    
    if( buffers_ != 0 ) {
        alDeleteBuffers( 2, buffers_ );
        delete [] buffers_;
    }
    
    if( context_ != 0 ) {
        alcMakeContextCurrent( 0 );
        alcDestroyContext( context_ );
    }
    
    if( device_ != 0 ) {
        alcCloseDevice( device_ );
    }
}

int OpenALPlayback::start()
{
    int result = -1;
    
    if( source_ != 0 ) {
        clearBufferData();
        alSourceQueueBuffers( *source_, 3, buffers_ );
        alSourcePlay( *source_ );

        // Wait for device to start playing
        ALint info;

        for( int i=0; i<5; i++ ) {
            usleep( 1*1000 );
            
            alGetSourcei( *source_, AL_SOURCE_STATE, &info );
            
            if( alGetError() != AL_NO_ERROR ) {
                continue;
            }
            
            if( info != AL_PLAYING ) {
                continue;
            }

            result = 0;
            break;
        }
    }
    
    return result;
}

int OpenALPlayback::stop()
{
    if( source_ != 0 ) {
        alSourceStop( *source_ );
    }
    
    return 0;
}

int OpenALPlayback::clearBufferData()
{
    signed short * data = new signed short [ samples_per_frame_ ];
    
    memset( data, 0, samples_per_frame_ * sizeof(signed short) );
    
    alBufferData( buffers_[0], AL_FORMAT_MONO16, data, samples_per_frame_ * sizeof(signed short), sampling_rate_ );
    alBufferData( buffers_[1], AL_FORMAT_MONO16, data, samples_per_frame_ * sizeof(signed short), sampling_rate_ );
    alBufferData( buffers_[2], AL_FORMAT_MONO16, data, samples_per_frame_ * sizeof(signed short), sampling_rate_ );
    
    delete [] data;
    
    return 0;
}

int OpenALPlayback::synchAndPlay( const int * buf, int voices )
{
    if( source_ != 0 ) {
        // Make sure source is playing
        ALint info;
        
        alGetSourcei( *source_, AL_SOURCE_STATE, &info );
        
        if( info != AL_PLAYING ) {
            return -2;
        }
        
        // Prepare buffer
        signed short * sndBuf = sound_buffer_;

        if( (buf == 0) || (voices <= 0) ) {
            memset( sndBuf, 0, sizeof(signed short)*samples_per_frame_ );
        }
        else {
            register int mulFactor = 128 / voices;

            for( int i=0; i<samples_per_frame_; i++ ) {
                *sndBuf++ = (*buf++ * mulFactor);
            }
        }
        
        // Wait for one buffer to finish playing
        do {
             // For reasons I don't understand, I get no sound if I don't query the state first...
            alGetSourcei( *source_, AL_SOURCE_STATE, &info );
            
            if( info != AL_PLAYING ) {
                return -4;
            }
            
            alGetSourcei( *source_, AL_BUFFERS_PROCESSED, &info );
            
            if( alGetError() != AL_NO_ERROR ) {
                return -3;
            }
        } while( info == 0 );
        
        // Put data into buffer and requeue it
        ALuint buffers[1];
        
        alSourceUnqueueBuffers( *source_, 1, buffers );
  
        alBufferData( buffers[0], AL_FORMAT_MONO16, sound_buffer_, samples_per_frame_ * sizeof(signed short), sampling_rate_ );
        
        alSourceQueueBuffers( *source_, 1, buffers );
    }

    return last_error_;
}
