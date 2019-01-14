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
    kColumnName = 'NAME',
    kColumnManufacturer = 'MANU',
    kColumnYear = 'YEAR'
};

// Window handle
static WindowRef window = 0;

// Invoked by the driver listbox (i.e. data browser) to fetch item information
static OSStatus LoadDriverDataCallback(ControlRef browser, DataBrowserItemID itemId, DataBrowserPropertyID propertyId, DataBrowserItemDataRef itemData, Boolean changeValue)
{  
    OSStatus result = errDataBrowserPropertyNotSupported;
    TGameRegistry & gameRegistry = TGameRegistry::instance();
    
    if( ! changeValue && itemId >= 1 && itemId <= gameRegistry.count() ) {
        const TMachineInfo * info = gameRegistry.item(itemId-1)->info();
        CFStringRef sref = 0;
        char buffer[16];
        
        switch( propertyId ) {
            case kColumnName:
                sref = CFStringCreateWithCString( 0, info->name, kCFStringEncodingASCII );
                if( sref != 0 ) {
                    result = SetDataBrowserItemDataText( itemData, sref );
                }
                break;
            case kColumnManufacturer:
                sref = CFStringCreateWithCString( 0, info->manufacturer, kCFStringEncodingASCII );
                if( sref != 0 ) {
                    result = SetDataBrowserItemDataText( itemData, sref );
                }
                break;
            case kColumnYear:
                sprintf( buffer, "%d", info->year );
                sref = CFStringCreateWithCString( 0, buffer, kCFStringEncodingASCII );
                result = SetDataBrowserItemDataText( itemData, sref );
                break;
        }
        
        if( sref != 0 ) {
            CFRelease(sref);
        }
        
        result = noErr;
    }
    
    return result;
}

// Invoked by the driver listbox (i.e. data browser) to compare items for sorting
static Boolean LoadDriverCompareCallback(ControlRef browser, DataBrowserItemID itemId1, DataBrowserItemID itemId2, DataBrowserPropertyID propertyId)
{
    Boolean result = true;
    TGameRegistry & gameRegistry = TGameRegistry::instance();
    const TMachineInfo * info1 = gameRegistry.item(itemId1-1)->info();
    const TMachineInfo * info2 = gameRegistry.item(itemId2-1)->info();
    
    if( info1 != 0 && info2 != 0 ) {
        switch( propertyId ) {
            case kColumnName:
                result = strcmp( info1->name, info2->name ) < 0;
                break;
            case kColumnManufacturer:
                result = strcmp( info1->manufacturer, info2->manufacturer ) < 0;
                break;
            case kColumnYear:
                result = info1->year < info2->year;
                break;
        }
    }
    
    return result;
}

// Invoked when an item is double-clicked
static void LoadDriverNotificationCallback(ControlRef browser, DataBrowserItemID itemId, DataBrowserItemNotification message)
{
    if( message == kDataBrowserItemDoubleClicked ) {
        SetWRefCon(window,kHICommandOK);
        QuitAppModalLoopForWindow(window);
    }
}

// Handles event for the load driver window
static OSStatus LoadWindowEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void * inRefcon )
{
    OSStatus err = eventNotHandledErr;
    WindowRef window = (WindowRef) inRefcon;
    
    if( GetEventClass( inEvent ) == kEventClassCommand ) {
        HICommandExtended cmd;
        verify_noerr( GetEventParameter( inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof( cmd ), NULL, &cmd ) );

        if( GetEventKind( inEvent ) == kEventCommandProcess ) {
            switch ( cmd.commandID ) {
                case kHICommandOK:
                case kHICommandCancel:
                    SetWRefCon(window,cmd.commandID);
                    QuitAppModalLoopForWindow(window);
                    err = noErr;
                    break;
                default:
                    break;
            }
        }
    }
    
    return err;
}

DEFINE_ONE_SHOT_HANDLER_GETTER( LoadWindowEventHandler );

// Allows user to select a driver from the list, returns a non-negative index on success
int GetDriverFromUser( IBNibRef sNibRef )
{
    static const EventTypeSpec kWindowEvents[] = {
        { kEventClassCommand, kEventCommandProcess }
    };
    
    const ControlID dbControlID = { 'LIST', 129 };
    
    int result = -1;
    OSStatus err;
    ControlRef dbControl;
    DataBrowserCallbacks dbCallbacks;
    DataBrowserItemID selection = 1;
    TGameRegistry & gameRegistry = TGameRegistry::instance();
    
    gameRegistry.sort();
    
    // Create load driver window
    err = CreateWindowFromNib( sNibRef, CFSTR("LoadWindow"), &window );
    
    if( err == noErr ) {
        // Initialize data browser control
        GetControlByID( window, &dbControlID, &dbControl );
        
        dbCallbacks.version = kDataBrowserLatestCallbacks;
        InitDataBrowserCallbacks( &dbCallbacks );
        dbCallbacks.u.v1.itemDataCallback = NewDataBrowserItemDataUPP( (DataBrowserItemDataProcPtr) LoadDriverDataCallback );
        dbCallbacks.u.v1.itemCompareCallback = NewDataBrowserItemCompareUPP( (DataBrowserItemCompareProcPtr) LoadDriverCompareCallback );
        dbCallbacks.u.v1.itemNotificationCallback = NewDataBrowserItemNotificationUPP( (DataBrowserItemNotificationProcPtr) LoadDriverNotificationCallback );
        SetDataBrowserCallbacks( dbControl, &dbCallbacks );
        
        SetDataBrowserSortProperty( dbControl, kColumnName );
        SetDataBrowserSortOrder( dbControl, kDataBrowserOrderIncreasing );
        
        AddDataBrowserItems( dbControl, kDataBrowserNoItem, gameRegistry.count(), NULL, kDataBrowserItemNoProperty );
        
        SetDataBrowserSelectedItems( dbControl, 1, &selection, kDataBrowserItemsAssign );
        
        SetKeyboardFocus( window, dbControl, kControlFocusNextPart );
        
        // Install event handler
        InstallWindowEventHandler( window, GetLoadWindowEventHandlerUPP(), GetEventTypeCount( kWindowEvents ), kWindowEvents, window, NULL );
        
        // Position new windows in a staggered arrangement on the main screen
        RepositionWindow( window, NULL, kWindowCascadeOnMainScreen );
        
        // Display modal window
        ShowWindow( window );
        RunAppModalLoopForWindow( window );
        
        // If user choose OK, get index of selected item
        if( GetWRefCon(window) == kHICommandOK ) {
            for( selection=1; selection<=gameRegistry.count(); selection++ ) {
                if( IsDataBrowserItemSelected( dbControl, selection ) ) {
                    result = selection - 1;
                    break;
                }
            }
        }
        
        DisposeDataBrowserItemNotificationUPP( dbCallbacks.u.v1.itemNotificationCallback );
        DisposeDataBrowserItemCompareUPP( dbCallbacks.u.v1.itemCompareCallback );
        DisposeDataBrowserItemDataUPP( dbCallbacks.u.v1.itemDataCallback );
        
        DisposeWindow(window);
        window = 0;
    }
    
    return result;
}
