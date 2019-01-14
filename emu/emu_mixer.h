/*
    Tickle class library

    Copyright (c) 2003,2004 Alessandro Scotti
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
#ifndef EMU_MIXER_H_
#define EMU_MIXER_H_

enum TMixerChannel 
{
    chMono = 0,
    chStereoLeft = 1,
    chStereoRight = 2
};

class TMixerBuffer
{
public:
    TMixerBuffer() : data_(0), size_(0), num_voices_(0) {
    }

    /** Destructor. */
    virtual ~TMixerBuffer() {
        delete data_;
    }

    virtual void clear();

    int * data() {
        return data_;
    }

    unsigned size() const {
        return size_;
    }

    unsigned voices() const {
        return num_voices_;
    }

    void setVoices( unsigned voices ) {
        num_voices_ = voices;
    }

    void addVoices( unsigned voices ) {
        num_voices_ += voices;
    }

    void expand( unsigned size );

private:
    int * data_;
    unsigned size_;
    unsigned num_voices_;
};

class TMixer
{
public:
    TMixer( unsigned channels );

    /** Destructor. */
    virtual ~TMixer();

    virtual void clear();

    virtual TMixerBuffer * getBuffer( unsigned channel, unsigned length, unsigned voices );

    const int * buffer( unsigned channel ) {
        return getBuffer( channel, 0, 0 )->data();
    }

    virtual int maxVoicesPerChannel();

    static void mix( int * dest, const int * source, unsigned len ) {
        while( len-- > 0 ) {
            *dest++ += *source++;
        }
    }

protected:
    virtual unsigned channels() const {
        return num_channels_;
    }

private:
    unsigned num_channels_;
    TMixerBuffer * buffers_;
};

class TMixerMono : public TMixer
{
public:
    TMixerMono() : TMixer(1) {
    }

    virtual TMixerBuffer * getBuffer( unsigned channel, unsigned length, unsigned voices );
};

class TMixerStereo : public TMixer
{
public:
    TMixerStereo() : TMixer(3) {
    }

    virtual TMixerBuffer * getBuffer( unsigned channel, unsigned length, unsigned voices );
};

#endif // EMU_MIXER_H_
