#ifndef DCE_7450_H_
#define DCE_7450_H_

/*
    Dual 2-wide 2-input AND-OR-INVERT gate (one gate expandable).
*/
class D7450 : public DChip
{
public:
    D7450() : DChip(14) {
        //  1: 1A -> input for port #1
        //  2: 2A -> input for port #2
        //  3: 2B -> input for port #2
        //  4: 2C -> input for port #2
        //  5: 2D -> input for port #2
        connectLine(  6, y2_ ); // 2Y, output of port #2
        connectLine(  7, DLine::Ground );
        connectLine(  8, y1_ ); // 1Y, output of port #1
        //  9: 1C -> input for port #1
        // 10: 1D -> input for port #1
        // 11: 1X -> expander for port #1
        // 12: NOT 1X -> expander for port #1
        // 13: 1B -> input for port #1
        connectLine( 14, DLine::Power );

        y1_ = 0;
        y2_ = 0;
    }

    void update() {
        // TODO: expander not implemented

        y1_ = 1 ^ ( (line( 1) & line(13)) | (line( 9) & line(10)) );
        y2_ = 1 ^ ( (line( 2) & line( 3)) | (line( 4) & line( 5)) );
    }

private:
    DBit    y1_;
    DBit    y2_;
};

#endif // DCE_7450_H_
