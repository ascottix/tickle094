#ifndef DCE_PROBE_H_
#define DCE_PROBE_H_

#include "dce.h"

class DProbe
{
public:
    DProbe() {
        state_ = 0;
    }

    void connect( unsigned bit, DChip & chip, unsigned line ) {
        lines_[ bit ].connectTo( chip.getLine(line) );
    }

    void connect( unsigned bit, DLine & line ) {
        lines_[ bit ].connectTo( line );
    }

    unsigned update() {
        state_ = 0;

        for( int i=0; i<MaxLines; i++ ) {
            if( lines_[i].state() ) {
                state_ |= 1 << i;
            }
        }

        return state_;
    }

    unsigned state() const {
        return state_;
    }

private:
    enum {
        MaxLines = 8*sizeof(unsigned) // Portable enough...
    };

    DLine lines_[MaxLines];
    unsigned state_;
};

#endif // DCE_PROBE_H_
