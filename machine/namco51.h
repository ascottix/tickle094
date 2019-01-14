/*
    Namco 51xx custom chip emulator

    Copyright (c) 2011 Alessandro Scotti
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
#ifndef NAMCO_51XX_
#define NAMCO_51XX_

struct Namco5xCustom
{
    virtual unsigned char read() = 0;
    virtual void write( unsigned char b ) = 0;
};

struct Namco51xx : public Namco5xCustom
{
    Namco51xx( unsigned char * port0, unsigned char * port1 );

    void reset();
    
    unsigned char read();
    void write( unsigned char b );
    
    int credits_;
    int coins_[2];
    int creds_per_coin_[2];
    int coins_per_cred_[2];
    int data_count_;
    int mode_;
    unsigned char * port0_;
    unsigned char * port1_;
    unsigned char o_port0_;
    
private:
    Namco51xx();
};

#endif // NAMCO_51XX_
