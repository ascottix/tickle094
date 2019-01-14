/*
    Fantasy arcade machine emulator

    Copyright (c) 2011 Alessandro Scotti
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
#ifndef FANTASY_H_
#define FANTASY_H_

#include "nibbler.h"

struct FantasyBoard : public NibblerBoard
{
    void writeByte( unsigned, unsigned char ); // To add the speech port

    Hd38880_SimWithSamples hd38880_;
    TSamplePlayer speech_samples_[12];
};

/**
    Fantasy machine driver.

    @author Alessandro Scotti
    @version 1.0
*/
class Fantasy : public Nibbler
{
public:
    virtual bool setResourceFile( int id, const unsigned char * buf, unsigned len );
    
    virtual void run( TFrame * frame, unsigned samplesPerFrame, unsigned samplingRate );
    
    static TMachine * createInstance() {
        return new Fantasy( new FantasyBoard );
    }

protected:
    virtual bool initialize( TMachineDriverInfo * info );

    Fantasy( FantasyBoard * );

    unsigned char speech_rom_[0x1800];
};

#endif // FANTASY_H_
