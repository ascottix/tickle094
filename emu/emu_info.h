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
#ifndef EMU_INFO_H_
#define EMU_INFO_H_

#include "emu_list.h"
#include "emu_ui.h"

enum TResourceFileType
{
    efROM,
    efColorROM,
    efVideoROM,
    efPROM,
    efColorPROM,
    efPalettePROM,
    efSoundPROM,
    efTimerPROM,
    efSample,
    efOther
};

struct TResourceFileInfo
{
    TResourceFileInfo( int i, const char * n, unsigned c, unsigned s, TResourceFileType t ) :
        id(i), name(n), crc(c), size(s), type(t)
    {
    }

    int id;
    const char * name;
    unsigned crc;
    unsigned size;
    TResourceFileType type;
};

struct TMachineInfo
{
    const char * driver;
    const char * name;
    const char * manufacturer;
    int year;
    int screenWidth;
    int screenHeight;
    int screenColors;
    int framesPerSecond;
};

class TMachineDriverInfo
{
public:
    TMachineDriverInfo( const TMachineInfo * info ) : info_(info) {
    }

    /** Destructor. */
    ~TMachineDriverInfo();

    const TMachineInfo * machineInfo() const {
        return info_;
    }

    int resourceFileCount() const {
        return resource_files_.count();
    }

    const TResourceFileInfo * resourceFile( int index ) const {
        return (TResourceFileInfo *) resource_files_.item(index);
    }

    void clearResources();

    void addResourceFile( int id,  const char * name, unsigned crc, unsigned size, TResourceFileType type );

    TUserInterface * userInterface() const {
        return &user_interface_;
    }

private:
    const TMachineInfo * info_;
    TList resource_files_;
    mutable TUserInterface user_interface_;
};

#endif // EMU_INFO_H_
