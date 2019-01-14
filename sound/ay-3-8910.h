/*
    AY-3-8910 sound chip emulator

    Copyright (c) 2004 Alessandro Scotti
    http://www.ascotti.org/

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef AY_3_8910_H_
#define AY_3_8910_H_

class AY_3_8910
{
public:
    /** Hardware */
    enum {
        MinClock        = 1000000,  //@- Minimum chip clock in Hz
        MaxClock        = 2500000,  //@- Maximum chip clock in Hz
        NumRegisters    = 16,       //@- Number of registers
        NumChannels     = 3         //@- Number of channels
    };

    /** Registers */
    enum {
        TonePeriodFineA     = 0,    //@- R0
        TonePeriodCoarseA   = 1,    //@- R1
        TonePeriodFineB     = 2,    //@- R2
        TonePeriodCoarseB   = 3,    //@- R3
        TonePeriodFineC     = 4,    //@- R4
        TonePeriodCoarseC   = 5,    //@- R5
        NoisePeriod         = 6,    //@- R6
        Mixer               = 7,    //@- R7
        AmplitudeA          = 8,    //@- R10 (R8 for the YM2149)
        AmplitudeB          = 9,    //@- R11 (R9 for the YM2149)
        AmplitudeC          = 10,   //@- R12 (RA for the YM2149)
        EnvelopePeriodFine  = 11,   //@- R13 (RB for the YM2149)
        EnvelopePeriodCoarse= 12,   //@- R14 (RC for the YM2149)
        EnvelopeShape       = 13,   //@- R15 (RD for the YM2149)
        PortA               = 14,   //@- R16 (RE for the YM2149)
        PortB               = 15    //@- R17 (RF for the YM2149)
    };

    /** Mixer */
    enum {
        MixerToneA      = 0x01,     //@- 0=channel enabled, 1=channel disabled
        MixerToneB      = 0x02,     //@- 0=channel enabled, 1=channel disabled
        MixerToneC      = 0x04,     //@- 0=channel enabled, 1=channel disabled
        MixerNoiseA     = 0x08,     //@- 0=channel mixed with noise, 1=noise disabled
        MixerNoiseB     = 0x10,     //@- 0=channel mixed with noise, 1=noise disabled
        MixerNoiseC     = 0x20,     //@- 0=channel mixed with noise, 1=noise disabled
        MixerPortA      = 0x40,     //@- 0=port is input, 1=port is output
        MixerPortB      = 0x80,     //@- 0=port is input, 1=port is output
    };

    /**
        Constructor.

        @param clock master clock in Hertz
    */
    AY_3_8910( unsigned clock = MinClock );

    /** Destructor. */
    ~AY_3_8910() {
    }

    /**
        Sets the chip master clock.

        @param clock master clock in Hertz
    */
    void setClock( unsigned clock );

    /**
        Sets a register value.

        @param index register index
        @param value value to write into the register
    */
    void setRegister( unsigned index, unsigned char value );

    /** Returns the current value of the specified register. */
    unsigned char getRegister( unsigned index ) const {
        return (index < NumRegisters) ? reg_[index] : 0x00;
    }

    /** 
        Writes a register address into the internal address latch.

        The address store in the internal address latch is used for all
        subsequent read and write operations (until it is changed again
        with a call to this function.)

        @param address register address to store in internal latch

        @see #readData
        @see #writeData
    */
    void writeAddress( unsigned address ) {
        address_latch_ = address & 0x0F;
    }

    /**
        Sets the register currently being addressed by the internal latch.

        @param data data to be written into the register

        @see #writeAddress
    */
    void writeData( unsigned char data ) {
        setRegister( address_latch_, data );
    }

    /**
        Reads the register currently being addressed by the internal latch.

        @see #writeAddress
    */
    unsigned char readData() const {
        return reg_[address_latch_];
    }

    /** Resets the sound chip. */
    void reset();

    /**
        Lets the chip play for the specified amount of time.

        @param buffer buffer where sound output is mixed
        @param len length of buffer (in samples)
        @param samplingRate sampling rate (in Hz) at which output must be produced
    */
    void playSound( int * buffer, int len, unsigned samplingRate );

protected:
    unsigned getChannelPeriod( unsigned channel ) const {
        if( channel <= 2 )
            return reg_[channel*2] | (((unsigned)(reg_[channel*2+1] & 0x0F)) << 8);
        else if( channel == 3 )
            return reg_[NoisePeriod] & 0x1F;
        else if( channel == 4 )
            return getEnvelopePeriod();
        else
            return 0;
    }

    unsigned getChannelVolume( unsigned channel ) const {
        if( channel <= 2 )
            return reg_[AmplitudeA+channel] & 0x0F;
        else
            return 0;
    }

    bool isChannelUsingEnvelope( unsigned channel ) const {
        if( channel <= 2 )
            return reg_[AmplitudeA+channel] & 0x10 ? true : false;
        else
            return false;
    }

    unsigned getEnvelopePeriod() const {
        return reg_[EnvelopePeriodFine] | (((unsigned)reg_[EnvelopePeriodCoarse]) << 8);
    }

    unsigned getEnvelopeShape() const {
        return reg_[EnvelopeShape] & 0x0F;
    }

private:
    unsigned char reg_[NumRegisters]; // Registers
    unsigned char address_latch_; // Internal register address latch
    unsigned masterClock_; // Master clock
    unsigned soundClock_; // Clock used for sound generation (master/16)
    unsigned envelope_shape_counter_; // Counter for envelope shape
    unsigned noise_shift_register_; // Random generator for noise
    unsigned noise_value_; // Output of noise generator
    unsigned tone_value_; // Output of all three tone generators (8 bits each)
    // Note: all counters are in fixed point format with 10 bits reserved for decimals
    unsigned tone_counter_[NumChannels+2]; // Tone channels plus noise and envelope
};

#endif // AY_3_8910_H_
