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
#ifndef ASE_MULTIPLEXER_H_
#define ASE_MULTIPLEXER_H_

#include "ase.h"

class AMultiplexer : public AChannel
{
public:
    AMultiplexer();

    void addChannel( AChannel * channel, AFloat level );

protected:
    virtual void updateBuffer( AFloat * buf, unsigned len, unsigned ofs );

private:
    enum {
        MaxChannels = 8
    };

    AChannel * channel_[ MaxChannels ];
    AFloat channel_level_[ MaxChannels ];
    int channel_count_;
};

#endif // ASE_MULTIPLEXER_H_
