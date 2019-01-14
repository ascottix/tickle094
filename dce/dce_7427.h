#ifndef DCE_7427_H_
#define DCE_7427_H_

/*
    Triple 3-input positive NOR gate.
*/
class D7427 : public DChip
{
public:
    D7427() : DChip(14) {
        //  1: 1A -> input for port #1
        //  2: 1B -> input for port #1
        //  3: 2A -> input for port #2
        //  4: 2B -> input for port #2
        //  5: 2C -> input for port #2
        connectLine(  6, y2_ ); // 2Y, output of port #2
        connectLine(  7, DLine::Ground );
        connectLine(  8, y3_ ); // 3Y, output of port #3
        //  9: 3A -> input for port #3
        // 10: 3B -> input for port #3
        // 11: 3C -> input for port #3
        connectLine( 12, y1_ ); // 1Y, output of port #1
        // 13: 1C -> input for port #1
        connectLine( 14, DLine::Power );

        y1_ = 0;
        y2_ = 0;
        y3_ = 0;
    }

    void update() {
        y1_ = 1 ^ (line( 1) | line( 2) | line(13));
        y2_ = 1 ^ (line( 3) | line( 4) | line( 5));
        y3_ = 1 ^ (line( 9) | line(10) | line(11));
    }

private:
    DBit    y1_;
    DBit    y2_;
    DBit    y3_;
};

#endif // DCE_7427_H_
