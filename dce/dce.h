#ifndef DCE_H_
#define DCE_H_

#include <assert.h>

typedef unsigned char DBit;

class DChip;

class DLine
{
public:
    DLine() {
        bit_ = &Unconnected;
        assigned_ = false;
        connected_ = false;

        numOfLines++;
    }

    DLine( const DLine & line ) {
        bit_ = line.bit_;
        assigned_ = false;
        connected_ = true;

        numOfLines++;
    }

    ~DLine() {
    }

    void connectTo( const DLine & line ) {
        assert( ! assigned_ );
        assert( line.connected_ );

        bit_ = line.bit_;
        connected_ = true;

        numOfLinesConnected++;
    }

    void connectTo( DChip & chip, unsigned line );

    void assign( DBit & bit ) {
        assert( ! assigned_ );

        bit_ = &bit;

        assigned_ = true;
        connected_ = true;

        numOfLinesAssigned++;
    }

    DBit state() const {
        return *bit_;
    }

    bool isConnected() const {
        return bit_ != &Unconnected;
    }

    DBit * connection() {
        return bit_;
    }

    static DBit Unconnected;
    static DBit Power;
    static DBit Ground;
    static DBit High;
    static DBit Low;

    static unsigned numOfLines;
    static unsigned numOfLinesAssigned;
    static unsigned numOfLinesConnected;

private:
    DLine & operator = ( const DLine & );

    DBit *  bit_;
    bool assigned_;
    bool connected_;
};

class DChip
{
public:
    DChip( unsigned numOfLines ) {
        assert( numOfLines > 0 );

        line_count_ = numOfLines;
        line_ = new DLine [ numOfLines ];

        numOfChips++;
        numOfChipLines += numOfLines;
    }

    virtual ~DChip() {
        delete [] line_;
    }

    virtual void update() = 0;

    DLine & getLine( unsigned index ) {
        assert( index > 0 );
        assert( index <= line_count_ );

        index--;

        return line_[index];
    }

    void connectLine( unsigned pin, DLine & otherLine ) {
        assert( pin > 0 );
        assert( pin <= line_count_ );

        line_[pin-1].connectTo( otherLine );
    }

    void connectLine( unsigned pin, DChip & otherChip, unsigned otherPin ) {
        assert( otherPin > 0 );
        assert( otherPin <= otherChip.line_count_ );

        connectLine( pin, otherChip.line_[otherPin-1] );
    }

    static unsigned numOfChips;
    static unsigned numOfChipLines;

protected:
    void connectLine( unsigned pin, DBit & bit ) {
        assert( pin > 0 );
        assert( pin <= line_count_ );

        line_[pin-1].assign( bit );
    }

    DBit bit( unsigned s ) {
        return s != 0 ? 1 : 0;
    }

    DBit line( unsigned pin ) {
        assert( pin > 0 );
        assert( pin <= line_count_ );

        return line_[pin-1].state();
    }

private:
    DLine * line_;
    unsigned line_count_;
};

inline void DLine::connectTo( DChip & chip, unsigned line ) {
    connectTo( chip.getLine(line) );
}


#endif // DCE_H_
