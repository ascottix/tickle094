#include "rebound.h"

#include "rebound_h_sync.h"
#include "rebound_v_sync.h"
#include "rebound_score.h"
#include "rebound_paddle.h"
#include "rebound_net.h"
#include "rebound_ball.h"
#include "rebound_serve.h"

#include "rebound_video_sync_fast.h"

//#define FAST_MODE // Somewhat faster, but less accurate

#ifdef WIN32
extern void trace( const char * fmt, ... );
#endif

// Hardware info
enum {
    ScreenWidth             = 453 - 80,
    ScreenHeight            = 262 - 14,
    ScreenColors            = 8,
    MasterClock             = 14318100,
    VideoFrequency          = 60,
};

enum { 
    OptCoin, OptScore
};

// Machine info
static TMachineInfo ReboundInfo = { 
    "rebound", "Rebound [IN PROGRESS]", "Atari", 1974, ScreenWidth, ScreenHeight, ScreenColors, VideoFrequency
};

static TGameRegistrationHandler reg( &ReboundInfo, Rebound::createInstance );

ReboundBoard::ReboundBoard()
{
    clock_frequency = MasterClock / 2;

    paddle1_pos = 2.0;
    paddle2_pos = 3.2;

    // Power and ground
    s_POWER.assign( DLine::Power );
    s_GROUND.assign( DLine::Ground );

    // Clock
    b_clock = 0;
    b_not_clock = 1 ^ b_clock;
    s_CLOCK.assign( b_clock );
    s_NOT_CLOCK.assign( b_not_clock );

    // TODO!!!
    s_START.assign( DLine::Ground );
    s_NOT_START.assign( DLine::Power );

#if 0
    // Attract mode
    s_ATTRACT.assign( DLine::Power );
    s_NOT_ATTRACT.assign( DLine::Ground );
#else
    // Play mode
    s_ATTRACT.assign( DLine::Ground );
    s_NOT_ATTRACT.assign( DLine::Power );
#endif

    s_BALL_RETURN.assign( DLine::Ground );

    //s_NOT_SERVE_TIME.assign( DLine::Power ); // TODO: see rebound_serve.cxx
    //s_NOT_BACKWARDS.assign( DLine::Power ); // TODO: see rebound_ball.cxx

    // Switches
    opt_playTo15 = true;
    opt_doubleCredit = false;

    // Board circuits
#ifdef FAST_MODE
    printf( "0\n" );
    c_video_sync = new ReboundVideoSyncFast( this );
    printf( "1\n" );
#else
    c_horizontal_sync = new ReboundHorizontalSync( this );
    c_vertical_sync = new ReboundVerticalSync( this );
#endif
    c_serve = new ReboundServe( this );
    c_net = new ReboundNet( this );
    c_score = new ReboundScore( this );
    c_paddle = new ReboundPaddle( this );
    c_ball = new ReboundBall( this );
}

void ReboundBoard::update( unsigned x, unsigned y )
{
    // Tick clock
    b_clock ^= 1;
    b_not_clock ^= 1;

    assert( (b_clock ^ b_not_clock) == 1 );

    // Update
#ifdef FAST_MODE
    c_video_sync->update();
#else
    c_horizontal_sync->update();
    c_vertical_sync->update();
#endif

    // Try to get some speed by updating net and score only when needed
    // (too risky?)
    if( 0 && y >= 32 ) {
        bool w_net = y >= 128 && x >= 254 && x < 260;
        bool w_score = y < 64 && ((x >= 128 && x < 192) || (x >= 320 && x < 384));

        if( w_net || w_score ) {
            c_net->update();    // Depends on NOT SERVE and BALL RETURN signals
            c_score->update();  // Depends on NOT NET signal
        }
    }

    c_net->update();    // Depends on NOT SERVE and BALL RETURN signals
    c_score->update();  // Depends on NOT NET signal

    // Try to get some speed by updating the paddle only when it's needed
    if( y < 2 || (y >= 248 && y < 256) ) {
        c_paddle->update();
    }

    // c_paddle->update();

    if( y >= 48 && y <= 196 ) {
        c_serve->update();
    }

    c_ball->update();
}

Rebound::Rebound()
{
    createScreen( ScreenWidth, ScreenHeight, ScreenColors );

    palette()->setColor( 0, TPalette::encodeColor(0x00,0x00,0x00) );
    palette()->setColor( 1, TPalette::encodeColor(0xFF,0xFF,0xFF) );

    registerDriver( ReboundInfo );
}

