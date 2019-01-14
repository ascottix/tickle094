#ifndef DCE_74107_H_
#define DCE_74107_H_

/*
    Dual J-K flip-flop with clear.
*/
class D74107 : public DChip
{
public:
    D74107() : DChip(14) {
        //  1: 1J -> J input for flip-flop #1
        connectLine(  2, q1_not_ ); // NOT 1Q, output of flip-flop #1
        connectLine(  3, q1_ ); // 1Q, output of flip-flop #1
        //  4: 1K -> K input for flip-flop #1
        connectLine(  5, q2_ ); // 2Q, output of flip-flop #2
        connectLine(  6, q2_not_ ); // NOT 2Q, output of flip-flop #2
        connectLine(  7, DLine::Ground );
        //  8: 2J -> J input for flip-flop #2
        //  9: 2CLK -> clock for flip-flop #2
        // 10: 2CLR -> clear for flip-flop #2 (negated)
        // 11: 2K -> K input for flip-flop #2
        // 12: 1CLK -> clock for flip-flop #1
        // 13: 1CLR -> clear for flip-flop #1 (negated)
        connectLine( 14, DLine::Power );

        q1_ = 0;
        q1_not_ = 1;
        last_clk1_ = 0;
        q2_ = 0;
        q2_not_ = 1;
        last_clk2_ = 0;
    }

    void update() {
        // Flip-flop #1
        if( ! line(13) ) {
            // Clear
            q1_ = 0;
        }
        else if( last_clk1_ && !line(12) ) {
            q1_ = doFlipFlop( q1_, line( 1), line( 4) );
        }

        last_clk1_ = line(12);

        // Flip-flop #2
        if( ! line(10) ) {
            // Clear
            q2_ = 0;
        }
        else if( last_clk2_ && !line( 9) ) {
            q2_ = doFlipFlop( q2_, line( 8), line(11) );
        }

        last_clk2_ = line( 9);

        // Generate negative output too
        q1_not_ = 1 ^ q1_;
        q2_not_ = 1 ^ q2_;
    }

private:
    DBit doFlipFlop( DBit q, DBit j, DBit k ) {
        if( j ^ k ) { // J and K are different
            q = j;
        }
        else if( j ) { // J and K are both high
            q ^= 1;
        }

        return q;
    }

    DBit    q1_;
    DBit    q1_not_;
    DBit    last_clk1_;
    DBit    q2_;
    DBit    q2_not_;
    DBit    last_clk2_;
};

#endif // DCE_74107_H_
