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
#ifndef EMU_TICKLE_MACHINE_H_
#define EMU_TICKLE_MACHINE_H_

#include "emu_driver.h"

class TTickleMachine : public TMachine
{
public:
    static TMachine * create() {
        return TMachine::createInstance( TTickleMachine::createInstance );
    }

    /** Destructor. */
    ~TTickleMachine();
    
    virtual bool setResourceFile( int id, const unsigned char * buf, unsigned len );

    virtual bool handleInputEvent( unsigned device, unsigned param, void * data );

    virtual void run( TFrame * frame, unsigned samplesPerFrame, unsigned samplingRate );

    virtual void reset() {
    }

protected:
    TTickleMachine();

    virtual bool initialize( TMachineDriverInfo * info );

    static TMachine * createInstance() {
        return new TTickleMachine();
    }

private:
    void drawChar( int x, int y, char c );
    void drawString( int x, int y, const char * s );

    enum {
        stTestMode = 0x01,
        stP1StartPressed = 0x02,
        stCoin1Pressed = 0x04,
        stP1Action1Pressed = 0x08
    };

    TBitmapRGB * screen_;
    TBitmapRGB * splash_;
    TBitmapRGB * font_;
    TOptionToPortHandler option_handler_;
    unsigned last_device_;
    unsigned last_device_param_;
    unsigned status_;
    unsigned char dip_switches_;
};

#endif // EMU_TICKLE_MACHINE_H_
