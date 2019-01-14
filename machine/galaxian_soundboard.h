/*
    Galaxian arcade machine emulator

    Soundboard emulation

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
#ifndef GALAXIAN_SOUNDBOARD_H_
#define GALAXIAN_SOUNDBOARD_H_

#include <ase/ase_bandpass_filter.h>
#include <ase/ase_clipper.h>
#include <ase/ase_inverter.h>
#include <ase/ase_latch.h>
#include <ase/ase_noise.h>
#include <ase/ase_opamp_noninv1.h>
#include <ase/ase_capacitor_with_switch.h>
#include <ase/ase_timer555_astable.h>
#include <ase/ase_timer555_linear_ramp.h>

/**
    Emulates the circuit that controls the frequency of the fire timer.
*/
class AGalaxianFireControl : public AChannel
{
public:
    AGalaxianFireControl( AChannel * ch_fire, AChannel * ch_noise );

    void updateBuffer( AFloat * buf, unsigned len, unsigned ofs );

private:
    AChannel * a_fire_;
    AChannel * a_noise_;
    double a_[4];
    double c28_;
    double c29_;
};

struct GalaxianSoundBoard
{
    GalaxianSoundBoard();

    ~GalaxianSoundBoard();

    void startFrame();

    void endFrame( unsigned offset );

    void playFrame( int * buffer, unsigned bufsize );

    void writeToLatch9L( unsigned currentTimeInSamples, unsigned offset, unsigned char value );

    void writeToLatch9M( unsigned currentTimeInSamples, unsigned offset, unsigned char value );

    void setPitch( unsigned currentTimeInSamples, unsigned char pitch ) {
        tone_pitch_ = pitch;
    }

    unsigned    frame_size_;

    // Sound emulation
    AWhiteNoise *           a_noise_;       // White noise generator

    ALatch *                a_hit_latch_;   // Hit input latch
    ACapacitorWithSwitch *  a_hit_;         // Hit signal generator (i.e. capacitor)
    AClipperLo *            a_hit_diode_;
    AActiveBandPassFilter * a_hit_filter_;  // Hit signal bandpass filter

    ALatch *                a_fire_latch_;  // Fire input latch
    AClipperLo *            a_fire_diode_;
    AInverter *             a_fire_latch_inverter_;
    AGalaxianFireControl *  a_fire_control_;// Fire control (controls timer)
    ATimer555Astable *      a_fire_timer_;  // Fire timer (modulates signal)
    ACapacitorWithSwitch *  a_fire_;        // Fire signal generator (i.e. capacitor)

    ATimer555Astable *      a_fs_[3];
    ALatch *                a_fs_control_current_;
    ATimer555LinearRamp *   a_fs_control_;
    AOpAmp_NonInv1 *        a_fs_control_amp_;
    AFloat                  a_fs_control_res_[4];

    unsigned                tone_pitch_;
    unsigned                tone_volume_;
    unsigned                tone_offset_;   // Current offset in "virtual" tone output stream
    unsigned                tone_output_;
    unsigned                tone_counter_;
    int                     tone_volume_table_[4][16];

    enum {
        ToneGeneratorClock  = 96000,    // ...scaled down already
    };
};

#endif // GALAXIAN_SOUNDBOARD_H_
