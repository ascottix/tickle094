/*
    N6502 emulator

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
#include "n6502.h"

N6502::N6502( N6502Environment & env )
    : env_( env )
{
    reset();
}

void N6502::reset()
{
    A = 0;
    X = 0;
    Y = 0;
    F = Zero | Bit5; // TODO: should start with IrqDisabled?
    S = 0xFF;
    PC = env_.readWord( 0xFFFC );

    cycles_ = 0;
    t_cycles_ = 0;
}

void N6502::step()
{
    cycles_ = 0;

    unsigned op = nextByte();

    // Execute
    cycles_ += Opcode_[ op ].cycles;

    if( Opcode_[ op ].handler ) {
        (this->*(Opcode_[ op ].handler))();
    }
    
    t_cycles_ += cycles_;

    PC &= 0xFFFF;
}

unsigned N6502::run( unsigned runCycles )
{
    int remainingCycles = (int) runCycles;

    // Execute instructions until the specified number of cycles has elapsed
    while( remainingCycles > 0 ) {
        cycles_ = 0;
    
        unsigned op = env_.readByte( PC++ );

        cycles_ += Opcode_[ op ].cycles;

        if( Opcode_[ op ].handler ) {
            (this->*(Opcode_[ op ].handler))();
        }
        
        t_cycles_ += cycles_;
        
        remainingCycles -= (int) cycles_;
    }

    // Update registers
    PC &= 0xFFFF; // Clip program counter

    // Return the number of extra cycles executed
    return (unsigned) (-remainingCycles);
}

void N6502::interrupt( int type )
{
    if( (type == Int_NMI) || ((F & IrqDisabled) == 0) ) {
        cycles_ = 7;

        pushWord( PC );
        pushByte( F & ~Break );

        F &= ~DecimalMode;

        if( type == Int_NMI ) {
            PC = 0xFFFA;
        }
        else {
            PC = 0xFFFE;
            F |= IrqDisabled;
        }

        PC = nextWord();
        
        t_cycles_ += cycles_;
    }
}
