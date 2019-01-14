/*
     Tickle front-end
     User interface for Mac OS X
     
     Copyright (c) 2006 Alessandro Scotti
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
#include <Carbon/Carbon.h>

#include <emu/emu.h>

enum {
    kSignature = 'OPTS',
    kOptionListId = 130,
    kValueListId = 131,
    kValueNameId = 132,
    kValueTypeId = 133,
};

struct OptionsData
{
    WindowRef window;
    TMachine * machine;
    ControlRef dbOptions;
    ControlRef dbOptionValues;
    ControlRef textValueName;
    ControlRef textValueType;
    int currOption;

    int getOptionCount();
    const TUiOption * getOptionAt( int index );
};

// Module global: keeps information about the currently displayed window
static OptionsData * data = 0;

int OptionsData::getOptionCount()
{
    int result = 0;
    TUserInterface * ui = machine->getDriverInfo()->userInterface();

    for( int index=0; index < ui->groupCount(); index++ ) {
        TUiOptionGroup * group = ui->group(index);

        result += group->count();
    }
    
    return result;
}

const TUiOption * OptionsData::getOptionAt( int index )
{
    TUserInterface * ui = machine->getDriverInfo()->userInterface();
    
    for( int i=0; i < ui->groupCount(); i++ ) {
        TUiOptionGroup * group = ui->group(i);

        for( int j=0; j<group->count(); j++ ) {
            if( index == 0 ) {
                return group->item(j);
            }
            index--;
        }
    }

    return 0;
}

// Invoked by the options listbox (i.e. data browser) to fetch item information
static OSStatus OptionListDataCallback(ControlRef browser, DataBrowserItemID itemId, DataBrowserPropertyID propertyId, DataBrowserItemDataRef itemData, Boolean changeValue)
{  
    OSStatus result = errDataBrowserPropertyNotSupported;
    
    if( ! changeValue && itemId >= 1 ) {
        if( data->getOptionAt( itemId-1 ) != 0 ) {
            CFStringRef sref = CFStringCreateWithCString( 0, data->getOptionAt( itemId-1 )->name(), kCFStringEncodingASCII );
            result = SetDataBrowserItemDataText( itemData, sref );
            CFRelease(sref);
        }
    }
    
    return result;
}

// Invoked by the option values listbox (i.e. data browser) to fetch item information
static OSStatus OptionValueListDataCallback(ControlRef browser, DataBrowserItemID itemId, DataBrowserPropertyID propertyId, DataBrowserItemDataRef itemData, Boolean changeValue)
{  
    OSStatus result = errDataBrowserPropertyNotSupported;
    
    if( ! changeValue && itemId >= 1 ) {
        const TUiOption * option = data->getOptionAt( data->currOption );

        if( option != 0 ) {
            CFStringRef sref = CFStringCreateWithCString( 0, option->item( itemId-1 ), kCFStringEncodingASCII );
            result = SetDataBrowserItemDataText( itemData, sref );
            CFRelease(sref);
        }            
    }
    
    return result;
}

// Invoked when an item is selected
static void OptionValueListNotificationCallback(ControlRef browser, DataBrowserItemID itemId, DataBrowserItemNotification message)
{
    if( message == kDataBrowserItemSelected ) {
        TUiOption * option = (TUiOption *) data->getOptionAt( data->currOption );

        if( option != 0 && itemId >= 1 && itemId <= option->count() ) {
            option->select( itemId-1 );
            option->notify( data->machine );
        }            
    }
}

// Invoked when an item is selected
static void OptionListNotificationCallback(ControlRef browser, DataBrowserItemID itemId, DataBrowserItemNotification message)
{
    if( message == kDataBrowserItemSelected ) {
        data->currOption = itemId - 1;
        
        const TUiOption * option = data->getOptionAt( itemId-1 );

        // Update option values list
        RemoveDataBrowserItems( data->dbOptionValues, kDataBrowserNoItem, 0, NULL, kDataBrowserItemNoProperty );
         
        if( option != 0 && option->count() > 0 ) {
            // Set value list
            DataBrowserItemID selection = option->selected() + 1;
            
            AddDataBrowserItems( data->dbOptionValues, kDataBrowserNoItem, option->count(), NULL, kDataBrowserItemNoProperty );
            
            SetDataBrowserSelectedItems( data->dbOptionValues, 1, &selection, kDataBrowserItemsAssign );

            // Set value labels
            const char * type = "(N/A)";
            
            switch( TInput::getDeviceType(option->id()) ) {
                case dtDipSwitch:
                    type = "(DIP switch)";
                    break;
                case dtDriverOption:
                    type = "(Driver option)";
                    break;
                case dtCheat:
                    type = "(Cheat)";
                    break;
                case dtBoardSwitch:
                    type = "(Board switch)";
                    break;
            }
            
            CFStringRef sref = CFStringCreateWithCString( 0, option->name(), kCFStringEncodingASCII );
            SetControlData( data->textValueName, kControlEntireControl, kControlStaticTextCFStringTag, sizeof(sref), &sref );
            CFRelease(sref);
            sref = CFStringCreateWithCString( 0, type, kCFStringEncodingASCII );
            SetControlData( data->textValueType, kControlEntireControl, kControlStaticTextCFStringTag, sizeof(sref), &sref );
            CFRelease(sref);
        }
    }
}

static int SetupOptionsWindow( WindowRef mainWindow )
{
    DataBrowserItemID selection = 1;
    
    CFStringRef sref = CFStringCreateWithCString( 0, "", kCFStringEncodingASCII );
    SetControlData( data->textValueName, kControlEntireControl, kControlStaticTextCFStringTag, sizeof(sref), &sref );
    SetControlData( data->textValueType, kControlEntireControl, kControlStaticTextCFStringTag, sizeof(sref), &sref );
    CFRelease(sref);
    
    // Refresh content of data browser
    RemoveDataBrowserItems( data->dbOptions, kDataBrowserNoItem, 0, NULL, kDataBrowserItemNoProperty );
    AddDataBrowserItems( data->dbOptions, kDataBrowserNoItem, data->getOptionCount(), NULL, kDataBrowserItemNoProperty );
    
    SetDataBrowserSelectedItems( data->dbOptions, 1, &selection, kDataBrowserItemsAssign );
    
    // Position the window properly
    Rect screen;
    Rect parentBounds;
    Rect bounds;
    
    GetWindowGreatestAreaDevice( data->window, kWindowStructureRgn, NULL, &screen );
    GetWindowBounds( mainWindow, kWindowStructureRgn, &parentBounds );
    GetWindowBounds( data->window, kWindowStructureRgn, &bounds );
    
    int width = bounds.right - bounds.left;
    int height = bounds.bottom - bounds.top;
    
    bounds.top = parentBounds.top;
    bounds.bottom = bounds.top + height;
    bounds.left = parentBounds.right + 8;
    bounds.right = bounds.left + width;
    
    if( bounds.right > screen.right ) {
        bounds.right = screen.right;
        bounds.left = bounds.right - width;
    }

    SetWindowBounds( data->window, kWindowStructureRgn, &bounds );
    
    return 0;
}

// Handles event for the driver options window
static OSStatus OptionsWindowEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void * inRefcon )
{
    OSStatus err = eventNotHandledErr;
    UInt32 eventClass = GetEventClass( inEvent );
    UInt32 eventKind = GetEventKind( inEvent );

    if( eventClass == kEventClassCommand ) {
        HICommandExtended cmd;
        verify_noerr( GetEventParameter( inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof( cmd ), NULL, &cmd ) );
     
        if( eventKind == kEventCommandProcess ) {
            switch ( cmd.commandID ) {
                case kHICommandOK:
                    DisposeWindow( data->window );
                    break;
                case 'RESE':
                    TUiOption * option = (TUiOption *) data->getOptionAt( data->currOption );
                    if( option != 0 ) {
                        option->restoreDefault();
                        DataBrowserItemID selection = option->selected() + 1;
                        SetDataBrowserSelectedItems( data->dbOptionValues, 1, &selection, kDataBrowserItemsAssign );
                    }
                    break;
            }
        }
    }
    else if( eventClass = kEventClassWindow ) {
        if( eventKind == kEventWindowClosed ) {
            delete data;
            data = 0;
        }
    }
    
    return err;
}

bool IsOptionsWindowActive()
{
    return data != 0;
}

DEFINE_ONE_SHOT_HANDLER_GETTER( OptionsWindowEventHandler );

int ShowOptionsWindow( IBNibRef sNibRef, TMachine * machine, WindowRef mainWindow )
{
    static const EventTypeSpec kWindowEvents[] = {
        { kEventClassCommand, kEventCommandProcess },
        { kEventClassWindow, kEventWindowClosed }
    };
    
    const ControlID dbOptionsID = { kSignature, kOptionListId };
    const ControlID dbOptionValuesID = { kSignature, kValueListId };
    const ControlID textValueNameID = { kSignature, kValueNameId };
    const ControlID textValueTypeID = { kSignature, kValueTypeId };
    
    // If window already displayed, just activate it
    if( IsOptionsWindowActive() ) {
        data->machine = machine;
        SetupOptionsWindow( mainWindow );
        SelectWindow( data->window );
        return 0;
    }
    
    int result = -1;
    WindowRef window;
    OSStatus err;
    DataBrowserCallbacks dbCallbacks;
    
    // Create load driver window
    err = CreateWindowFromNib( sNibRef, CFSTR("OptionsWindow"), &window );
    
    if( err == noErr ) {
        // Setup data
        data = new OptionsData;
        data->window = window;
        data->machine = machine;
        GetControlByID( window, &dbOptionsID, &data->dbOptions );
        GetControlByID( window, &dbOptionValuesID, &data->dbOptionValues );
        GetControlByID( window, &textValueNameID, &data->textValueName );
        GetControlByID( window, &textValueTypeID, &data->textValueType );
        data->currOption = -1;
        
        // Initialize data browser controls
        dbCallbacks.version = kDataBrowserLatestCallbacks;
        InitDataBrowserCallbacks( &dbCallbacks );
        dbCallbacks.u.v1.itemDataCallback = NewDataBrowserItemDataUPP( (DataBrowserItemDataProcPtr) OptionListDataCallback );
        dbCallbacks.u.v1.itemNotificationCallback = NewDataBrowserItemNotificationUPP( (DataBrowserItemNotificationProcPtr) OptionListNotificationCallback );
        SetDataBrowserCallbacks( data->dbOptions, &dbCallbacks );

        InitDataBrowserCallbacks( &dbCallbacks );
        dbCallbacks.u.v1.itemDataCallback = NewDataBrowserItemDataUPP( (DataBrowserItemDataProcPtr) OptionValueListDataCallback );
        dbCallbacks.u.v1.itemNotificationCallback = NewDataBrowserItemNotificationUPP( (DataBrowserItemNotificationProcPtr) OptionValueListNotificationCallback );
        SetDataBrowserCallbacks( data->dbOptionValues, &dbCallbacks );
        
        SetupOptionsWindow( mainWindow );
        
        // Install event handler
        InstallWindowEventHandler( window, GetOptionsWindowEventHandlerUPP(), GetEventTypeCount( kWindowEvents ), kWindowEvents, window, NULL );
        
        // Show the window
        ShowWindow( window );
        
        result = 0;
    }
    
    return result;
}
