#ifndef DCE_7402_H_
#define DCE_7402_H_

/*
    Quadruple 2-input positive NOR gate.
*/
class D7402 : public DChip
{
public:
    D7402() : DChip(14) {
        connectLine(  1, y1_ ); // 1Y, output of port #1
        //  2: 1A -> input for port #1
        //  3: 1B -> input for port #1
        connectLine(  4, y2_ ); // 2Y, output of port #2
        //  5: 2A -> input for port #2
        //  6: 2B -> input for port #2
        connectLine(  7, DLine::Ground );
        //  8: 3A -> input for port #3
        //  9: 3B -> input for port #3
        connectLine( 10, y3_ ); // 3Y, output of port #3
        // 11: 4A -> input for port #4
        // 12: 4B -> input for port #4
        connectLine( 13, y4_ ); // 4Y, output of port #4
        connectLine( 14, DLine::Power );

        y1_ = 0;
        y2_ = 0;
        y3_ = 0;
        y4_ = 0;
    }

    void update() {
        y1_ = 1 ^ (line( 2) | line( 3));
        y2_ = 1 ^ (line( 5) | line( 6));
        y3_ = 1 ^ (line( 8) | line( 9));
        y4_ = 1 ^ (line(11) | line(12));
    }

private:
    DBit    y1_;
    DBit    y2_;
    DBit    y3_;
    DBit    y4_;
};

#endif // DCE_7402_H_
