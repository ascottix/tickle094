#ifndef DCE_74193_H_
#define DCE_74193_H_

/*
    Presettable 4-bit binary up/down counter.
*/
class D74193 : public DChip
{
public:
    D74193() : DChip(16) {
        //  1: P1 -> parallel data input (for the preset function)
        connectLine(  2, q1_ ); // Q1, output bit 1
        connectLine(  3, q0_ ); // Q0, output bit 0
        //  4: CPD -> count down clock pulse
        //  5: CPU -> count up clock pulse
        connectLine(  6, q2_ ); // Q2, output bit 2
        connectLine(  7, q3_ ); // Q3, output bit 3
        connectLine(  8, DLine::Ground );
        //  9: P3 -> parallel data input (for the preset function)
        // 10: P2 -> parallel data input (for the preset function)
        // 11: PL -> asynchronous parallel load (active LOW)
        connectLine( 12, tcu_ ); // Terminal count up (carry), negated
        connectLine( 13, tcd_ ); // Terminal count down (borrow), negated
        // 14: MR -> asynchronous master reset (clear)
        // 15: P0 -> parallel data input (for the preset function)
        connectLine( 16, DLine::Power );

        q0_ = 0;
        q1_ = 0;
        q2_ = 0;
        q3_ = 0;
        tcd_ = 1;
        tcu_ = 1;
        last_clock_up_ = 0;
        last_clock_down_ = 0;
    }

    void update() {
        if( line(14) ) {
            // Clear
            q0_ = 0;
            q1_ = 0;
            q2_ = 0;
            q3_ = 0;
            tcd_ = 1;
            tcu_ = 0;
        }
        else if( ! line(11) ) {
            // Parallel load
            q0_ = line(15);
            q1_ = line( 1);
            q2_ = line(10);
            q3_ = line( 9);
        }
        else if( ! last_clock_up_ && line(5) ) {
            // Count up
            q0_ ^= 1;
            if( q0_ == 0 ) {
                q1_ ^= 1;
                if( q1_ == 0 ) {
                    q2_ ^= 1;
                    if( q2_ == 0 ) {
                        q3_ ^= 1;
                    }
                }
            }
        }
        else if( ! last_clock_down_ && line(4) ) {
            // Count down
            q0_ ^= 1;
            if( q0_) {
                q1_ ^= 1;
                if( q1_ ) {
                    q2_ ^= 1;
                    if( q2_ ) {
                        q3_ ^= 1;
                    }
                }
            }
        }

        last_clock_up_ = line( 5);
        last_clock_down_ = line( 4);

        tcu_ = q0_ & q1_ & q2_ & q3_ & (1 ^ last_clock_up_);
        tcd_ = 1 ^ (q0_ | q1_ | q2_ | q3_ | last_clock_down_);
    }

private:
    DBit    q0_;
    DBit    q1_;
    DBit    q2_;
    DBit    q3_;
    DBit    tcu_;
    DBit    tcd_;
    DBit    last_clock_up_;
    DBit    last_clock_down_;
};

#endif // DCE_74193_H_
