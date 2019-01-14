#ifndef DCE_74153_H_
#define DCE_74153_H_

/*
    Dual 4-line to 1-line multiplexer.
*/
class D74153 : public DChip
{
public:
    D74153() : DChip(16) {
        //  1: Ea -> (negated) enable output a
        //  2: S1 -> select input
        //  3: I3a -> input a
        //  4: I2a -> input a
        //  5: I1a -> input a
        //  6: I0a -> input a
        connectLine(  7, ya_ ); // Ya, output
        connectLine(  8, DLine::Ground );
        connectLine(  9, yb_ ); // Yb, output
        // 10: I0b -> input b
        // 11: I1b -> input b
        // 12: I2b -> input b
        // 13: I3b -> input b
        // 14: S0 -> select input
        // 15: Eb -> (negated) enable output b
        connectLine( 16, DLine::Power );

        ya_ = 0;
        yb_ = 0;
    }

    void update() {
        unsigned s = (line(2) << 1) | line(14); // S1-S0

        ya_ = doMultiplex( s, line( 1), line( 6), line( 5), line( 4), line( 3) );
        yb_ = doMultiplex( s, line(15), line(10), line(11), line(12), line(13) );
    }

private:
    DBit doMultiplex( unsigned s, DBit enable, DBit i0, DBit i1, DBit i2, DBit i3 ) {
        DBit result = 0;

        if( ! enable ) {
            if( s == 0 )
                result = i0;
            else if( s == 1 )
                result = i1;
            else if( s == 2 )
                result = i2;
            else
                result = i3;
        }

        return result;
    }

    DBit    ya_;
    DBit    yb_;
};

#endif // DCE_74153_H_
