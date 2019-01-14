/*
    RC filter

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
#ifndef RCFILTER_H_
#define RCFILTER_H_

class RCFilter 
{
public:
    RCFilter( double r, double c ) {
        setRC( r, c );
        y_ = 0.0;
    }

    void setRC( double r, double c ) {
        r_ = r;
        c_ = c;
        sampling_rate_ = 0; // Force a refresh next time filter is applied
    }

    void reset( double y = 0.0 ) {
        y_ = y;
    }

    void apply( int * buf, unsigned len, unsigned samplingRate );

private:
    unsigned sampling_rate_;
    double y_; // Last sample
    double a_;
    double b_;
    double r_; // Resistor value in Ohms
    double c_; // Capacitor value in Farads
};

#endif // RCFILTER_H_
