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
#include "ase_multiplexer.h"

AMultiplexer::AMultiplexer()
{
    channel_count_ = 0;
}

void AMultiplexer::addChannel( AChannel * channel, AFloat level )
{
    if( channel_count_ < MaxChannels ) {
        channel_[ channel_count_ ] = channel;
        channel_level_[ channel_count_ ] = level;

        channel_count_++;
    }
}

void AMultiplexer::updateBuffer( AFloat * buf, unsigned len, unsigned ofs )
{
    if( channel_count_ > 0 ) {
        int i;

        // Update all channels
        for( i=0; i<channel_count_; i++ ) {
            channel_[i]->updateTo( ofs );
        }

        // Copy first channel into buffer
        unsigned n;
        AFloat * src = channel_[0]->stream() + streamSize();
        AFloat * dst = buf;

        for( n=0; n<len; n++ ) {
            *dst++ = channel_level_[0] * (*src++);
        }

        // Mix all other channels
        for( i=1; i<channel_count_; i++ ) {
            AFloat * src = channel_[i]->stream() + streamSize();
            AFloat * dst = buf;

            for( n=0; n<len; n++ ) {
                *dst++ += channel_level_[i] * (*src++);
            }
        }
    }
}
