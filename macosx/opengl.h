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
#ifndef TICKLE_OPENGL_H_
#define TICKLE_OPENGL_H_

#include <AGL/agl.h>

#include <Carbon/Carbon.h>

AGLContext initOpenGL( WindowRef window, GLint * attributes );

int setAglContextSize( AGLContext context, int width, int height );

int termOpenGL( AGLContext context );

#endif // TICKLE_OPENGL_H_
