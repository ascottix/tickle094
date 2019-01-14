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
#include "joystick.h"

const int MaxJoysticks = 4;

static int gJoystickCount = 0;
static HidGamePad * gJoystick[4] = { 0 };

static void inspectObject( CFTypeRef object, GamePadInfo * info );

void HidGamePad::poll()
{
    IOHIDEventStruct hidEvent;
    
    // Get x axis
    if( (*hidDeviceInterface_)->getElementValue( hidDeviceInterface_, gp_.xAxisCookie, &hidEvent ) == 0 ) {
        x_ = (int) hidEvent.value;
    }
    
    // Get y axis
    if( (*hidDeviceInterface_)->getElementValue( hidDeviceInterface_, gp_.yAxisCookie, &hidEvent ) == 0 ) {
        y_ = (int) hidEvent.value;
    }
    
    // Get buttons
    buttons_ = 0;
    
    for( int i=0; i<MaxGamePadButtons; i++ ) {
        if( gp_.buttonCookie[i] != 0 ) {
            if( (*hidDeviceInterface_)->getElementValue( hidDeviceInterface_, gp_.buttonCookie[i], &hidEvent ) == 0 ) {
                if( hidEvent.value != 0 ) {
                    buttons_ |= (1 << i);
                }
            }
        }
    }
}

// Looks into the specified dictionary for "interesting" data (in our case, joystick-related cookies)
static void inspectDictionary( CFDictionaryRef dictionary, GamePadInfo * info )
{
    CFTypeRef object;
    long number;
    IOHIDElementCookie cookie;
    long usage;
    long usagePage;

    // Get cookie
    object = CFDictionaryGetValue( dictionary, CFSTR(kIOHIDElementCookieKey) );
    if( object == 0 || CFGetTypeID(object) != CFNumberGetTypeID() )
        return;
    if( ! CFNumberGetValue( (CFNumberRef) object, kCFNumberLongType, &number) )
        return;
    cookie = (IOHIDElementCookie) number;

    // Get usage
    object = CFDictionaryGetValue( dictionary, CFSTR(kIOHIDElementUsageKey) );
    if( object == 0 || CFGetTypeID(object) != CFNumberGetTypeID() )
        return;
    if( ! CFNumberGetValue( (CFNumberRef) object, kCFNumberLongType, &number) )
        return;
    usage = number;

    // Get usage page
    object = CFDictionaryGetValue( dictionary, CFSTR(kIOHIDElementUsagePageKey) );
    if( object == 0 || CFGetTypeID(object) != CFNumberGetTypeID() )
        return;
    if( ! CFNumberGetValue( (CFNumberRef) object, kCFNumberLongType, &number) )
        return;
    usagePage = number;
    
    // Check for gamepad properties
    if( usage == kHIDUsage_GD_X && usagePage == kHIDPage_GenericDesktop ) {
        info->xAxisCookie = cookie;
    }
    else if( usage == kHIDUsage_GD_Y && usagePage == kHIDPage_GenericDesktop ) {
        info->yAxisCookie = cookie;
    }
    else if( usage >= 1 && usage <= 8 && usagePage == kHIDPage_Button ) {
        info->buttonCookie[usage-1] = cookie;
    }
}

// Inspects all members of an array
static void inspectArrayCallback( const void * value, void * parameter )
{
    if( CFGetTypeID(value) == CFDictionaryGetTypeID() ) {
        inspectObject( value, (GamePadInfo *) parameter );
    }
}

// Inspects an object for "interesting" data
static void inspectObject( CFTypeRef object, GamePadInfo * info )
{
    if( object != 0 ) {
        CFTypeID type = CFGetTypeID(object);
        
        if( type == CFArrayGetTypeID() ) {
            // Inspect array
            CFArrayRef array = (CFArrayRef) object;
            CFRange range = { 0, CFArrayGetCount(array) };
            CFArrayApplyFunction( array, range, inspectArrayCallback, info );
        }
        else if( type == CFDictionaryGetTypeID() ) {
            // Inspect dictionary
            CFDictionaryRef dictionary = (CFDictionaryRef) object;
            inspectObject( CFDictionaryGetValue( dictionary, CFSTR(kIOHIDElementKey) ), info );
            inspectDictionary( dictionary, info );
        }
    }
}

