/*
    I8080 emulator

    Copyright (c) 1996-2003,2004 Alessandro Scotti
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
#include "i8080.h"

I8080::I8080( I8080Environment & env )
    : env_( env )
{
    reset();
}

void I8080::reset()
{
    B = 0; 
    C = 0;
    D = 0; 
    E = 0;
    H = 0;
    L = 0;
    A = 0;
    F = 0;
    PC = 0;
    SP = 0xF000;

    iflags_ = 0;
    cycles_ = 0;
    t_cycles_ = 0;
}

void I8080::step()
{
    cycles_ = 0;
    
    if( iflags_ & FlagHalted ) {
        // CPU is halted, do a NOP instruction
        cycles_ += Opcode_[0].cycles; // NOP
    }
    else {
        unsigned op = env_.readByte( PC++ );

        // Execute
        cycles_ += Opcode_[ op ].cycles;

        if( Opcode_[ op ].handler ) {
            (this->*(Opcode_[ op ].handler))();
        }
    }
    
    t_cycles_ += cycles_;

    PC &= 0xFFFF;
    SP &= 0xFFFF; // Clip stack pointer
}

unsigned I8080::run( unsigned runCycles )
{
    int remainingCycles = (int) runCycles;
    
    // Execute instructions until the specified number of cycles has elapsed
    while( remainingCycles > 0 ) {
        cycles_ = 0;
        
        if( iflags_ & FlagHalted ) {
            // CPU is halted, do NOPs for the rest of cycles
            // (this may be off by a few cycles)
            cycles_ = remainingCycles;
        }
        else {
            unsigned op = env_.readByte( PC++ );

            cycles_ += Opcode_[ op ].cycles;

            if( Opcode_[ op ].handler ) {
                (this->*(Opcode_[ op ].handler))();
            }
        }
        
        t_cycles_ += cycles_;
        
        remainingCycles -= (int) cycles_;
    }

    // Update registers
    PC &= 0xFFFF; // Clip program counter
    SP &= 0xFFFF; // Clip stack pointer

    // Return the number of extra cycles executed
    return (unsigned) (-remainingCycles);
}

void I8080::interrupt( unsigned char opcode )
{
    if( iflags_ & FlagInterruptEnabled ) {
        cycles_ = 0;
        
        if( iflags_ & FlagHalted ) {
            iflags_ &= ~FlagHalted;
        }

        // Execute the 8-bit interrupt instruction provided on the data bus
        cycles_ += Opcode_[ opcode ].cycles;

        if( Opcode_[ opcode ].handler ) {
            (this->*(Opcode_[ opcode ].handler))();
        }
        
        t_cycles_ += cycles_;
    }
}
