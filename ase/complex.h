/*
    Complex numbers

    Copyright (c) 2004 Alessandro Scotti
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
#ifndef COMPLEX_H_
#define COMPLEX_H_

struct Complex
{
    Complex() {
        real = 0.0;
        imag = 0.0;
    }

    Complex( double r, double i = 0.0 ) {
        real = r;
        imag = i;
    }

    double real;
    double imag;
};

Complex operator * ( const Complex & c1, const Complex & c2 );

Complex operator * ( double a, const Complex & c1 ) ;

Complex operator / ( const Complex & c1, const Complex & c2 );

Complex operator + ( const Complex & c1, const Complex & c2 );

Complex operator - ( const Complex & c1, const Complex & c2 );

Complex operator - ( const Complex & c1 ) ;

Complex csqrt( const Complex & c1 );

#endif // COMPLEX_H_
