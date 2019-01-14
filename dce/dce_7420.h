#ifndef DCE_7420_H_
#define DCE_7420_H_

/*
    Dual 4-input positive NAND gate.
*/
class D7420 : public DChip
{
public:
    D7420() : DChip(14) {
        //  1: A1 -> input for port #1
        //  2: B1 -> input for port #1
        //  3: NC -> not connected
        //  4: C1 -> input for port #1
        //  5: D1 -> input for port #1
        connectLine(  6, y1_ ); // Y1, output of port #1
        connectLine(  7, DLine::Ground );
        connectLine(  8, y2_ ); // Y2, output of port #2
        //  9: A2 -> input for port #2
        // 10: B2 -> input for port #2
        // 11: NC -> not connected
        // 12: C2 -> input for port #2
        // 13: D2 -> input for port #2
        connectLine( 14, DLine::Power );

        y1_ = 0;
        y2_ = 0;
    }

    void update() {
        y1_ = 1 ^ (line( 1) & line( 2) & line( 4) & line( 5));
        y2_ = 1 ^ (line( 9) & line(10) & line(12) & line(13));
    }

private:
    DBit    y1_;
    DBit    y2_;
};

#endif // DCE_7420_H_
