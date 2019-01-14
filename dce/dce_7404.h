#ifndef DCE_7404_H_
#define DCE_7404_H_

/*
    Hex inverter.
*/
class D7404 : public DChip
{
public:
    D7404() : DChip(14) {
        //  1: 1A -> input for inverter #1
        connectLine(  2, y1_ ); // 1Y, output of inverter #1
        //  3: 2A -> input for inverter #2
        connectLine(  4, y2_ ); // 2Y, output of inverter #2
        //  5: 3A -> input for inverter #3
        connectLine(  6, y3_ ); // 3Y, output of inverter #3
        connectLine(  7, DLine::Ground );
        connectLine(  8, y4_ ); // 4Y, output of inverter #4
        //  9: 4A -> input for inverter #4
        connectLine( 10, y5_ ); // 5Y, output of inverter #5
        // 11: 5A -> input for inverter #5
        connectLine( 12, y6_ ); // 6Y, output of inverter #6
        // 13: 6A -> input for inverter #6
        connectLine( 14, DLine::Power );

        y1_ = 0;
        y2_ = 0;
        y3_ = 0;
        y4_ = 0;
        y5_ = 0;
        y6_ = 0;
    }

    void update() {
        y1_ = 1 ^ line( 1);
        y2_ = 1 ^ line( 3);
        y3_ = 1 ^ line( 5);
        y4_ = 1 ^ line( 9);
        y5_ = 1 ^ line(11);
        y6_ = 1 ^ line(13);
    }

private:
    DBit    y1_;
    DBit    y2_;
    DBit    y3_;
    DBit    y4_;
    DBit    y5_;
    DBit    y6_;
};

#endif // DCE_7404_H_
