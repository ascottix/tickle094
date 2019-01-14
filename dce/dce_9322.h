#ifndef DCE_9322_H_
#define DCE_9322_H_

/*
    Quadruple 2-line to 1-line data selector/multiplexer.
*/
class D9322 : public DChip
{
public:
    D9322() : DChip(16) {
        //  1: SELECT
        //  2: A1 -> input for port #1
        //  3: B1 -> input for port #1
        connectLine(  4, y1_ ); // 1Y, output of port #1
        //  5: A2 -> input for port #2
        //  6: B2 -> input for port #2
        connectLine(  7, y2_ ); // 2Y, output of port #2
        connectLine(  8, DLine::Ground );
        connectLine(  9, y3_ ); // 3Y, output of port #3
        // 10: B3 -> input for port #3
        // 11: A3 -> input for port #3
        connectLine( 12, y4_ ); // 4Y, output of port #4
        // 13: B4 -> input for port #4
        // 14: A4 -> input for port #4
        // 15: STROBE
        connectLine( 16, DLine::Power );

        y1_ = 0;
        y2_ = 0;
        y3_ = 0;
        y4_ = 0;
    }

    void update() {
        if( line(15) ) {
            // All outputs low on strobe
            y1_ = 0;
            y2_ = 0;
            y3_ = 0;
            y4_ = 0;
        }
        else {
            DBit    select = line( 1);

            y1_ = select ? line( 3) : line( 2);
            y2_ = select ? line( 6) : line( 5);
            y3_ = select ? line(10) : line(11);
            y4_ = select ? line(13) : line(14);
        }
    }

private:
    DBit    y1_;
    DBit    y2_;
    DBit    y3_;
    DBit    y4_;
};

#endif // DCE_9322_H_
