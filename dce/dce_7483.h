#ifndef DCE_7483_H_
#define DCE_7483_H_

/*
    4-bit binary adder with fast carry.
*/
class D7483 : public DChip
{
public:
    D7483() : DChip(16) {
        //  1: A4
        connectLine(  2, y3_ ); // 3Y, bit 2 of sum
        //  3: A3
        //  4: B3
        connectLine(  5, DLine::Power );
        connectLine(  6, y2_ ); // 2Y, bit 1 of sum
        //  7: B2
        //  8: A2
        connectLine(  9, y1_ ); // 1Y, bit 0 of sum
        // 10: A1
        // 11: B1
        connectLine( 12, DLine::Ground );
        // 13: C0 -> input carry
        connectLine( 14, c4_ ); // Carry
        connectLine( 15, y4_ ); // 4Y, bit 3 of sum
        // 16: B4

        y1_ = 0;
        y2_ = 0;
        y3_ = 0;
        y4_ = 0;
        c4_ = 0;
    }

    void update() {
        unsigned a = line(10) | (line( 8) << 1) | (line( 3) << 2) | (line( 1) << 3); 
        unsigned b = line(11) | (line( 7) << 1) | (line( 4) << 2) | (line(16) << 3);

        unsigned s = a + b + line(13);

        y1_ = (s     ) & 1;
        y2_ = (s >> 1) & 1;
        y3_ = (s >> 2) & 1;
        y4_ = (s >> 3) & 1;
        c4_ = (s >> 4) & 1;
    }

private:
    DBit    y1_;
    DBit    y2_;
    DBit    y3_;
    DBit    y4_;
    DBit    c4_;
};

#endif // DCE_7483_H_
