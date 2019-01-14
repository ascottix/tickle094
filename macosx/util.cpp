/*
     Tickle front-end
     User interface for Mac OS X
     
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
#include "util.h"

#ifdef WIN32
const char * PathSeparator = "\\";
#else
const char * PathSeparator = "/";
#endif

// Loads a file from an input stream into a newly allocated buffer (which must be deleted by the caller)
unsigned char * loadFileFromStream( TInputStream * is, unsigned size, TCRC32 * crc )
{
    unsigned char * result = new unsigned char [size];
    
    if( is->read( result, size ) == size ) {
        crc->reset();
        crc->update( result, size );
    }
    else {
        delete result;
        result = 0;
    }
    
    return result;
}

// Returns the size of the specified file
unsigned getFileSize( const char * name )
{
    unsigned result = 0;
    
    FILE * f = fopen( name, "rb" );

    if( f != 0 ) {
        fseek( f, 0L, SEEK_END );
        result = (unsigned) ftell( f );
        fclose( f );
    }
    
    return result;
}

static DialogItemIndex messageBox( const char * title, const char * text )
{
    DialogRef alert;
    DialogItemIndex alert_exit_code;
    CFStringRef s_title = CFStringCreateWithCString( 0, title, kCFStringEncodingASCII );
    CFStringRef s_text = CFStringCreateWithCString( 0, text, kCFStringEncodingASCII );
    
    CreateStandardAlert( kAlertNoteAlert, s_title, s_text, 0, &alert );
    
    RunStandardAlert( alert, 0, &alert_exit_code );
    
    CFRelease( s_text );
    CFRelease( s_title );
    
    return alert_exit_code;
}


// Loads a single file required by the driver
unsigned char * loadFile( TMachine * machine, const TResourceFileInfo * info, const char * base, unsigned * bufsize )
{
    unsigned char * result = 0;
    
    TString home;

    if( base != 0 ) {
        home += base;
        home += PathSeparator;
    }
    
    TString file = home + info->name;
    TCRC32 crc;
    TInputStream * is = TFileInputStream::open( file.cstr() );
    
    if( is != 0 ) {
        unsigned size = info->size;
        
        if( size == 0 ) {
            size = getFileSize( file.cstr() );
        }
        
        result = loadFileFromStream( is, size, &crc );
        
        *bufsize = size;
        
        delete is;
    }
    else {
        for( int i=0; i<machine->getResourceCount(); i++ ) {
            file = home + machine->getResourceName(i) + ".zip";
            
            TZipFile * zf = TZipFile::open( file.cstr() );
            
            if( zf != 0 ) {
                const TZipEntry * ze = zf->entry( info->name, true );
                
                if( ze != 0 ) {
                    is = ze->open();
                    
                    if( is != 0 ) {
                        *bufsize = info->size ? info->size : ze->size();
                        result = loadFileFromStream( is, *bufsize, &crc );
                    }

                    delete is;
                }

                delete zf;
            }

            if( result != 0 )
                break;
        }
    }
    
    if( result != 0 ) {
        // Delete buffer if CRC check fails
        if( (info->crc != 0) && (crc.value() != info->crc) ) {
            delete result;
            result = 0;
        }
    }
    
    return result;
}

// Loads all the files required by the driver
bool loadMachineFiles( TMachine * machine, TList & failedList )
{
    bool result = true;
    const TMachineDriverInfo * info = machine->getDriverInfo();
    
    for( int i=0; i<info->resourceFileCount(); i++ ) {
        const TResourceFileInfo * file = info->resourceFile( i );
        unsigned size = 0;
        unsigned char * buf = loadFile( machine, file, RomsDir, &size );
        
        if( (buf == 0) && (strstr(file->name,".wav") != 0) ) {
            buf = loadFile( machine, file, SamplesDir, &size );
        }
        
        if( buf != 0 ) {
            machine->setResourceFile( file->id, buf, size );
            delete buf;
        }
        else {
            failedList.add( (void *)file->name );
            result = false;
        }
    }
    
    return result;
}
