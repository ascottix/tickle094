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
#ifndef EMU_ZIPFILE_H_
#define EMU_ZIPFILE_H_

#include <stdio.h>

#include "emu_iostream.h"
#include "emu_list.h"
#include "emu_string.h"

class TZipFile;

/**
    Entry in a ZIP archive.
*/
class TZipEntry
{
public:
    /** Destructor. */
    virtual ~TZipEntry() {
        delete name_;
    }

    const char * name() const {
        return name_;
    }

    unsigned size() const {
        return size_;
    }

    unsigned crc() const {
        return crc_;
    }

    TInputStream * open() const;

private:
    friend class TZipFile;

    TZipEntry( char * name, unsigned size, unsigned csize, unsigned offset, unsigned crc, int method, TZipFile * owner );
    
    unsigned offset() const {
        return offset_;
    }

    unsigned csize() const {
        return csize_;
    }

    int method() const {
        return method_;
    }

    TZipEntry( const TZipEntry & );
    TZipEntry & operator = ( const TZipEntry & );

    char * name_;
    unsigned size_;
    unsigned csize_;
    unsigned crc_;
    unsigned offset_;
    int method_;
    TZipFile * owner_;
};

class TZipFile
{
public:
    /** Destructor. */
    virtual ~TZipFile();

    const TZipEntry * entry( int index ) {
        return (TZipEntry *) entries_[index];
    }

    const TZipEntry * entry( const char * name, bool nameonly = false );

    int count() const {
        return entries_.count();
    }

    static TZipFile * open( const char * name );

private:
    friend class TZipEntry;

    TInputStream * openEntry( const TZipEntry * entry ) const;

    TZipFile( FILE * f );

    FILE * f_;
    TList entries_;
};

#endif // EMU_ZIPFILE_H_
