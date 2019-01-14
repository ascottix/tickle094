#ifndef DCE_7448_H_
#define DCE_7448_H_

/*
    BCD-to-seven-segments decoder.

    Segment layout:

         a
       -----
      |     |
    f |     | b
      |  g  |
       -----
      |     |
    e |     | c
      |     |
       -----
         d

    Note: the only difference with model 74LS248 is that the latter 
    composes the "6" and "9" with the "tail".

*/
class D7448 : public DChip
{
public:
    D7448() : DChip(16 ) {
        //  1: B -> input bit 1
        //  2: C -> input bit 2
        //  3: LT -> lamp test (negated)
        //  4: BI/RBO -> blanking input/ripple blanking output (negated)
        //  5: RBI -> ripple blanking input (negated)
        //  6: D -> input bit 3
        //  7: A -> input bit 0
        connectLine(  8, DLine::Ground );
        connectLine(  9, segment_[4] ); // Segment e, output
        connectLine( 10, segment_[3] ); // Segment d, output
        connectLine( 11, segment_[2] ); // Segment c, output
        connectLine( 12, segment_[1] ); // Segment b, output
        connectLine( 13, segment_[0] ); // Segment a, output
        connectLine( 14, segment_[6] ); // Segment g, output
        connectLine( 15, segment_[5] ); // Segment f, output
        connectLine( 16, DLine::Power );

        setAllSegmentsTo( 0 );
    }

    void update() {
        static DBit NumberToSegment[] = {
            1, 1, 1, 1, 1, 1, 0,    // 0
            0, 1, 1, 0, 0, 0, 0,    // 1
            1, 1, 0, 1, 1, 0, 1,    // 2
            1, 1, 1, 1, 0, 0, 1,    // 3
            0, 1, 1, 0, 0, 1, 1,    // 4
            1, 0, 1, 1, 0, 1, 1,    // 5
            0, 0, 1, 1, 1, 1, 1,    // 6
            1, 1, 1, 0, 0, 0, 0,    // 7
            1, 1, 1, 1, 1, 1, 1,    // 8
            1, 1, 1, 0, 0, 1, 1,    // 9
            0, 0, 0, 1, 1, 0, 1,    // 10
            0, 0, 1, 1, 0, 0, 1,    // 11
            0, 1, 0, 0, 0, 1, 1,    // 12
            1, 0, 0, 1, 0, 1, 1,    // 13
            0, 0, 0, 1, 1, 1, 1,    // 14
            0, 0, 0, 0, 0, 0, 0,    // 15
        };

        if( line(4) ) { // BI/RBO
            if( !getLine(3).isConnected() || line(3) ) { // LT
                // Get number at input
                int n = line(7) | (line(1) << 1) | (line(2) << 2) | (line(6) << 3);

                if( (n == 0) && getLine(5).isConnected() && (line(5) == 0) ) {
                    // Blank output for zero if RBI is low
                    setAllSegmentsTo( 0 );

                }
                else {
                    // Copy segment configuration from pre-defined array
                    n *= 7;

                    for( int i=0; i<7; i++ ) {
                        segment_[i] = NumberToSegment[n+i];
                    }
                }
            }
            else {
                // Lamp test, light all segments
                setAllSegmentsTo( 1 );
            }
        }
        else {
            // Blank input
            setAllSegmentsTo( 0 );
        }
    }

private:
    void setAllSegmentsTo( DBit bit ) {
        for( int i=0; i<7; i++ ) {
            segment_[i] = bit;
        }
    }

    DBit    segment_[7];
};

#endif // DCE_7448_H_
