/*
    Tickle front-end
    User interface for Windows

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
#include <windows.h>
#include <commctrl.h>

#include <stdio.h>
#include <assert.h>

#include "resource.h"

#include "dx_app.h"
#include "win_frame.h"

#include <emu/emu.h>

#define WM_OPTIONS_CHANGED  (WM_USER+2)

#ifdef GCC
// For some reason g++ gets confused by the indirect
// reference and does not link the machines, so the whole point
// of not having to declare drivers here goes to... oh well...
#include <machine/1942.h>
#include <machine/fantasy.h>
#include <machine/frogger.h>
#include <machine/galaga.h>
#include <machine/galaxian.h>
#include <machine/invaders.h>
#include <machine/nibbler.h>
#include <machine/pacman.h>
#include <machine/pengo.h>
#include <machine/pinball_action.h>
#include <machine/pooyan.h>
#include <machine/rallyx.h>
#include <machine/scramble.h>
#include <machine/vanguard.h>

void dummy()
{
    delete M1942::createInstance();
    delete Fantasy::createInstance();
    delete Frogger::createInstance();
    delete Galaga::createInstance();
    delete Galaxian::createInstance();
    delete Nibbler::createInstance();
    delete Pacman::createInstance();
    delete Pengo::createInstance();
    delete PinballAction::createInstance();
    delete Pooyan::createInstance();
    delete RallyX::createInstance();
    delete Scramble::createInstance();
    delete SpaceInvaders::createInstance();
    delete Vanguard::createInstance();
}
#endif

const unsigned SamplesPerSecond = 44100;

// Global variables
char szAppName[] = "Tickle";    // Application name
DxApp *   dxApp     = 0;
HWND hWindow     = NULL;        // Handle of main window
HWND hDlgOptions = NULL;
int  sizeFactor  = 1;
TMachine * machine  = 0;
WinFrame * frame    = 0;
TEmuInputManager inputManager;

TJoystick * joy[4] = { 0, 0, 0, 0 };

static char szAbout[] = 
    "Tickle\n"
    "Copyright (c) 1997-2010,2011 Alessandro Scotti\n"
    "\n"
    "http://www.ascotti.org/";

// 
void setInputManager( int config )
{
    inputManager.clear();

    switch( config ) {
    case 0:
        inputManager.add( VK_CONTROL, idKeyP1Action1 );
        inputManager.add( 'Z', idKeyP1Action2 );
        inputManager.add( 'X', idKeyP1Action3 );
        inputManager.add( 'C', idKeyP1Action4 );

        inputManager.add( 'E', idKeyP2Action1 );
        inputManager.add( 'R', idKeyP2Action2 );
        inputManager.add( 'T', idKeyP2Action3 );
        inputManager.add( 'G', idKeyP2Action4 );

        inputManager.add( '1', idKeyStartPlayer1 );
        inputManager.add( '2', idKeyStartPlayer2 );
        inputManager.add( '5', idCoinSlot1 );
        inputManager.add( '6', idCoinSlot2 );

        inputManager.add( '0', idKeyService1 );

        // Player 1
        joy[0] = inputManager.addJoystick( idJoyP1Joystick1, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN );
        joy[0]->bindButtonToKey( 0, idKeyP1Action1 );
        joy[0]->bindButtonToKey( 1, idKeyP1Action2 );
        joy[0]->bindButtonToKey( 2, idKeyP1Action3 );
        joy[0]->bindButtonToKey( 3, idKeyP1Action4 );
        joy[0]->bindButtonToKey( 4, idKeyP1Action1 );  // To move flippers with a gamepad!
        joy[0]->bindButtonToKey( 5, idKeyP1Action4 );
        joy[0]->bindButtonToKey( 6, idKeyStartPlayer1 );
        joy[0]->bindButtonToKey( 7, idCoinSlot1 );

        joy[2] = inputManager.addJoystick( idJoyP1Joystick2, 'A','D', 'W', 'S' );

        // Player 2
        joy[1] = inputManager.addJoystick( idJoyP2Joystick1, 'J', 'L', 'I', 'K' );
        joy[1]->bindButtonToKey( 0, idKeyP2Action1 );
        joy[1]->bindButtonToKey( 1, idKeyP2Action2 );
        joy[1]->bindButtonToKey( 2, idKeyP2Action3 );
        joy[1]->bindButtonToKey( 3, idKeyP2Action4 );
        joy[1]->bindButtonToKey( 6, idKeyStartPlayer2 );
        joy[1]->bindButtonToKey( 7, idCoinSlot1 );

        break;
    }
}

const char * PathSeparator = "\\";

// Load a file from an input stream into a newly allocated buffer
// (which must be deleted by the caller)
unsigned char * loadFileFromStream( TInputStream * is, unsigned size, TCRC32 * crc )
{
    unsigned char * result = new unsigned char [size];

    if( is->read( result, size ) == size ) {
        crc->reset();
        crc->update( result, size );
    }
    else {
        delete result;
        result = 0;
    }

    return result;
}

unsigned getFileSize( const char * name )
{
    unsigned result = 0;

    FILE * f = fopen( name, "rb" );

    if( f != 0 ) {
        fseek( f, 0L, SEEK_END );
        result = (unsigned) ftell( f );
        fclose( f );
    }

    return result;
}

// Load a single file required by the driver
unsigned char * loadFile( TMachine * machine, const TResourceFileInfo * info, const char * base, unsigned * bufsize )
{
    unsigned char * result = 0;

    TString home;
    
    if( base != 0 ) {
        home += base;
        home += PathSeparator;
    }

    TString file = home + info->name;
    TCRC32 crc;
    TInputStream * is = TFileInputStream::open( file.cstr() );

    if( is != 0 ) {
        unsigned size = info->size;

        if( size == 0 ) {
            size = getFileSize( file.cstr() );
        }

        result = loadFileFromStream( is, size, &crc );

        *bufsize = size;

        delete is;
    }
    else {
        for( int i=0; i<machine->getResourceCount(); i++ ) {
            file = home + machine->getResourceName(i) + ".zip";

            TZipFile * zf = TZipFile::open( file.cstr() );

            if( zf != 0 ) {
                const TZipEntry * ze = zf->entry( info->name, true );
                if( ze != 0 ) {
                    is = ze->open();

                    if( is != 0 ) {
                        *bufsize = info->size ? info->size : ze->size();
                        result = loadFileFromStream( is, *bufsize, &crc );
                    }

                    delete is;
                }

                delete zf;
            }

            if( result != 0 )
                break;
        }
    }

    if( result != 0 ) {
        // Delete buffer if CRC check fails
        if( (info->crc != 0) && (crc.value() != info->crc) ) {
            delete result;
            result = 0;
        }
    }

    return result;
}

// Load the files required by the driver
bool loadMachineFiles( TMachine * machine, TList & failedList )
{
    bool result = true;
    const TMachineDriverInfo * info = machine->getDriverInfo();

    for( int i=0; i<info->resourceFileCount(); i++ ) {
        const TResourceFileInfo * file = info->resourceFile( i );
        unsigned size = 0;
        unsigned char * buf = loadFile( machine, file, "roms", &size );

        if( (buf == 0) && (strstr(file->name,".wav") != 0) ) {
            buf = loadFile( machine, file, "samples", &size );
        }

        if( buf != 0 ) {
            machine->setResourceFile( file->id, buf, size );
            delete buf;
        }
        else {
            failedList.add( (void *)file->name );
            result = false;
        }
    }

    return result;
}

struct GameOptionsParam
{
    TList * options;
    TMachine * machine;
    HWND hWndParent;
};

GameOptionsParam * makeOptionsParam( TMachine * machine )
{
    // Prepare the parameters for the dialog box
    GameOptionsParam * param = new GameOptionsParam;

    param->hWndParent = hWindow;
    param->options = new TList;
    param->machine = machine;

    // The option list is arranged in groups, make it a flat list for our dialog box
    TUserInterface * ui = machine->getDriverInfo()->userInterface();
    int index;

    for( index=0; index<ui->groupCount(); index++ ) {
        TUiOptionGroup * group = ui->group(index);

        for( int i=0; i<group->count(); i++ ) {
            param->options->add( group->item(i) );
        }
    }

    return param;
}

// Sort callback for the selection list view
int CALLBACK LvCompareFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    const TMachineInfo * info1 = (const TMachineInfo *) lParam1;
    const TMachineInfo * info2 = (const TMachineInfo *) lParam2;

    switch( lParamSort ) {
    case 1:
        return strcmp( info1->manufacturer, info2->manufacturer );
    case 2:
        return info1->year - info2->year;
    }

    return strcmp( info1->name, info2->name );
}

int LvItemToDriverIndex( HWND hLV, int item )
{
    int result = -1;

    LV_ITEM lvi = { 0 };

    lvi.iItem = item;
    lvi.mask = LVIF_PARAM;
    ListView_GetItem( hLV, &lvi );

    TMachineInfo * info = (TMachineInfo *) lvi.lParam;

    if( info != 0 ) {
        result = TGameRegistry::instance().find( info->name );
    }

    return result;
}

// Driver selection dialog box procedure
BOOL CALLBACK GameListDialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg ) {
    case WM_INITDIALOG:
        {
            // Setup the list view columns
            HWND hLV = GetDlgItem( hWnd, IDC_LISTVIEW );
            RECT rc;

#ifdef LVS_EX_FULLROWSELECT
            ListView_SetExtendedListViewStyle(hLV, LVS_EX_FULLROWSELECT );
#endif

            GetWindowRect( hLV, &rc );

            DWORD width = rc.right - rc.left - GetSystemMetrics( SM_CXVSCROLL );
            DWORD cx = width / 4;
            LV_COLUMN lvc = { 0 };

            lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
            lvc.fmt = LVCFMT_LEFT;

            lvc.cx = width - (cx + 2) * 2;
            lvc.pszText = "Name";
            ListView_InsertColumn( hLV, 0, &lvc );

            lvc.cx = cx;
            lvc.pszText = "Manufacturer";
            ListView_InsertColumn( hLV, 1, &lvc );

            lvc.cx = cx;
            lvc.pszText = "Year";
            ListView_InsertColumn( hLV, 2, &lvc );

            // Load the items
            TGameRegistry & gameRegistry = TGameRegistry::instance();

            for( int i=0; i<gameRegistry.count(); i++ ) {
                LV_ITEM lvi = { 0 };

                lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
                lvi.pszText = LPSTR_TEXTCALLBACK;
                lvi.lParam = (LPARAM) gameRegistry.item(i)->info();
                lvi.state = (i == 0) ? LVIS_SELECTED : 0;

                ListView_InsertItem( hLV, &lvi );
            }

            ListView_SortItems( hLV, LvCompareFunc, 0 );

            // Select and highlight the first item
            ListView_SetItemState( hLV, 0, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED );
        }
        break;
    case WM_NOTIFY:
        if( wParam == IDC_LISTVIEW ) {
            switch( ((NMHDR *) lParam)->code ) {
            case LVN_GETDISPINFO:
                if( ((LV_DISPINFO *) lParam)->item.mask & LVIF_TEXT ) {
                    LV_DISPINFO * lvdi = (LV_DISPINFO *) lParam;
                    TMachineInfo * info = (TMachineInfo *) lvdi->item.lParam;

                    switch( lvdi->item.iSubItem ) {
                    case 0:
                        lvdi->item.pszText = (char *) info->name;
                        break;
                    case 1:
                        lvdi->item.pszText = (char *) info->manufacturer;
                        break;
                    case 2:
                        _itoa( info->year, lvdi->item.pszText, 10 );
                        break;
                    }
                }
                break;
            case LVN_COLUMNCLICK:
                ListView_SortItems( GetDlgItem(hWnd,IDC_LISTVIEW), LvCompareFunc, ((NM_LISTVIEW *) lParam)->iSubItem );
                break;
            case NM_DBLCLK:
                DWORD pos = GetMessagePos();
                LV_HITTESTINFO lvht;

                ZeroMemory( &lvht, sizeof(lvht) );

                lvht.pt.x = LOWORD(pos);
                lvht.pt.y = HIWORD(pos);

                ScreenToClient( GetDlgItem(hWnd,IDC_LISTVIEW), &lvht.pt );

                ListView_HitTest( GetDlgItem(hWnd,IDC_LISTVIEW), &lvht );

                if( lvht.iItem >= 0 ) {
                    EndDialog( hWnd, LvItemToDriverIndex(GetDlgItem(hWnd,IDC_LISTVIEW),lvht.iItem) );
                }
                break;
            }
        }
        break;
    case WM_LBUTTONDBLCLK:
        break;
    case WM_COMMAND:
        {
            WORD id = LOWORD(wParam);

            if( id == IDOK ) {
                // Return current selection and exit
                int selected = -1;

                for( int count=ListView_GetItemCount(GetDlgItem(hWnd,IDC_LISTVIEW))-1; count>=0; count-- ) {
                    if( ListView_GetItemState( GetDlgItem(hWnd,IDC_LISTVIEW), count, LVIS_SELECTED ) ) {
                        selected = LvItemToDriverIndex( GetDlgItem(hWnd,IDC_LISTVIEW), count );
                        break;
                    }
                }

                EndDialog( hWnd, selected );
            }
            else if( id == IDCANCEL ) {
                // Exit without selection
                EndDialog( hWnd, -1 );
            }
        }
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

// Loads a new driver
bool loadGame( HWND hWindow, TMachine * m = 0 )
{
    bool result = false;

    // Pause the current driver
    dxApp->setActive( false );

    // If we were not given a machine to load, let the user choose one
    if( m == 0 ) {
        int index = DialogBox( GetModuleHandle(0), MAKEINTRESOURCE(IDD_GAME_LIST), hWindow, GameListDialogProc );

        const TGameRegistryItem * t = TGameRegistry::instance().item( index );

        if( t != 0 ) {
            m = TMachine::createInstance( t->factory() );
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

            if( IDYES == MessageBox( NULL, msg.cstr(), szAppName, MB_YESNOCANCEL | MB_ICONWARNING ) ) {
                ok = true;
            }
            else {
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

            dxApp->setVideoParams( info->machineInfo()->screenWidth, info->machineInfo()->screenHeight );
            dxApp->setAudioParams( SamplesPerSecond / info->machineInfo()->framesPerSecond, SamplesPerSecond );
            dxApp->getCanvas()->clear();
            dxApp->prepareGameScreen();

            // Create a new frame buffer for this machine
            delete frame;
            frame = new WinFrame( dxApp->getCanvas(), dxApp );

            // Update options dialog if still open
            if( hDlgOptions != NULL ) {
                SendMessage( hDlgOptions, WM_OPTIONS_CHANGED, 0, (LPARAM) makeOptionsParam(machine) );
            }

            result = true;
        }
    }

    // Resume the current driver
    dxApp->setActive( true );

    return result;
}

void generateDriverList( const char * filename )
{
#if 0
    TGameRegistry::instance().sort();

    FILE * f = fopen( filename, "w" );

    fprintf( f, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n" );
    fprintf( f, "<HTML>\n" );
    fprintf( f, "<HEAD>\n" );
    fprintf( f, "<TITLE>Tickle driver list</TITLE>\n" );
    fprintf( f, "</HEAD>\n" );
    fprintf( f, "<BODY>\n" );

    for( int index=0; index<TGameRegistry::instance().count(); index++ ) {
        const TGameRegistryItem * t = TGameRegistry::instance().item( index );

        if( t != 0 ) {
            TMachine * m = TMachine::createInstance( t->factory() );

            const TMachineDriverInfo * info = m->getDriverInfo();

            if( index > 0 ) {
                fprintf( f, "%s", "\n<p><HR ALIGN=\"CENTER\" WIDTH=\"75%\" NOSHADE>\n" );
            }

            fprintf( f, "\n<p><b>%s</b> (%s, %d)\n",
                info->machineInfo()->name,
                info->machineInfo()->manufacturer,
                info->machineInfo()->year );

            fprintf( f, "<p><b>Resources:</b> " );

            for( int j=0; j<m->getResourceCount(); j++ ) {
                if( j > 0 ) {
                    fprintf( f, ", " );
                }
                fprintf( f, "%s.zip", m->getResourceName( j ) );
            }

            fprintf( f, "<br>\n<b>Files:</b> " );
            for( int i=0; i<info->resourceFileCount(); i++ ) {
                const TResourceFileInfo * file = info->resourceFile( i );

                if( i > 0 ) {
                    fprintf( f, ", " );
                }
                fprintf( f, "%s", file->name );
            }

            delete m;
        }
    }

    fprintf( f, "\n\n</BODY>\n" );
    fprintf( f, "</HTML>\n" );

    fclose( f );
#endif
}

// Game options dialog box procedure
BOOL CALLBACK GameOptionsDialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg ) {
    case WM_INITDIALOG:
        {
            GameOptionsParam * param = (GameOptionsParam *) lParam;

            SetWindowLong( hWnd, GWL_USERDATA, (LONG) param );

            // Load the list box
            SendDlgItemMessage( hWnd, IDC_OPTION_LIST, LB_RESETCONTENT, 0, 0 );

            for( int i=0; i<param->options->count(); i++ ) {
                TUiOption * option = (TUiOption *) param->options->item(i);

                SendDlgItemMessage( hWnd, IDC_OPTION_LIST, LB_ADDSTRING, 0, (LPARAM) option->name() );
            }

            // Set the current selection
            SendDlgItemMessage( hWnd, IDC_OPTION_LIST, LB_SETCURSEL, 0, 0 );
            SendMessage( hWnd, WM_COMMAND, MAKEWPARAM(IDC_OPTION_LIST,LBN_SELCHANGE), 0 );

            // Try to place the dialog so that it doesn't overlap with the main window
            RECT rcMain;
            RECT rcDialog;
            RECT rc;

            GetWindowRect( param->hWndParent, &rcMain );
            GetWindowRect( hWnd, &rcDialog );

            IntersectRect( &rc, &rcMain, &rcDialog );
            if( ! IsRectEmpty( &rc ) ) {
                RECT rcWorkArea;
                SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWorkArea, 0 );

                // Try to move the window to the right of the main window
                OffsetRect( &rcDialog, rcMain.right - rcDialog.left + 4, rcMain.top - rcDialog.top );
                if( rcDialog.right > rcWorkArea.right ) {
                    // Nope, try moving it to the left
                    GetWindowRect( hWnd, &rcDialog );
                    OffsetRect( &rcDialog, rcMain.left - rcDialog.right - 4, rcMain.top - rcDialog.top );

                    if( rcDialog.left < 0 ) {
                        // Ok, leave it alone then!
                        GetWindowRect( hWnd, &rcDialog );
                    }
                }

                SetWindowPos( hWnd, NULL, rcDialog.left, rcDialog.top, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER );
            }
        }
        break;
    case WM_OPTIONS_CHANGED:
        {
            GameOptionsParam * param = (GameOptionsParam *) GetWindowLong( hWnd, GWL_USERDATA );
            delete param->options;
            delete param;

            SendMessage( hWnd, WM_INITDIALOG, wParam, lParam );
        }
        break;
    case WM_CLOSE:
        DestroyWindow( hWnd );
        break;
    case WM_DESTROY:
        {
            GameOptionsParam * param = (GameOptionsParam *) GetWindowLong( hWnd, GWL_USERDATA );
            delete param->options;
            delete param;
            hDlgOptions = NULL;
        }
        break;
    case WM_COMMAND:
        {
            WORD id = LOWORD(wParam);

            if( id == IDOK || id == IDCANCEL ) {
                // Close dialog
                DestroyWindow( hWnd );
            }
            else {
                // Initialize some variables that are tipically useful here
                GameOptionsParam * param = (GameOptionsParam *) GetWindowLong( hWnd, GWL_USERDATA );
                LRESULT selected = SendDlgItemMessage( hWnd, IDC_OPTION_LIST, LB_GETCURSEL, 0, 0 );
                TUiOption * option = (selected != LB_ERR) ? (TUiOption *) param->options->item( selected ) : 0;

                if( option == 0 )
                    break;

                if( id == IDC_OPTION_DEFAULT ) {
                    // Select default value
                    option->restoreDefault();
                    SendDlgItemMessage( hWnd, IDC_OPTION_VALUES, LB_SETCURSEL, option->selected(), 0 );
                }
                else if( (id == IDC_OPTION_LIST) && (HIWORD(wParam) == LBN_SELCHANGE) ) {
                    // Load values into list box
                    SendDlgItemMessage( hWnd, IDC_OPTION_VALUES, LB_RESETCONTENT, 0, 0 );
                
                    for( int i=0; i<option->count(); i++ ) {
                        SendDlgItemMessage( hWnd, IDC_OPTION_VALUES, LB_ADDSTRING, 0, (LPARAM) option->item(i) );
                    }

                    SendDlgItemMessage( hWnd, IDC_OPTION_VALUES, LB_SETCURSEL, option->selected(), 0 );

                    // Set caption
                    TString caption;

                    switch( TInput::getDeviceType(option->id()) ) {
                    case dtDipSwitch:
                        caption = "DIP switch:\r\n";
                        break;
                    case dtDriverOption:
                        caption = "Driver option:\r\n";
                        break;
                    case dtCheat:
                        caption = "Cheat:\r\n";
                        break;
                    case dtBoardSwitch:
                        caption = "Board switch:\r\n";
                        break;
                    }

                    caption += option->name();

                    SetDlgItemText( hWnd, IDC_OPTION_LABEL, caption.cstr() );
                }
                else if( (id == IDC_OPTION_VALUES) && (HIWORD(wParam) == LBN_SELCHANGE) ) {
                    // Reflect the selection change in the option list
                    selected = SendDlgItemMessage( hWnd, IDC_OPTION_VALUES, LB_GETCURSEL, 0, 0 );
                    assert( selected != LB_ERR );

                    option->select( selected );
                    option->notify( param->machine );
                }
            }
        }
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

// Let the user edit the options (e.g. DIP switches) for the current driver
void setGameOptions( HWND hWindow )
{
    if( hDlgOptions != NULL )
        return;

    // Create the (modeless) dialog box
    hDlgOptions = CreateDialogParam( GetModuleHandle(0), 
        MAKEINTRESOURCE(IDD_GAME_OPTIONS),
        hWindow, 
        GameOptionsDialogProc, 
        (LPARAM) makeOptionsParam(machine) );

    ShowWindow( hDlgOptions, SW_SHOW );
}

void loadConfiguration()
{
    const char * sInifile = ".\\tickle.ini";
    const char * sOptions = "Options";
    const char * sForceFeedbackStrength = "ForceFeedbackStrength";

    UINT u = GetPrivateProfileInt( sOptions, sForceFeedbackStrength, DxJoystick::getForceFeedbackStrength(), sInifile );

    DxJoystick::setForceFeedbackStrength( u );
}

// Main window procedure
LRESULT PASCAL WndProc( HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam )
{
    LRESULT result = 0;

    bool handled = dxApp->handleWindowMessage( hWnd, wMsg, wParam, lParam, &result );

    if( handled )
        return result;

    switch( wMsg ) {
        // Menu commands
        case WM_COMMAND:
            switch( LOWORD(wParam) ) {
            case ID_FILE_LOAD:
                loadGame( hWnd );
                break;
            case ID_EXIT:
                PostMessage( hWnd, WM_CLOSE, 0, 0 );
                break;
            case ID_GAME_RESTART:
                machine->reset();
                break;
            case ID_GAME_OPTIONS:
                setGameOptions( hWnd );
                break;
            case ID_HELP_CONTROLS:
                {
                    TString s = "Left\tP1 Left\n"
                       "Right\tP1 Right\n"
                       "Up\tP1 Up\n"
                       "Down\tP1 Down\n"
                       "LCtrl\tP1 Action1\n"
                       "Z\tP1 Action2\n"
                       "X\tP1 Action3\n"
                       "C\tP1 Action4\n\n"
                       "J\tP2 Left\n"
                       "L\tP2 Right\n"
                       "I\tP2 Up\n"
                       "K\tP2 Down\n"
                       "E\tP2 Action1\n"
                       "R\tP2 Action2\n"
                       "T\tP2 Action3\n"
                       "G\tP2 Action4\n\n"
                       "1\tP1 Start\n"
                       "2\tP2 Start\n\n"
                       "5\tCoin slot 1\n"
                       "6\tCoin slot 2\n\n"
                       "P\tPause\n"
                       "F2\tToggle window size";

                    MessageBox( 0, s.cstr(), "Controls", MB_OK | MB_ICONINFORMATION );
                }
                break;
            case ID_HELP_ABOUT:
                MessageBox( 0, szAbout, szAppName, MB_OK | MB_ICONINFORMATION );
                break;
            }
            break;
        // Keyboard events
        case WM_KEYDOWN:
            switch( wParam ) {
            case 'P':
                dxApp->setActive( ! dxApp->getActive() );

                {
                    TString caption( szAppName );

                    if( ! dxApp->getActive() ) {
                        caption += " - PAUSED";
                    }

                    SetWindowText( hWnd, caption.cstr() );
                }
                break;
            case VK_ESCAPE:
                PostMessage( hWnd, WM_CLOSE, 0, 0 );
                return 0;
            case VK_F2:
                sizeFactor = (sizeFactor + 1) % 3;
                dxApp->setWindowSize( sizeFactor );
                break;
            case VK_F4:
                dxApp->copySnapshotToClipboard();
                break;
            case VK_F11:
                generateDriverList( "driver_info.htm" );
                break;
            default:
                inputManager.handle( wParam, 1, machine );
            }
            break;
        case WM_KEYUP:
            inputManager.handle( wParam, 0, machine );
            break;
    }

    return DefWindowProc( hWnd, wMsg, wParam, lParam );
}

// Runs the game engine for one frame.
void CALLBACK runOneFrame( void )
{
    // For now, only 2 joysticks are supported
    for( int i=0; i<2; i++ ) {
        DxJoystick * joystick = dxApp->joystick( i );

        if( joystick != 0 ) {
            // Set the joystick range
            joystick->setRange( jpHalfRange );

            if( joystick->poll() ) {
                joy[i]->setPosition( joystick->x(), joystick->y() );
                joy[i]->setButtons( joystick->buttons() );
                joy[i+2]->setPosition( joystick->rx(), joystick->ry() );
            }
        }
    }

    inputManager.notifyJoysticks( machine );

    static unsigned bufferIndex = 0;

    // Run the machine for a frame
    unsigned samplingRate = dxApp->getSamplingRate();
    unsigned samplesPerFrame = samplingRate / machine->getDriverInfo()->machineInfo()->framesPerSecond;

    frame->getMixer()->clear();
    machine->run( frame, samplesPerFrame, samplingRate );

    // Write into the sound buffer
    dxApp->synchToSoundBuffer( bufferIndex );
    dxApp->writeToSoundBuffer( bufferIndex, frame->getMixer()->buffer(0), frame->maxVoices() );

    // Blit last frame and draw current in the back buffer
    dxApp->blitGameScreen();
    dxApp->prepareGameScreen();

    // Flip the buffer!
    bufferIndex = dxApp->getNextFrame( bufferIndex );
}

/*
    Application main.
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow )
{
    InitCommonControls();

    MSG         stMsg;
    WNDCLASS    stWndClass;

    /* Setup window class structure */
    stWndClass.style = CS_DBLCLKS;
    stWndClass.lpfnWndProc = WndProc;
    stWndClass.cbClsExtra = 0;
    stWndClass.cbWndExtra = 0;
    stWndClass.hInstance = hInstance;
    stWndClass.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN) );
    stWndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
    stWndClass.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
    stWndClass.lpszMenuName = MAKEINTRESOURCE( IDM_MAIN );
    stWndClass.lpszClassName = "EmuWindowClass";

    /* Register window class */
    if( ! RegisterClass( &stWndClass ) )
        return 0;

    /* Create main window */
    hWindow = CreateWindowEx( 0,            // Extended style
        stWndClass.lpszClassName,           // Class
        szAppName,                          // Title
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,   // Style
        CW_USEDEFAULT,                      // X
        CW_USEDEFAULT,                      // Y
        160,                                // Width
        120,                                // Height
        NULL,                               // Parent or owner window
        NULL,                               // Menu or child window id
        hInstance,                          // Instance handle
        NULL );                             // Pointer to window creation data

    HACCEL hAccelerators = LoadAccelerators( hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR) );

    /* Create DirectX application helper */
    dxApp = new DxApp( hWindow );

    setInputManager( 0 );

    TMachine * defaultMachine = TTickleMachine::create();
    if( dxApp->initialize() && loadGame( hWindow, defaultMachine ) ) {
        // Load configuration parameters
        loadConfiguration();

        // Sort the list of available drivers
        TGameRegistry::instance().sort();
        
        // Show the window and run the application
        dxApp->showWindow();

        while( true ) {
            if( PeekMessage( &stMsg, NULL, 0, 0, PM_NOREMOVE ) ) {
                if( ! GetMessage( &stMsg, NULL, 0, 0 ) )
                    break;

                if( ! IsDialogMessage( hDlgOptions, &stMsg ) ) {
                    if( ! TranslateAccelerator( hWindow, hAccelerators, &stMsg ) ) {
                        TranslateMessage( &stMsg );
                        DispatchMessage( &stMsg );
                    }
                }
            }
            else if( dxApp->isActive() ) {
                runOneFrame();
            }
            else {
                WaitMessage();
            }
        }
    }

    delete dxApp;
    delete machine;
    delete frame;

    return 0;
}
