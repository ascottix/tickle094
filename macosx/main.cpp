/*
     Tickle front-end
     User interface for Mac OS X
     
     Copyright (c) 2006-2010,2011 Alessandro Scotti
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <AGL/agl.h>
#include <Carbon/Carbon.h>
#include <QuickTime/QuickTime.h>

#include <emu/emu.h>

#include "joystick.h"
#include "mac_frame.h"
#include "openal.h"
#include "opengl.h"
#include "util.h"

// Define the symbol below to use "idle" events rather than high-resolution timers
//#define USE_IDLE_EVENTS

const MenuCommand kCommandOptions = FOUR_CHAR_CODE('opts');
const MenuCommand kCommandRestart = FOUR_CHAR_CODE('rest');

const int MaxSnapshotFiles = 100;

enum {
    kIDLE = 'IDLE'
};

int GetDriverFromUser( IBNibRef sNibRef );
int ShowOptionsWindow( IBNibRef sNibRef, TMachine * machine, WindowRef mainWindow );
bool IsOptionsWindowActive();

const unsigned SamplesPerSecond = 44100;

// Global variables
IBNibRef sNibRef;
int sizeFactor = 1;
Rect sizeRect[3];
bool sizeRectValid[3] = { false, false, false };
TMachine * machine  = 0;
TEmuInputManager inputManager;
bool active = false;
bool activeOneFrameOnly = false;
int samplesPerFrame = 0;
MacFrame * frame = 0;
OpenALPlayback * sound = 0;
WindowRef mainWindow = 0;
AGLContext mainContext = 0;
EventLoopTimerRef mainTimer = 0;
UInt32 currentKeyMods = 0;
MenuRef viewMenuRef = 0;
TJoystick * joy[4] = { 0, 0, 0, 0 };

// Resizes the main window on the current frame size
static void resizeMainWindowOnFrame()
{
    if( frame != 0 && mainWindow != 0 ) {
        Rect rc;
        
        if( sizeRectValid[sizeFactor] ) {
            rc = sizeRect[sizeFactor];
        }
        else {
            GetWindowBounds( mainWindow, kWindowContentRgn, &rc );
            
            if( sizeFactor == 1 || sizeFactor == 2 ) {
                // Fixed factor
                rc.right = rc.left + frame->width_ * sizeFactor;
                rc.bottom = rc.top + frame->height_ * sizeFactor;
            }
            else {
                // Greatest available size (preserving aspect ration)
                GetWindowGreatestAreaDevice( mainWindow, kWindowStructureRgn, NULL, &rc );
                
                int avail_w = rc.right - rc.left;
                int avail_h = rc.bottom - rc.top;
                
                int h = avail_w * frame->height_ / frame->width_;
                int w = avail_h * frame->width_ / frame->height_;
                
                if( h <= avail_h ) {
                    rc.bottom = rc.top + h;
                }
                else {
                    rc.right = rc.left + w;
                }
            }
        }

        SetWindowBounds( mainWindow, kWindowContentRgn, &rc );

        // Automatically place options window if visible (...really a good idea?!?)
        if( IsOptionsWindowActive() ) {
            ShowOptionsWindow( sNibRef, machine, mainWindow );
        }
        
        // Recreate context for new window size
        termOpenGL(mainContext);
        mainContext = initOpenGL( mainWindow, 0 );
        if( mainContext == 0 ) {
            printf( "*** Fatal: cannot recreate OpenGL context for current window\n" );
            exit( EXIT_FAILURE );
        }
        setAglContextSize( mainContext, frame->width_, frame->height_ );
    }
}

void resynchViewMenu()
{
    int item = sizeFactor == 0 ? 3 : sizeFactor;
    
    for( int i=1; i<=3; i++ ) {
        CheckMenuItem( viewMenuRef, i, i == item );
    }
}

static void setResizeMode( int mode ) 
{
    GetWindowBounds( mainWindow, kWindowContentRgn, sizeRect + sizeFactor );
    sizeRectValid[sizeFactor] = true;
    
    sizeFactor = mode % 3;
    
    resizeMainWindowOnFrame();
    resynchViewMenu();
}

static void nextResizeStep()
{
    setResizeMode( (sizeFactor + 1) % 3 );
}

static void setWindowTitle()
{
    if( mainWindow != 0 ) {
        const char * machineName = machine->getDriverInfo()->machineInfo()->name;
        
        char * title = new char [ strlen(machineName) + 64 ];
        
        strcpy( title, machineName );
        
        if( ! active ) {
            strcat( title, " - PAUSED" );
        }
        
        CFStringRef sTitle = CFStringCreateWithCString( 0, title, kCFStringEncodingASCII );
        SetWindowTitleWithCFString( mainWindow, sTitle );
        CFRelease( sTitle );
        
        delete [] title;
    }
}

void setActive( bool a )
{
    if( active != a ) {
        active = a;

        setWindowTitle();
        
        if( sound != 0 ) {
            if( active ) {
                sound->start();
            }
            else {
                sound->stop();
            }
        }
    }
}

bool loadGame( TMachine * m = 0 )
{
    bool result = false;
    
    setActive( false );
    
    // If we were not given a machine to load, let the user choose one
    if( m == 0 ) {
        int selected = GetDriverFromUser( sNibRef );
            
        if( selected >= 0 ) {
            const TGameRegistryItem * t = TGameRegistry::instance().item( selected );
            
            if( t != 0 ) {
                m = TMachine::createInstance( t->factory() );
            }
        }
    }
    
    // Load machine
    if( m != 0 ) {
        TList badFiles;
        bool ok = loadMachineFiles( m, badFiles );
        
        if( ! ok ) {
            // Unable to load required files
            TString msg = "One or more files could not be loaded:\n";
            
            for( int i=0; i<badFiles.count(); i++ ) {
                msg += "- ";
                msg += (char *) badFiles.item(i);
                msg += "\n";
            }
            
            msg += "\nFiles were looked for in the \"roms\" folder (also \"samples\""
                "\nfor sounds) and in the following archives:\n";
            
            for( int j=0; j<m->getResourceCount(); j++ ) {
                msg += "- ";
                msg += m->getResourceName( j );
                msg += ".zip\n";
            }
            
            msg += "\nDo you want to start the game anyway?";
            
            CFStringRef s_msg = CFStringCreateWithCString( 0, msg.cstr(), kCFStringEncodingASCII );
            DialogRef alert;
            DialogItemIndex alert_exit_code;
            AlertStdCFStringAlertParamRec alert_param;
            
            alert_param.version = kStdCFStringAlertVersionOne;
            alert_param.movable = true;
            alert_param.helpButton = false;
            alert_param.defaultText = CFSTR( "Start" );
            alert_param.cancelText = CFSTR( "Do not start" );
            alert_param.otherText = 0;
            alert_param.defaultButton = kAlertStdAlertOKButton;
            alert_param.cancelButton = kAlertStdAlertCancelButton;
            alert_param.position = kWindowDefaultPosition;
            alert_param.flags = 0;
            
            if( CreateStandardAlert( kAlertCautionAlert, CFSTR("Cannot load driver"), s_msg, &alert_param, &alert ) == noErr ) {
                RunStandardAlert( alert, 0, &alert_exit_code );

                ok = alert_exit_code ==  kAlertStdAlertOKButton;
            }
            
            CFRelease(s_msg);
            
            if( ! ok ) {
                delete m;
            }
        }
        
        if( ok ) {
            // Replace current machine
            delete machine;
            machine = m;
            machine->reset();
            
            // Get info and update the GUI with the new parameters
            const TMachineDriverInfo * info = m->getDriverInfo();
            
            int w = info->machineInfo()->screenWidth;
            int h = info->machineInfo()->screenHeight;
            
            // Create a new frame buffer for this machine
            delete frame;
            frame = new MacFrame( w, h );
            
            // Set the window title
            setWindowTitle();
            
            // Resize the main window
            sizeRectValid[0] = false;
            sizeRectValid[1] = false;
            sizeRectValid[2] = false;
            resizeMainWindowOnFrame();
            
            // Create a new playback device
            delete sound;
            samplesPerFrame = SamplesPerSecond / info->machineInfo()->framesPerSecond;
            sound = new OpenALPlayback( SamplesPerSecond, samplesPerFrame );
            
            if( sound->getLastError() < 0 ) {
                printf( "*** Error: sound initialization failed!\n" );
            }
            
            // Update options dialog if still open
            if( IsOptionsWindowActive() ) {
                ShowOptionsWindow( sNibRef, m, mainWindow );
            }

            result = true;
        }
    }

    setActive( true );

    return result;
}

void drawCurrentFrame()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0.0, 1024.0, 0.0, 1024.0, -1.0, +1.0 );
    glDisable(GL_DEPTH_TEST);
        
    glRasterPos2i(0,0);
    glDrawPixels( frame->width_, frame->height_, GL_RGBA, GL_UNSIGNED_BYTE, frame->screen_ );
    
    GLint interval = 1; // Synch with vertical retrace (0=don't synch)
    aglSetInteger(mainContext, AGL_SWAP_INTERVAL, &interval);
    aglSwapBuffers(mainContext);
}

int adjustJoystickRange( int value )
{
    int result = 0;
    
    if( value > 128 ) {
        result = ((value-128) * jpHalfRange) / 127;
    }
    else if( value < 128 ) {
        result = (value * jpHalfRange) / 128 - jpHalfRange;
    }
    
    return result;
}

void runOneFrame()
{
    if( ! active ) {
        return;
    }
    
    for( int i=0; i<2; i++ ) {
        if( getJoystick(i) != 0 ) {
            getJoystick(i)->poll();
            
            int x = adjustJoystickRange( getJoystick(i)->x() );
            int y = adjustJoystickRange( getJoystick(i)->y() );
            
            joy[i]->setPosition( x, y );
            joy[i]->setButtons( getJoystick(i)->buttons() );
        }
    }
    
    inputManager.notifyJoysticks( machine );
    
    // Draw screen
    drawCurrentFrame();
    
    // Run the machine for a frame
    unsigned samplingRate = SamplesPerSecond;
    unsigned samplesPerFrame = samplingRate / machine->getDriverInfo()->machineInfo()->framesPerSecond;
    
    frame->getMixer()->clear();
    machine->run( frame, samplesPerFrame, samplingRate );
    frame->setFlags(0);
    
    // Write into the sound buffer
    if( sound != 0 ) {
        int res = 0;

        // TODO: hack for Rebounds!
        if( strchr( machine->getDriverInfo()->machineInfo()->name, '[' ) == 0 ) {
            res = sound->synchAndPlay( frame->getMixer()->buffer(0), frame->maxVoices() );        
            
            if( res < 0 ) {
            // Don't know why but OpenAL seems to fail every now and then... can't recover, just start it again
                printf( "*** Error: cannot synch sound buffer (err=%d)\n", res );
                setActive( false );
                delete sound;
                sound = new OpenALPlayback( SamplesPerSecond, samplesPerFrame );
                setActive( true );
            }
        }
    }

    if( activeOneFrameOnly ) {
        activeOneFrameOnly = false;
        active = false;
    }
}

static size_t FrameScreenGetBytes( void * info, void * buffer, size_t count )
{
    size_t *ofs = (size_t *) info;
    unsigned char * src = (unsigned char *) frame->screen_;
    unsigned char * dst = (unsigned char *) buffer;

    // Hack: the image appears upside down, fix it here for PNG
    size_t line_size = frame->width_ * 4;
    size_t o = 0;
    size_t y;
    size_t x = line_size;
    for( size_t i=0; i<count; i++ ) {
        if( x >= line_size ) {
            y = (*ofs + i) / line_size;
            x = (*ofs + i) % line_size;
            o = (frame->height_ - 1 - y) * line_size;
        }
        *dst++ = src[o+x];
        x++;
    }

    *ofs += count;
    
    return count;
}

void FrameScreenSkipBytes( void *info, size_t count )
{
    size_t * ofs = (size_t *) info;
    *ofs += count;
}

void FrameScreenRewind( void *info )
{
    size_t * ofs = (size_t *) info;
    *ofs = 0;
}

void TakeSnapshotPNG()
{
    SInt32 err = noErr;
    
    // Get a good filename
    char filename[1024];
    int index = 1;
    
    while( index < MaxSnapshotFiles ) {
        sprintf( filename, "/Users/%s/Desktop/TickleSnapshot%02d.png", getlogin(), index ); // There must be a better way...
        
        FILE * f = fopen( filename, "rb" );
        
        if( f == 0 ) {
            break;
        }
        
        fclose( f );
        
        index++;
    }
    
    if( index >= MaxSnapshotFiles ) {
        printf( "*** Error: cannot find a good filename for the snapshot\n" );
        return;
    }
    
    // Create URL and target file
    CGRect pageRect = CGRectMake( 0.0, 0.0, frame->width_, frame->height_ );
    CFStringRef path = CFStringCreateWithCString( NULL, filename, kCFStringEncodingUTF8 );
    CFURLRef url = CFURLCreateWithFileSystemPath( NULL, path, kCFURLPOSIXPathStyle, 0 );
    CFRelease( path );
    
    CFDataRef dummyData = CFDataCreate( NULL, NULL, 0 );
	CFURLWriteDataAndPropertiesToResource( url, dummyData, NULL, &err );
	CFRelease( dummyData );

    FSRef fileRef;
	CFURLGetFSRef( url, &fileRef );
	FSSpec fileSpec;
	FSGetCatalogInfo( &fileRef, kFSCatInfoNone, NULL, NULL, &fileSpec, NULL );
    CFRelease( url );
    
	// Create a PNG exporter
	GraphicsExportComponent	exComp;
	err = OpenADefaultComponent( GraphicsExporterComponentType, kQTFileTypePNG, &exComp );
    
    // Create image from current screen
    size_t offset = 0;
    CGDataProviderCallbacks providerCallbacks;
    providerCallbacks.getBytes = FrameScreenGetBytes;
    providerCallbacks.skipBytes = FrameScreenSkipBytes;
    providerCallbacks.rewind = FrameScreenRewind;
    providerCallbacks.releaseProvider = 0;
    CGDataProviderRef provider = CGDataProviderCreate( &offset, &providerCallbacks );
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName( kCGColorSpaceGenericRGB );
    CGImageRef image = CGImageCreate( frame->width_, frame->height_, 8, 32, frame->width_*4, colorSpace, kCGBitmapByteOrder32Big, provider, 0, 0, kCGRenderingIntentDefault );

    // Export the image
    GraphicsExportSetInputCGImage( exComp, image );
    GraphicsExportSetOutputFile( exComp, &fileSpec );
    GraphicsExportDoExport( exComp, NULL );
    CloseComponent( exComp );
    
    // Release the image
    CGImageRelease( image );
}

static void SaveDriverListAsJSON()
{
    TString json = TGameRegistry::instance().getAsJson();
    
    char filename[1024];
    
    sprintf( filename, "/Users/%s/Desktop/TickleDrivers.json", getlogin() );
        
    FILE * f = fopen( filename, "wt" );
    
    if( f != 0 ) {
        fwrite( json.cstr(), 1, json.length(), f );
        fclose( f );
    }
}

static void TimerEventHandler( EventLoopTimerRef timer, void * userData )
{
    runOneFrame();
}

// Handles main window events
static OSStatus MainWindowEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void * inRefcon )
{
    OSStatus err = eventNotHandledErr;
    UInt32 eventClass = GetEventClass( inEvent );
    UInt32 eventKind = GetEventKind( inEvent );
    
    if( eventClass == kEventClassCommand ) {
        // Nothing to do here...
    }
    else if( eventClass == kEventClassKeyboard ) {
        UInt32 keyCode;
        UInt32 keyMods;
        
        switch ( eventKind ) {
            case kEventRawKeyDown:
                verify_noerr( GetEventParameter( inEvent, kEventParamKeyCode, typeUInt32, NULL, sizeof( keyCode ), NULL, &keyCode ) );
                if( ! inputManager.handle( keyCode, 1, machine ) ) {
                    if( keyCode == kMacKeyF2 ) {
                        nextResizeStep();
                    }
                    else if( keyCode == kMacKeyF4 ) {
                        TakeSnapshotPNG();
                    }
                    else if( keyCode == kMacKeyF5 ) {
                        SaveDriverListAsJSON();
                    }
                    else if( keyCode == kMacKeyF8 ) {
                        // Debug flag
                        frame->setFlags( frame->getFlags() | 0x01 );
                    }
                    else if( keyCode == kMacKeyP ) {
                        if( currentKeyMods & kKeyModShift ) {
                            if( active ) {
                                setActive( false );
                            }
                            else {
                                activeOneFrameOnly = true;
                                setActive( true );
                            }
                        }
                        else {
                            setActive( ! active );
                        }
                    }
                }
                break;
            case kEventRawKeyUp:
                verify_noerr( GetEventParameter( inEvent, kEventParamKeyCode, typeUInt32, NULL, sizeof( keyCode ), NULL, &keyCode ) );
                inputManager.handle( keyCode, 0, machine );
                break;
            case kEventRawKeyModifiersChanged:
                verify_noerr( GetEventParameter( inEvent, kEventParamKeyModifiers, typeUInt32, NULL, sizeof( keyMods ), NULL, &keyMods ) );
                if( (keyMods ^ currentKeyMods) & kKeyModCtrl ) {
                    inputManager.handle( kMacKeyLeftControl, keyMods & kKeyModCtrl ? 1 : 0, machine );
                }
                currentKeyMods = keyMods;
                break;
        }
    }
    
    return err;
}

// Creates the main window
DEFINE_ONE_SHOT_HANDLER_GETTER( MainWindowEventHandler );

static OSStatus CreateMainWindow()
{
    OSStatus err;
    WindowRef window;
    static const EventTypeSpec kWindowEvents[] = {
        { kEventClassCommand, kEventCommandProcess },
        { kEventClassKeyboard, kEventRawKeyDown },
        { kEventClassKeyboard, kEventRawKeyUp },
        { kEventClassKeyboard, kEventRawKeyModifiersChanged }
    };
        
    // Create window
    err = CreateWindowFromNib( sNibRef, CFSTR("MainWindow"), &window );

    if( err == noErr ) {
        // Create OpenGL context
        AGLContext context = initOpenGL( window, 0 );
        
        if( context == 0 ) {
            printf( "*** Fatal: cannot initialize OpenGL\n" );
            exit( EXIT_FAILURE );
        }
        
        // Install event handler
        InstallWindowEventHandler( window,
            GetMainWindowEventHandlerUPP(),
            GetEventTypeCount( kWindowEvents ), kWindowEvents,
            window, 
            NULL );
     
#ifndef USE_IDLE_EVENTS
        // Install timer event handler
        err = InstallEventLoopTimer( GetCurrentEventLoop(), 0, kEventDurationSecond / (5*machine->getDriverInfo()->machineInfo()->framesPerSecond), NewEventLoopTimerUPP( TimerEventHandler ), NULL, &mainTimer );

        if( err != noErr ) {
            printf( "*** Fatal: cannot install event loop timer, error: %s\n", GetMacOSStatusCommentString(err) );
            exit( EXIT_FAILURE );
        }
        // TODO: timer should be reallocated if "framePerSecond" changes (for new machines)
#endif
        
        // Save window reference
        mainWindow = window;
        mainContext = context;
        viewMenuRef = GetMenuRef( 130 );
        
        // Reposition and show the window
        resynchViewMenu();
        setWindowTitle();
        resizeMainWindowOnFrame();
        RepositionWindow( window, NULL, kWindowCascadeOnMainScreen );
        ShowWindow( mainWindow );
    }
    
    return err;
}

static DialogItemIndex messageBox( const char * title, const char * text )
{
    DialogRef alert;
    DialogItemIndex alert_exit_code;
    CFStringRef s_title = CFStringCreateWithCString( 0, title, kCFStringEncodingASCII );
    CFStringRef s_text = CFStringCreateWithCString( 0, text, kCFStringEncodingASCII );
    
    CreateStandardAlert( kAlertNoteAlert, s_title, s_text, 0, &alert );
    
    RunStandardAlert( alert, 0, &alert_exit_code );
    
    CFRelease( s_text );
    CFRelease( s_title );
    
    return alert_exit_code;
}

// Handles application events
static OSStatus AppEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void * inRefcon )
{
    OSStatus result = eventNotHandledErr;
    UInt32 eventClass = GetEventClass( inEvent );
    UInt32 eventKind = GetEventKind( inEvent );
    
    if( eventClass == kEventClassCommand ) {
        HICommandExtended cmd;
        verify_noerr( GetEventParameter( inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof( cmd ), NULL, &cmd ) );
            
        if( eventKind == kEventCommandProcess ) {
            switch ( cmd.commandID ) {
                case kHICommandOpen:
                    loadGame();
                    break;
                case 'OPTS':
                    ShowOptionsWindow( sNibRef, machine, mainWindow );
                    break;
                case 'abou':
                    if( HIAboutBox != NULL ) {
                        // No way to make the damn thing work with a dictionary!
                        HIAboutBox(0);
                    }
                    else {
                        messageBox( "Tickle", "Copyright (c) 1997-2010,2011 Alessandro Scotti\nhttp://www.ascotti.org/" );
                    }
                    result = noErr;
                    break;
                case 'CMAP':
                    messageBox( "Controls",
                       "Left\t\tP1 Left\n"
                       "Right\t\tP1 Right\n"
                       "Up\t\tP1 Up\n"
                       "Down\tP1 Down\n\n"
                       "LCtrl\t\tP1 Action1\n"
                       "Z\t\tP1 Action2\n"
                       "X\t\tP1 Action3\n"
                       "C\t\tP1 Action4\n\n"
                       "J\t\tP2 Left\n"
                       "L\t\tP2 Right\n"
                       "I\t\tP2 Up\n"
                       "K\t\tP2 Down\n\n"
                       "E\t\tP2 Action1\n"
                       "R\t\tP2 Action2\n"
                       "T\t\tP2 Action3\n"
                       "G\t\tP2 Action4\n\n"
                       "1\t\tP1 Start\n"
                       "2\t\tP2 Start\n\n"
                       "5\t\tCoin slot 1\n"
                       "6\t\tCoin slot 2\n\n"
                       "P\t\tPause\n"
                       "Shift-P\tAdvance one frame\n"
                       "F2\t\tToggle window size\n"
                       "F4\t\tSave current screen to PNG\n"
                       "F5\t\tSave current driver list to JSON" );
                    break;
                case 'INFO':
                    {
                        char buffer[1024];
                        
                        const TMachineInfo * info = machine->getDriverInfo()->machineInfo();
                        
                        sprintf( buffer, "Screen: %dx%d\nFPS: %d\nManufacturer: %s\nYear: %d",
                            info->screenWidth,
                            info->screenHeight,
                            info->framesPerSecond,
                            info->manufacturer,
                            info->year );
 
                        messageBox( info->name, buffer );
                    }
                    break;
                case 'REST':
                    machine->reset();
                    break;
                case 'SIZ1':
                    setResizeMode(1);
                    break;
                case 'SIZ2':
                    setResizeMode(2);
                    break;
                case 'SIZ3':
                    setResizeMode(0);
                    break;
                case 'quit':
                    break;
                default:
                    printf( "*** Oops: apparently I forgot to implement command %x!\r\n", (unsigned) cmd.commandID );
                    break;
            }
        }
    }
    else if( eventClass == kIDLE ) {
        runOneFrame();
        
        // Immediately requeue another idle event
        PostEventToQueue( GetMainEventQueue(), inEvent, kEventPriorityStandard );
    }
    
    return result;
}

int main(int argc, char* argv[])
{
    OSStatus err;
    
    // If bundled, change the current directory so that application
    // behaves like in other platforms (with respect to the position
    // of the "roms" and "samples" directories)
    CFBundleRef bundle = CFBundleGetMainBundle();
    
    if( bundle != 0 ) {
        CFURLRef url = CFBundleCopyResourcesDirectoryURL( bundle );
        
        char buffer[1024];
        
        if( CFURLGetFileSystemRepresentation( url, true, (UInt8 *) buffer, sizeof(buffer) ) ) {
            strcat( buffer, "/../../.." );
            chdir( buffer );
        }
    }

    // Create a NIB reference, passing the name of the nib file (without the .nib extension)
    err = CreateNibReference( CFSTR("main"), &sNibRef );

    if( err != noErr ) {
        printf( "*** Fatal: cannot get NIB reference, error: %s\n", GetMacOSStatusCommentString(err) );
        return EXIT_FAILURE;
    }
    
    // Set the menu bar
    err = SetMenuBarFromNib( sNibRef, CFSTR("MenuBar") );

    if( err != noErr ) {
        printf( "*** Fatal: cannot set the menu bar, error: %s\n", GetMacOSStatusCommentString(err) );
        return EXIT_FAILURE;
    }
    
    // Install handler for common commands on the application target
    static const EventTypeSpec kAppEvents[] = {
        { kEventClassCommand, kEventCommandProcess },
        { kIDLE, kIDLE }
    };
    
    InstallApplicationEventHandler( NewEventHandlerUPP( AppEventHandler ), GetEventTypeCount( kAppEvents ), kAppEvents, 0, NULL );
    
    // Init emulation library
    initJoystickLib();
    
    TGameRegistry::instance().sort();
    
    inputManager.add( kMacKeyLeftControl, idKeyP1Action1 ); // Actually any CTRL key (handled with key modifiers event)
    inputManager.add( kMacKeyZ, idKeyP1Action2 ); // Z
    inputManager.add( kMacKeyX, idKeyP1Action3 ); // X
    inputManager.add( kMacKeyC, idKeyP1Action4 ); // C
    
    inputManager.add( kMacKeyE, idKeyP2Action1 ); // E
    inputManager.add( kMacKeyR, idKeyP2Action2 ); // R
    inputManager.add( kMacKeyT, idKeyP2Action3 ); // T
    inputManager.add( kMacKeyG, idKeyP2Action4 ); // G
    
    inputManager.add( kMacKey1, idKeyStartPlayer1 ); // 1
    inputManager.add( kMacKey2, idKeyStartPlayer2 ); // 2
    inputManager.add( kMacKey5, idCoinSlot1 ); // 5
    inputManager.add( kMacKey6, idCoinSlot2 ); // 6
    
    inputManager.add( kMacKey0, idKeyService1 ); // 0

    // Joysticks for player 1
    joy[0] = inputManager.addJoystick( idJoyP1Joystick1, kMacKeyLeft, kMacKeyRight, kMacKeyUp, kMacKeyDown );
    joy[0]->bindButtonToKey( 0, idKeyP1Action1 );
    joy[0]->bindButtonToKey( 1, idKeyP1Action2 );
    joy[0]->bindButtonToKey( 2, idKeyP1Action3 );
    joy[0]->bindButtonToKey( 3, idKeyP1Action4 );
    joy[0]->bindButtonToKey( 4, idKeyP1Action1 );  // To move flippers with a gamepad!
    joy[0]->bindButtonToKey( 5, idKeyP1Action4 );
    joy[0]->bindButtonToKey( 6, idKeyStartPlayer1 );
    joy[0]->bindButtonToKey( 7, idCoinSlot1 );
    
    joy[2] = inputManager.addJoystick( idJoyP1Joystick2, kMacKeyA, kMacKeyD, kMacKeyW, kMacKeyS );
    
    // Joysticks for player 2
    joy[1] = inputManager.addJoystick( idJoyP2Joystick1, kMacKeyJ, kMacKeyL, kMacKeyI, kMacKeyK );
    joy[1]->bindButtonToKey( 0, idKeyP2Action1 );
    joy[1]->bindButtonToKey( 1, idKeyP2Action2 );
    joy[1]->bindButtonToKey( 2, idKeyP2Action3 );
    joy[1]->bindButtonToKey( 3, idKeyP2Action4 );
    joy[1]->bindButtonToKey( 6, idKeyStartPlayer2 );
    joy[1]->bindButtonToKey( 7, idCoinSlot1 );
    
    joy[3] = 0;
    
    loadGame( TTickleMachine::create() );
    
    // Create main window
    CreateMainWindow();
    
    // Automatically resize to fit the screen
    setResizeMode(0);

#ifdef USE_IDLE_EVENTS
    // Create idle event if needed
    EventRef myEvent;
    
    err = CreateEvent( 0, kIDLE, kIDLE, 0, kEventAttributeNone, &myEvent );

    if( err != noErr ) {
        printf( "*** Fatal: cannot create idle event!\n" );
        return EXIT_FAILURE;
    }
    
    PostEventToQueue( GetMainEventQueue(), myEvent, kEventPriorityStandard );
#endif
        
    // Run the event loop
    RunApplicationEventLoop();
    
    // Terminate
    delete sound;
    termOpenGL( mainContext );
    termJoystickLib();
        
    return EXIT_SUCCESS;
}
