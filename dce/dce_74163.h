#ifndef DCE_74163_H_
#define DCE_74163_H_

/*
    Presettable synchronous 4-bit binary counter, synchronous reset.
*/
class D74163 : public DChip
{
public:
    D74163() : DChip(16) {
        //  1: CLEAR, active low
        //  2: CLOCK
        //  3: A -> input (for the preset function)
        //  4: B -> input (for the preset function)
        //  5: C -> input (for the preset function)
        //  6: D -> input (for the preset function)
        //  7: ENABLE P (count enable)
        connectLine(  8, DLine::Ground );
        //  9: LOAD (or PE, Parallel Enable input), active low
        // 10: ENABLE T (count enable carry)
        connectLine( 11, qd_ ); // Qd, output bit 3
        connectLine( 12, qc_ ); // Qc, output bit 2
        connectLine( 13, qb_ ); // Qb, output bit 1
        connectLine( 14, qa_ ); // Qa, output bit 0
        connectLine( 15, tc_ ); // Carry (or TC, Terminal Count)
        connectLine( 16, DLine::Power );

        qa_ = 0;
        qb_ = 0;
        qc_ = 0;
        qd_ = 0;
        tc_ = 0;
        last_clock_ = 0;
    }

    // TODO: check carry (TC) behavior

    void update() {
        if( line( 2) && (last_clock_ == 0) ) {
            // Positive clock edge
            if( line( 1) ) {
                if( line( 9) ) {
                    if( line( 7) && line(10) ) {
                        // Count
                        qa_ ^= 1;
                        if( qa_ == 0 ) {
                            qb_ ^= 1;
                            if( qb_ == 0 ) {
                                qc_ ^= 1;
                                if( qc_ == 0 ) {
                                    qd_ ^= 1;
                                }
                            }
                        }
                    }
                }
                else {
                    // Load
                    qa_ = line( 3);
                    qb_ = line( 4);
                    qc_ = line( 5);
                    qd_ = line( 6);
                }

                tc_ = qa_ & qb_ & qc_ & qd_ & line(10);
            }
            else {
                // Clear
                qa_ = 0;
                qb_ = 0;
                qc_ = 0;
                qd_ = 0;
                tc_ = 0;
            }
        }
        else {
            // Hold
            tc_ &= line(10); // Mask off carry if not enabled
        }

        last_clock_ = line( 2);
    }

private:
    DBit    qa_;
    DBit    qb_;
    DBit    qc_;
    DBit    qd_;
    DBit    tc_;
    DBit    last_clock_;
};

#endif // DCE_74163_H_