Rebound::~Rebound()
{
}

bool Rebound::initialize( TMachineDriverInfo * info )
{
    return true;
}

void Rebound::reset()
{
#ifdef FAST_MODE
    for( int y=0; y<262; y++ ) {
        for( int x=0; x<453; x++ ) {
            board_.update( x, y );
            board_.update( x, y );
        }
    }
#endif
}

bool Rebound::handleInputEvent( unsigned device, unsigned param, void * data )
{
    unsigned type = TInput::getDeviceType( device );
    unsigned id = TInput::getDeviceId( device );

    if( type == dtJoystick ) {
        int x = TInput::getXPosFromParam( param );

        double d = x < 0 ? -0.05 : +0.05;

        if( id == 0 ) {
            board_.paddle1_pos += d;
        }
        else if( id == 1 ) {
            board_.paddle2_pos += d;
        }
    }

    return true;
}

static int zzz = 0; // Debug stuff

void Rebound::run( TFrame * frame, unsigned samplesPerFrame, unsigned samplingRate )
{
    //FILE *  f = fopen( "c:\\o.txt", "a" );

    unsigned char * video = screen()->bits()->data();

    board_.c_paddle->setPaddlePos( board_.paddle1_pos, board_.paddle2_pos );

    unsigned    x;
    unsigned    y;

    // Skip the VSYNC signal
    for( y=0; y<262; y++ ) {
        //fprintf( f, "\nV=%d\n", y );

        for( x=0; x<453; x++ ) {
            // Draw the visible part
            if( y >= 14 && x >= 80 ) {
                *video = 0;
                *video |= board_.s_PADDLES.state();
                *video |= board_.s_SCORE.state();
                *video |= board_.s_BALL.state();

                //if( y == 252 || y == 255 ) *video = 1;

                //if( y == 64 || y == 128 || x == 196 || x == 256 ) *video = 1;
                
                // *video |= 1 ^ board_.c_ball->c3.getLine(12).state(); // TP4, BALL V VIDEO
                // *video |= 1 ^ board_.c_ball->f4.getLine(8).state(); // TP5, BALL H VIDEO

                video++;
            }

            for( int j=0; j<2; j++ ) {
                /*
                if( zzz == 2 ) {
                    fprintf( f, "Y=%3d, X=%3d, H=%d%d%d%d%d%d%d%d%d, HBLANK=%d, HSYNC=%d, HRESET=%d, ",
                        y,
                        x,
                        board_.s_256H.state(),
                        board_.s_128H.state(),
                        board_.s_64H.state(),
                        board_.s_32H.state(),
                        board_.s_16H.state(),
                        board_.s_8H.state(),
                        board_.s_4H.state(),
                        board_.s_2H.state(),
                        board_.s_1H.state(),
                        board_.s_HBLANK.state(),
                        board_.s_HSYNC.state(),
                        board_.s_HRESET.state()
                    );
                    
                    fprintf( f, " V=%d%d%d%d%d%d%d%d%d, NOT_VSYNC=%d, VRESET=%d\n",
                        board_.s_256V.state(),
                        board_.s_128V.state(),
                        board_.s_64V.state(),
                        board_.s_32V.state(),
                        board_.s_16V.state(),
                        board_.s_8V.state(),
                        board_.s_4V.state(),
                        board_.s_2V.state(),
                        board_.s_1V.state(),
                        board_.s_NOT_VSYNC.state(),
                        board_.s_VRESET.state()
                    );
                }
                */

                board_.update( x, y );
            }
        }

        if( y >= 260 || y < 2 ) {
            /*
            trace( "Y=%3d, Hm=%2d, Vm=%2d\n", 
                y,
                8*board_.c_ball->e3.getLine(6).state() + 4*board_.c_ball->e3.getLine(5).state() + 2*board_.c_ball->e3.getLine(4).state() + board_.c_ball->e3.getLine(3).state(),
                8*board_.c_ball->e4.getLine(7).state() + 4*board_.c_ball->e4.getLine(6).state() + 2*board_.c_ball->e4.getLine(2).state() + board_.c_ball->e4.getLine(3).state()
                );
            */
        }
    }

    zzz++;

    frame->setVideo( screen() );

    //fclose( f );
}
