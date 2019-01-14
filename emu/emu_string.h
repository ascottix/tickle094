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
#ifndef EMU_STRING_H_
#define EMU_STRING_H_

#include <wchar.h>

class TString
{
public:
    TString() : buf_(0), cstr_buf_(0) {
        assign( (wchar_t *)0 );
    }

    TString( const char * s ) : buf_(0), cstr_buf_(0) {
        assign( s );
    }

    TString( const wchar_t * s ) : buf_(0), cstr_buf_(0) {
        assign( s );
    }

    TString( const TString & s ) : buf_(0), cstr_buf_(0) {
        assign( s.buf_ );
    }

    /** Destructor. */
    ~TString();

    TString & operator = ( const TString & s ) {
        if( this != &s ) {
            assign( s.buf_ );
    }
        return *this;
    }

    TString operator + ( const TString & s ) {
        TString result( *this );
        result += s;
        return result;
    }

    TString & append( wchar_t c );

    TString & append( const TString & s );

    TString & operator += ( const TString & s ) {
        return append( s );
    }

    void insert( const TString & s, int index );

    void remove( int index, int count = -1 );

    TString copy( int index, int count = -1 ) const;

    int pos( wchar_t c, int index = 0 ) const;

    int pos( const TString & s, int index = 0 ) const;

    int lastpos( wchar_t c ) const;

    int cmp( const TString & s ) const;

    int icmp( const TString & s ) const;

    bool operator == ( const TString & s ) const {
        return cmp(s) == 0;
    }

    size_t length() const {
        return len_;
    }

    const wchar_t * wstr() const {
        return buf_;
    }

    const char * cstr() const;

    long tol( int base = 10 ) const;

    unsigned long toul( int base = 10 ) const;

    static char * strdup( const char * s );
    
    static wchar_t * strdup( const wchar_t * s );

private:
    TString & assign( const char * s );
    TString & assign( const wchar_t * s );

    size_t len_;
    wchar_t * buf_;
    mutable char * cstr_buf_;
};

#endif // EMU_STRING_H_
