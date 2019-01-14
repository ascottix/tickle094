#ifndef DCE_7490_H_
#define DCE_7490_H_

#include "dce.h"

/*
    Decade counter.

    BCD count sequence: clock on input A, connect Qa to input B, read Qd,Qc,Qb,Qa.
    Bi-quinary (5-2) count sequence: clock on input B, connect Qd on input A, read Qa,Qd,Qc,Qb.
*/
class D7490 : public DChip
{
public:
    D7490() : DChip( 14 ) {
        //  1: INPUT A -> clock for Qa (high to low transition)
        //  2: R0(1) -> reset to 0 (1)
        //  3: R0(2) -> reset to 0 (2)
        //  4: NC -> unconnected
        connectLine(  5, DLine::Power );
        //  6: R9(1) -> reset to 9 (1)
        //  7: R9(2) -> reset to 9 (1)
        connectLine(  8, qc_ ); // Qc
        connectLine(  9, qb_ ); // Qb
        connectLine( 10, DLine::Ground );
        connectLine( 11, qd_ ); // Qd
        connectLine( 12, qa_ ); // Qa
        // 13: NC -> unconnected
        // 14: INPUT B -> clock for Qb (cascaded to Qc and Qd, high to low transition)

        last_input_a_ = 0;
        last_input_b_ = 0;
        qa_ = 0;
        qb_ = 0;
        qc_ = 0;
        qd_ = 0;
    }

    void update() {
        if( line(2) && line(3) ) {
            // Reset to 0
            qa_ = 0;
            qb_ = 0;
            qc_ = 0;
            qd_ = 0;
        }
        else if( line(6) && line(7) ) {
            // Reset to 9
            qa_ = 1;
            qb_ = 0;
            qc_ = 0;
            qd_ = 1;
        }
        else {
            // Count
            if( last_input_a_ && !line(14) ) {
                qa_ ^= 1;
            }

            if( last_input_b_ && !line( 1) ) {
                qb_ ^= 1;
                if( qb_ == 0 ) {
                    qc_ ^= 1;
                    if( qc_ == 0 ) {
                        qd_ ^= 1;
                    }
                }

                if( qd_ && !qc_ && qb_ && !qa_ ) {
                    // Wrap around at ten
                    qd_ = 0;
                    qb_ = 0;
                }
            }
        }

        last_input_a_ = line(14);
        last_input_b_ = line( 1);
    }

    // For debugging
    void set( unsigned n ) {
        qa_ = n & 1;
        qb_ = n & 2;
        qc_ = n & 4;
        qd_ = n & 8;
    }

private:
    DBit    last_input_a_;
    DBit    last_input_b_;
    DBit    qa_;
    DBit    qb_;
    DBit    qc_;
    DBit    qd_;
};

#endif // DCE_7490_H_
