/*
     Tickle front-end
     User interface for Mac OS X
     
     Copyright (c) 2006-2011 Alessandro Scotti
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
#include "opengl.h"

GLint DefaultInitAttributes[] = { AGL_RGBA, AGL_DOUBLEBUFFER, AGL_NONE };

/*
    Initialization is easy... when you know how to do it! ;-)
    Thanks to Keith Bauer for providing working code samples and some great tips:
    http://www.onesadcookie.com/CreatingMacGames/CreatingMacGames.xhtml
*/ 
AGLContext initOpenGL( WindowRef window, GLint * attributes )
{
    // Choose pixel format
    if( attributes == 0 ) {
        attributes = DefaultInitAttributes;
    }

#ifdef USE_GETGWORLDDEVICE
    GDHandle screen = GetGWorldDevice( GetWindowPort(window) ); // Get handle of screen device (note: deprecated)
    
    if( screen == 0 ) {
        printf( "initOpenGL: cannot get GDevice for window\n" );
        return 0;
    }
    
    AGLPixelFormat pixelFormat = aglChoosePixelFormat( &screen, 1, attributes );
#else
    AGLPixelFormat pixelFormat = aglChoosePixelFormat( 0, 1, attributes );
#endif
    
    if( pixelFormat == 0 ) {
        printf( "initOpenGL: cannot get pixel format\n" );
        return 0;
    }
    
    // Create context
    AGLContext context = aglCreateContext( pixelFormat, NULL );
    
    aglDestroyPixelFormat(pixelFormat);
    
    if( context == 0 ) {
        printf( "initOpenGL: cannot create context\n" );
        return 0;
    }

    // Make context drawable
    if( ! aglSetWindowRef( context, window ) ) {
        aglDestroyContext(context);
        printf( "initOpenGL: cannot make context drawable\n" );
        return 0;
    }
    
    // Make context current
    if( ! aglSetCurrentContext( context ) ) {
        aglDestroyContext(context);
        printf( "initOpenGL: cannot make context current\n" );
        return 0;
    }
    
    return context;
}

int setAglContextSize( AGLContext context, int width, int height )
{
    GLint dim[2];
    
    dim[0] = width;
    dim[1] = height;

    aglSetCurrentContext( 0 );
    aglSetInteger( context, AGL_SURFACE_BACKING_SIZE, dim );
    aglEnable( context, AGL_ENABLE_SURFACE_BACKING_SIZE );
    aglSetCurrentContext( context );
    
    return 0;
}

int termOpenGL( AGLContext context )
{
    aglSetCurrentContext( NULL );
    aglSetWindowRef( context, NULL );
    aglDestroyContext( context );
    
    return 0;
}
