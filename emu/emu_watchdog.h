/*
    Tickle class library

    Copyright (c) 1997-2003,2004 Alessandro Scotti
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
#ifndef EMU_WATCHDOG_H_
#define EMU_WATCHDOG_H_

class TWatchDog
{
public:
    TWatchDog( int timeout = 0 ) : timeout_(timeout) {
        reset();
    }

    void setTimeout( int timeout ) {
        timeout_ = timeout;
    }

    int timeout() const {
        return timeout_;
    }

    bool tick( TMachine * machine ) {
        bool result = false;

        time_++;

        if( (timeout_ > 0) && (time_ > timeout_) ) {
            reset();
            machine->reset();
            result = true;
        }

        return result;
    }

    void reset() {
        time_ = 0;
    }

private:
    int time_;
    int timeout_;
};

#endif // EMU_WATCHDOG_H_
