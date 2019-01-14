#ifndef DCE_555_MONOSTABLE_H_
#define DCE_555_MONOSTABLE_H_

#include <math.h>

/*
    555 timer in monostable connection.
*/
class D555_Monostable : public DChip
{
public:
    D555_Monostable() : DChip(8) {
        connectLine(  1, DLine::Ground );
        //  2: trigger
        connectLine(  3, output_ );
        //  4: reset
        //  5: control
        //  6: threshold
        //  7: discharge
        connectLine(  8, DLine::Power );

        clock_frequency_ = 0.0;
        last_trigger_ = 0;
        last_clock_ = 0;
        output_ = 0;
        vcc_ = 5.0;
    }

    void update() {
        if( ! line(4) ) {
            // Reset
            output_ = 0;
        }
        else {
            DBit clk = clock_.state();

            // Enabled
            if( output_ ) {
                // Timer is on, count until timeout is reached
                if( clk && ! last_clock_ ) {
                    current_count_++;
                    if( current_count_ >= timeout_count_ ) {
                        output_ = 0;
                    }
                }
            }
            else {
                // Timer is off, check for trigger
                if( last_trigger_ && (line(2) == 0) ) {
                    // Trigger the timer
                    current_count_ = 0;
                    output_ = 1;
                }
            }
            
            last_clock_ = clk;
        }

        last_trigger_ = line(2);
    }

    void setClock( DLine & clock, double frequency ) {
        clock_.connectTo( clock );
        clock_frequency_ = frequency;
    }

    void setResCap( double r, double c ) {
        res_ = r;
        cap_ = c;
        setControlVoltage( vcc_ * 2 / 3 );
    }

    void setControlVoltage( double v ) {
        /*
            Here we get the timer duration in seconds, then convert it
            into discrete steps as specified by the clock frequency.

            The voltage across a capacitor in a R-C circuit follows a
            decaying exponential curve and depends on the voltage applied
            to the circuit. When the capacitor starts with no charge, as
            in this case, the formula becomes:

                Vcap = Vcc * (1 - exp( -t / RC ))

            Thus we can solve for t:

                v = vcc * (1 - exp( -t / RC ))

                v / vcc = 1 - exp( -t / RC )

                1 - v / vcc = exp( -t / RC )

                ln( 1 - v / vcc ) = - t / RC

                t = - ln( 1 - v / vcc ) * RC

            In a typical configuration there would be no control voltage and 
            the threshold voltage becomes 2/3 * Vcc, so:

                T1 = - ln( 1 - 2 / 3 ) * RC = 1.1 * RC

            as in the datasheet.
        */
        double t = - log( 1 - v / vcc_ ) * res_ * cap_;

        timeout_count_ = (unsigned) (t * clock_frequency_);
    }

private:
    DBit        output_;
    DBit        last_trigger_;
    DBit        last_clock_;
    DLine       clock_;
    double      clock_frequency_;
    unsigned    timeout_count_;
    unsigned    current_count_;
    double      vcc_;
    double      res_;
    double      cap_;
};

#endif // DCE_555_MONOSTABLE_H_
