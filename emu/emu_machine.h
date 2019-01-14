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
#ifndef EMU_MACHINE_H_
#define EMU_MACHINE_H_

#include "emu_frame.h"
#include "emu_info.h"
#include "emu_list.h"
#include "emu_sample.h"
#include "emu_ui.h"

class TMachine;

typedef TMachine * (* TMachineFactoryFunc)();

class TMachine
{
public:
    /** Destructor. */
    virtual ~TMachine();

    
    const TMachineDriverInfo * getDriverInfo() {
        return info_;
    }

    int getResourceCount() const {
        return drivers_.count();
    }

    const char * getResourceName( int index ) const;


    virtual bool setResourceFile( int id, const unsigned char * buf, unsigned len ) {
        return false;
    }

    virtual bool handleInputEvent( unsigned device, unsigned param, void * data = 0 ) = 0;

    virtual void run( TFrame * frame, unsigned samplesPerFrame, unsigned samplingRate ) = 0;

    virtual void reset() = 0;


    static TMachine * createInstance( TMachineFactoryFunc factoryFunc );


    static bool copyResourceFile( unsigned char * dst, unsigned dst_size, const unsigned char * src, unsigned src_size );

    static TSample * copySample( const unsigned char * buf, unsigned len );

    static bool replaceMemory( unsigned char * address, const unsigned char * original, const unsigned char * replacement, unsigned len );

    static void replaceByte( unsigned char * address, unsigned char original, unsigned char replacement, unsigned len );

protected:
    TMachine();

    void registerDriver( const TMachineInfo & driver );

    virtual bool initialize( TMachineDriverInfo * info ) = 0;

private:
    TMachine( const TMachine & );
    TMachine & operator = ( const TMachine & );

    TMachineDriverInfo * info_;
    TList drivers_;
};

#endif // EMU_MACHINE_H_
