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
#ifndef EMU_INITPROXY_H_
#define EMU_INITPROXY_H_

typedef void (* TInitFunction)( void * );

class TInitProxy
{
public:
    TInitProxy( TInitFunction onInit, TInitFunction onTerm = 0, void * param = 0 ) : init_func_(onInit), term_func_(onTerm), param_(param) {
        if( init_func_ != 0 ) {
            init_func_( param_ );
        }
    }

    /** 
        Destructor. 
        
        Invokes the user defined termination function.
    */
    ~TInitProxy() {
        if( term_func_ != 0 ) {
            term_func_( param_ );
        }
    }

private:
    TInitFunction init_func_;
    TInitFunction term_func_;
    void * param_;
};

#endif // EMU_INITPROXY_H_
