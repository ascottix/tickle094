#ifndef DCE_PROXY_H_
#define DCE_PROXY_H_

#include "dce.h"

class DProxy : public DLine
{
public:
    DProxy() : DLine() {
        state_ = 0;
        remote_state_ = 0;
        assign( state_ );
    }

    void connectTo( DChip & chip, unsigned line ) {
        remote_state_ = chip.getLine(line).connection();
    }

    void update() {
        if( remote_state_ != 0 ) {
            state_ = *remote_state_;
        }
    }

private:
    DBit    state_;
    DBit *  remote_state_;
};

#endif // DCE_PROXY_H_
