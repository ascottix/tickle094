#ifndef DCE_7408_H_
#define DCE_7408_H_

/*
    Quadruple 2-input positive AND gate.
*/
class D7408 : public DChip
{
public:
    D7408() : DChip(14) {
        //  1: 1A -> input for port #1
        //  2: 1B -> input for port #1
        connectLine(  3, y1_ ); // 1Y, output of port #1
        //  4: 2A -> input for port #2
        //  5: 2B -> input for port #2
        connectLine(  6, y2_ ); // 2Y, output of port #2
        connectLine(  7, DLine::Ground );
        connectLine(  8, y3_ ); // 3Y, output of port #3
        //  9: 3A -> input for port #3
        // 10: 3B -> input for port #3
        connectLine( 11, y4_ ); // 4Y, output of port #4
        // 12: 4A -> input for port #4
        // 13: 4B -> input for port #4
        connectLine( 14, DLine::Power );

        y1_ = 0;
        y2_ = 0;
        y3_ = 0;
        y4_ = 0;
    }

    void update() {
        y1_ = line( 1) & line( 2);
        y2_ = line( 4) & line( 5);
        y3_ = line( 9) & line(10);
        y4_ = line(12) & line(13);
    }

private:
    DBit    y1_;
    DBit    y2_;
    DBit    y3_;
    DBit    y4_;
};

#endif // DCE_7408_H_
