#ifndef DCE_7474_H_
#define DCE_7474_H_

/*
    Dual D-type positive-edge triggered flip-flop with preset and clear.
*/
class D7474 : public DChip
{
public:
    D7474() : DChip(14) {
        //  1: 1CLR -> clear for flip-flop #1 (negated)
        //  2: 1D -> data for flip-flop #1
        //  3: 1CLK -> clock for flip-flop #1
        //  4: 1PRE -> preset for flip-flop #1 (negated)
        connectLine(  5, q1_ ); // 1Q, output of flip-flop #1
        connectLine(  6, q1_not_ ); // NOT 1Q, output of flip-flop #1
        connectLine(  7, DLine::Ground );
        connectLine(  8, q2_not_ ); // NOT 2Q, output of flip-flop #2
        connectLine(  9, q2_ ); // 2Q, output of flip-flop #2
        // 10: 2PRE -> preset for flip-flop #2 (negated)
        // 11: 2CLK -> clock for flip-flop #2
        // 12: 2D -> data for flip-flop #2
        // 13: 2CLR -> clear for flip-flop #2 (negated)
        connectLine( 14, DLine::Power );

        q1_ = 0;
        q1_not_ = 0;
        last_clk1_ = 0;
        q2_ = 0;
        q2_not_ = 0;
        last_clk2_ = 0;
    }

    void update() {
        doFlipFlop( q1_, q1_not_, line( 4), line( 1), line( 2), line( 3), last_clk1_ );
        doFlipFlop( q2_, q2_not_, line(10), line(13), line(12), line(11), last_clk2_ );

        last_clk1_ = line( 3);
        last_clk2_ = line(11);
    }

private:
    void doFlipFlop( DBit & q, DBit & q_not, DBit pre, DBit clr, DBit d, DBit clk, DBit last_clk ) {
        if( !pre && !clr ) {
            // Both outputs are high (nonstable configuration)
            q = 1;
            q_not = 1;
        }
        else if( pre && !clr ) {
            // Clear
            q = 0;
            q_not = 1;
        }
        else if( !pre && clr ) {
            // Preset
            q = 1;
            q_not = 0;
        }
        else {
            // If on positive clock edge, transfer data to output
            if( clk && !last_clk ) {
                q = d;
                q_not = 1 ^ q;
            }
        }
    }

    DBit    q1_;
    DBit    q1_not_;
    DBit    last_clk1_;
    DBit    q2_;
    DBit    q2_not_;
    DBit    last_clk2_;
};

#endif // DCE_7474_H_