// Searches for joysticks into the available input devices
int initJoystickLib() 
{
    // Initialization
    mach_port_t masterPort = 0;
    io_iterator_t hidObjectIterator = 0;

    // Get a Mach port to initiate communication with I/O Kit
    if( IOMasterPort( bootstrap_port, &masterPort ) != kIOReturnSuccess ) {
        printf("Couldn’t create a master I/O Kit Port\n");
        return -1;
    }
    
    // Setup matching dictionary
    UInt32 usagePage = kHIDPage_GenericDesktop;
    UInt32 usage = kHIDUsage_GD_GamePad;
    CFNumberRef refUsage = NULL;
    CFNumberRef refUsagePage = NULL;
    CFMutableDictionaryRef refHIDMatchDictionary = NULL;

    // Set up a matching dictionary to search I/O Registry by class name for all HID class devices.
    refHIDMatchDictionary = IOServiceMatching( kIOHIDDeviceKey );
    if( refHIDMatchDictionary == NULL ) {
        printf("Failed to get HID CFMutableDictionaryRef via IOServiceMatching\n");
        return -2;
    }
    
    // Add key for device type (joystick, in this case) to refine the matching dictionary.
    refUsagePage = CFNumberCreate( kCFAllocatorDefault, kCFNumberIntType, &usagePage );
    refUsage = CFNumberCreate( kCFAllocatorDefault, kCFNumberIntType, &usage );
    CFDictionarySetValue( refHIDMatchDictionary, CFSTR (kIOHIDPrimaryUsagePageKey), refUsagePage );
    CFDictionarySetValue( refHIDMatchDictionary, CFSTR (kIOHIDPrimaryUsageKey), refUsage );
    
    // Now search I/O Registry for matching devices.
    if( IOServiceGetMatchingServices( masterPort, refHIDMatchDictionary, &hidObjectIterator) != kIOReturnSuccess ) {
        printf("Couldn’t create a HID object iterator\n");
        return -3;
    }
    
    io_object_t hidDevice = 0;
    IOHIDDeviceInterface ** pphidDeviceInterface = 0;
    int numDevice = 0;
    
    while( (hidDevice = IOIteratorNext(hidObjectIterator)) != 0 ) {
        CFMutableDictionaryRef properties = 0;
        char path[1024];
        
        if( IORegistryEntryGetPath( hidDevice, kIOServicePlane, path ) != KERN_SUCCESS ) {
            printf( "Cannot get registry entry path for device: %d\n", numDevice );
            continue;
        }

        // Create a CF dictionary representation of the I/O Registry entry properties
        if( IORegistryEntryCreateCFProperties( hidDevice, &properties, kCFAllocatorDefault, kNilOptions ) != KERN_SUCCESS ) {
            printf( "Cannot create properties for device: %d\n", numDevice );
            continue;
        }
        
        // Look for joystick-related information
        CFTypeRef object = CFDictionaryGetValue( properties, CFSTR(kIOHIDElementKey) );
        
        GamePadInfo info;
        
        inspectObject(object,&info);
        
        // Create device interface
        io_name_t className;
        IOCFPlugInInterface **plugInInterface = NULL;
        HRESULT plugInResult = S_OK;
        SInt32 score = 0;
        
        if( IOObjectGetClass (hidDevice, className) == kIOReturnSuccess ) {
            if( IOCreatePlugInInterfaceForService( hidDevice, kIOHIDDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &plugInInterface, &score ) == kIOReturnSuccess ) {
                // Call a method of the intermediate plug-in to create the device interface
                plugInResult = (*plugInInterface)->QueryInterface( plugInInterface, CFUUIDGetUUIDBytes (kIOHIDDeviceInterfaceID), (void **) &pphidDeviceInterface );
                if( plugInResult != S_OK ) {
                    printf("Couldn’t query HID class device interface from plugInInterface (class: %s)\n", className);
                    pphidDeviceInterface = 0;
                }
                (*plugInInterface)->Release (plugInInterface);
            }
        }
        
        // Release the device
        IOObjectRelease(hidDevice);

        // If we have a device interface, try to open it
        if( *pphidDeviceInterface != 0 ) {
            if( (*pphidDeviceInterface)->open(pphidDeviceInterface, 0) == KERN_SUCCESS ) {
                gJoystick[ gJoystickCount ] = new HidGamePad( info, pphidDeviceInterface );
                gJoystickCount++;
            }
        }
        
        // Stop looking if we have enough objects!
        if( gJoystickCount >= MaxJoysticks ) {
            break;
        }
    }
    
    IOObjectRelease (hidObjectIterator);
    
    return 0;
}

int termJoystickLib()
{
    for( int i=0; i<gJoystickCount; i++ ) {
        // Close and release the device interface
        IOHIDDeviceInterface ** devi = gJoystick[i]->deviceInterface();

        (*devi)->close(devi);
        (*devi)->Release(devi);

        // Delete the joystick object
        delete gJoystick[i];
        gJoystick[i] = 0;
    }
    
    gJoystickCount = 0;
    
    return 0;
}

HidGamePad * getJoystick( int index )
{
    HidGamePad * result = 0;
    
    if( index >= 0 && index < gJoystickCount ) {
        result = gJoystick[index];
    }
    
    return result;
}

int getJoystickCount()
{
    return gJoystickCount;
}
