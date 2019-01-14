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
#include <assert.h>

#include "emu_sample_player.h"

enum {
    statusPlaying = 0x01,
    statusLooping = 0x02
};

TSamplePlayer::TSamplePlayer()
{
    status_ = 0;
    offset_ = 0;
    sample_ = 0;
}

TSamplePlayer::~TSamplePlayer()
{
    delete sample_;
}

void TSamplePlayer::setSample( TSample * sample )
{
    stop();
    delete sample_;
    sample_ = sample;
}

void TSamplePlayer::play( unsigned mode )
{
    if( (status_ & statusPlaying) == 0 ) {
        status_ = statusPlaying | ((mode & pmLooping) ? statusLooping : 0);
        offset_ = 0;
    }
}

void TSamplePlayer::stop()
{
    status_ = 0;
}

bool TSamplePlayer::mix( int * buf, unsigned len, unsigned samplingRate )
{
    bool result = false;

    if( (sample_ != 0) && (status_ & statusPlaying) ) {
        unsigned step = (sample_->samplingRate() << 10) / samplingRate;
        unsigned size = sample_->size() << 10;
        const int * data = sample_->data();

        while( len > 0 ) {
            *buf++ += data[ offset_ >> 10 ];

            offset_ += step;

            if( offset_ >= size ) {
                if( status_ & statusLooping ) {
                    offset_ -= size;
                }
                else {
                    stop();
                    break;
                }
            }

            len--;
        }

        result = true;
    }

    return result;
}

bool TSamplePlayer::mix( TMixer * mixer, unsigned channel, unsigned len, unsigned samplingRate )
{
    bool result = false;

    if( (sample_ != 0) && (status_ & statusPlaying) ) {
        TMixerBuffer * buffer = mixer->getBuffer( channel, len, 1 );

        result = mix( buffer->data(), len, samplingRate );
    }

    return result;
}
