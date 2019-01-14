#ifndef REBOUND_H_
#define REBOUND_H_

#include <emu/emu_standard_machine.h>

#include <dce/dce_all.h>

class ReboundHorizontalSync;
class ReboundVerticalSync;
class ReboundScore;
class ReboundPaddle;
class ReboundNet;
class ReboundBall;
class ReboundServe;

class ReboundVideoSyncFast;

struct ReboundBoard
{
    ReboundBoard();

    // Power and ground
    DLine   s_POWER;
    DLine   s_GROUND;

    // Clock circuit
    DLine   s_CLOCK;
    DLine   s_NOT_CLOCK;

    // Horizontal synchronization circuit
    DLine   s_1H;
    DLine   s_2H;
    DLine   s_4H;
    DLine   s_8H;
    DLine   s_16H;
    DLine   s_32H;
    DLine   s_64H;
    DLine   s_128H;
    DLine   s_256H;
    DLine   s_NOT_256H;
    DLine   s_HRESET;
    DLine   s_NOT_HRESET;
    DLine   s_HSYNC;
    DLine   s_NOT_HSYNC;
    DLine   s_HBLANK;
    DLine   s_NOT_HBLANK;

    // Vertical synchronization circuit
    DLine   s_1V;
    DLine   s_2V;
    DLine   s_4V;
    DLine   s_8V;
    DLine   s_16V;
    DLine   s_32V;
    DLine   s_64V;
    DLine   s_128V;
    DLine   s_256V;
    DLine   s_NOT_256V;
    DLine   s_VRESET;
    DLine   s_NOT_VRESET;
    DLine   s_NOT_VSYNC;
    DLine   s_NOT_COMPOSITE_SYNC;

    // Score circuit
    DLine   s_SCORE;
    DLine   s_SCORE_LSB1;
    DLine   s_SCORE_LSB2;
    DLine   s_STOP;

    // Paddle circuit
    DLine   s_PADDLES;
    DLine   s_DISP_PAD_COMP;
    DLine   s_PADDLE_COUNT_1;
    DLine   s_PADDLE_COUNT_2;
    DLine   s_PADDLE_COUNT_4;

    double  paddle1_pos;
    double  paddle2_pos;

    // Start circuit
    DLine   s_START;
    DLine   s_NOT_START;
    DLine   s_ATTRACT;
    DLine   s_NOT_ATTRACT;

    // Net circuit
    DLine   s_NOT_NET;
    DLine   s_NOT_NET_HEIGHT;

    // Ball direction and speed
    DLine   s_BALL;
    DLine   s_NOT_BALL;
    DLine   s_HIT;
    DLine   s_NOT_HIT;
    DLine   s_NOT_BACKWARDS;

    //
    DLine   s_SERVE;
    DLine   s_NOT_SERVE;
    DLine   s_SERVE_TIME;
    DLine   s_NOT_SERVE_TIME;

    DLine   s_BALL_RETURN;

    // Clock frequency
    unsigned    clock_frequency;

    // Switches
    bool    opt_playTo15;       // If true then play stops at 15 points, otherwise at 11
    bool    opt_doubleCredit;   // If true then one credit gives two games, otherwise just one

    // Circuits
    ReboundHorizontalSync * c_horizontal_sync;
    ReboundVerticalSync *   c_vertical_sync;
    ReboundScore *          c_score;
    ReboundPaddle *         c_paddle;
    ReboundNet *            c_net;
    ReboundBall *           c_ball;
    ReboundServe *          c_serve;

    ReboundVideoSyncFast *  c_video_sync;

    void update( unsigned x, unsigned y );

//private:
    DBit    b_clock;
    DBit    b_not_clock;
};

class Rebound : public TStandardMachine
{
public:
    virtual ~Rebound();

    virtual bool handleInputEvent( unsigned device, unsigned param, void * data = 0 );

    virtual void run( TFrame * frame, unsigned samplesPerFrame, unsigned samplingRate );

    virtual void reset();

    static TMachine * createInstance() {
        return new Rebound();
    }

protected:
    Rebound();

    virtual bool initialize( TMachineDriverInfo * info );

    ReboundBoard    board_;
};

#endif // REBOUND_H_
