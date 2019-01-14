#ifndef DCE_7430_H_
#define DCE_7430_H_

/*
    8-input positive NAND gate.
*/
class D7430 : public DChip
{
public:
    D7430() : DChip(14) {
        //  1: A -> input
        //  2: B -> input
        //  3: C -> input
        //  4: D -> input
        //  5: E -> input
        //  6: F -> input
        connectLine(  7, DLine::Ground );
        connectLine(  8, y_ ); // Y, output
        //  9: NC -> not connected
        // 10: NC -> not connected
        // 11: G -> input
        // 12: H -> input
        // 13: NC -> not connected
        connectLine( 14, DLine::Power );

        y_ = 0;
    }

    void update() {
        y_ = 1 ^ (line( 1) & line( 2) & line( 3) & line( 4) & line( 5) & line( 6) & line(11) & line(12));
    }

private:
    DBit    y_;
};

#endif // DCE_7430_H_
