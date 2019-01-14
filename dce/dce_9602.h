#ifndef DCE_9602_H_
#define DCE_9602_H_

/*
    Dual retriggerable resettable one-shot.

    Note: same as 74123, but different pin layout.
*/
class D9602 : public DChip
{
public:
    D9602() : DChip(16) {
        //  1: Cext1 -> external capacitor for one-shot #1
        //  2: Rext1 -> external resistor for one-shot #1
        //  3: CLR1 -> clear one-shot #1, active low
        //  4: B1 -> trigger for one-shot #1, active high
        //  5: A1 -> trigger for one-shot #1, active low
        connectLine(  6, q1_ );
        connectLine(  7, not_q1_ );
        connectLine(  8, DLine::Ground );
        connectLine(  9, not_q2_ );
        connectLine( 10, q2_ );
        // 11: A2 -> trigger for one-shot #2, active low
        // 12: B2 -> trigger for one-shot #2, active high
        // 13: CLR2 -> clear one-shot #1, active low
        // 14: Rext2 -> external resistor for one-shot #2
        // 15: Cext2 -> external capacitor for one-shot #2
        connectLine( 16, DLine::Power );

        q1_ = 0;
        q2_ = 0;
        not_q1_ = 1 ^ q1_;
        not_q2_ = 1 ^ q2_;
        last_input_1_ = 0;
        last_input_2_ = 0;
    }

    void update() {
        DBit input1 = line( 4) | (1 ^ line( 5));
        DBit input2 = line(12) | (1 ^ line(11));

        // TODO!!!

        not_q1_ = 1 ^ q1_;
        not_q2_ = 1 ^ q2_;

        last_input_1_ = input1;
        last_input_2_ = input2;
    }

private:
    DBit    q1_;
    DBit    not_q1_;
    DBit    q2_;
    DBit    not_q2_;
    DBit    last_input_1_;
    DBit    last_input_2_;
};

#endif // DCE_9602_H_
