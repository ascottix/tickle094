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
#ifndef UTIL_H_
#define UTIL_H_

#include <emu/emu.h>

extern const char * PathSeparator;

#define RomsDir "roms"
#define SamplesDir "samples"

enum {
    kKeyModAlt      = 0x0800,
    kKeyModApple    = 0x0100,
    kKeyModCaps     = 0x0400,
    kKeyModCtrl     = 0x1000,
    kKeyModShift    = 0x0200
};

// Key values as defined in MacMAME
enum {
	kMacKeyA				= 0x00,
	kMacKeyS				= 0x01,
	kMacKeyD				= 0x02,
	kMacKeyF				= 0x03,
	kMacKeyH				= 0x04,
	kMacKeyG				= 0x05,
	kMacKeyZ				= 0x06,
	kMacKeyX				= 0x07,
	kMacKeyC				= 0x08,
	kMacKeyV				= 0x09,
	kMacKeyB				= 0x0b,
	kMacKeyQ				= 0x0c,
	kMacKeyW				= 0x0d,
	kMacKeyE				= 0x0e,
	kMacKeyR				= 0x0f,
	kMacKeyY				= 0x10,
	kMacKeyT				= 0x11,
	kMacKey1				= 0x12,
	kMacKey2				= 0x13,
	kMacKey3				= 0x14,
	kMacKey4				= 0x15,
	kMacKey6				= 0x16,
	kMacKey5				= 0x17,
	kMacKey9				= 0x19,
	kMacKey7				= 0x1a,
	kMacKey8				= 0x1c,
	kMacKey0				= 0x1d,
	kMacKeyO				= 0x1f,
	kMacKeyU				= 0x20,
	kMacKeyI				= 0x22,
	kMacKeyP				= 0x23,
	kMacKeyReturn			= 0x24,
	kMacKeyL				= 0x25,
	kMacKeyJ				= 0x26,
	kMacKeyK				= 0x28,
	kMacKeyN				= 0x2d,
	kMacKeyM				= 0x2e,
	kMacKeyTab				= 0x30,
	kMacKeySpace			= 0x31,
	kMacKeyTilde			= 0x32,
	kMacKeyBackspace		= 0x33,
	kMacKeyEscape			= 0x35,
	kMacKeyCommand			= 0x37,
	kMacKeyLeftShift		= 0x38,
	kMacKeyLeftOption		= 0x3a,
	kMacKeyLeftControl		= 0x3b,
	kMacKeyRightShift		= 0x3c,
	kMacKeyRightOption		= 0x3d,
	kMacKeyRightControl		= 0x3e,
	kMacKeyKeypad0			= 0x52,
	kMacKeyKeypad1			= 0x53,
	kMacKeyKeypad2			= 0x54,
	kMacKeyKeypad3			= 0x55,
	kMacKeyKeypad4			= 0x56,
	kMacKeyKeypad5			= 0x57,
	kMacKeyKeypad6			= 0x58,
	kMacKeyKeypad7			= 0x59,
	kMacKeyKeypad8			= 0x5b,
	kMacKeyKeypad9			= 0x5c,
	kMacKeyF1				= 0x7a,
	kMacKeyF2				= 0x78,
	kMacKeyF3				= 0x63,
	kMacKeyF4				= 0x76,
	kMacKeyF5				= 0x60,
	kMacKeyF6				= 0x61,
	kMacKeyF7				= 0x62,
	kMacKeyF8				= 0x64,
	kMacKeyF9				= 0x65,
	kMacKeyF10				= 0x6d,
	kMacKeyF11				= 0x67,
	kMacKeyF12				= 0x6f,
	kMacKeyInsert			= 0x72,
	kMacKeyHome				= 0x73,
	kMacKeyPageUp			= 0x74,
	kMacKeyDel				= 0x75,
	kMacKeyEnd				= 0x77,
	kMacKeyPageDown			= 0x79,
	kMacKeyLeft				= 0x7b,
	kMacKeyRight			= 0x7c,
	kMacKeyDown				= 0x7d,
	kMacKeyUp				= 0x7e,
	kMacKeyMinus            = 0x1B,
	kMacKeyEquals           = 0x18,
	kMacKeyOpenBrace        = 0x21,
	kMacKeyCloseBrace       = 0x1E,
	kMacKeyColon            = 0x29,
	kMacKeyQuote            = 0x27,
	kMacKeyBackslash        = 0x2A,
	kMacKeyComma            = 0x2B,
	kMacKeyStop				= 0x2F,
	kMacKeySlash            = 0x2C,
	kMacKeyKeypadSlash      = 0x4B,
	kMacKeyKeypadAsterisk   = 0x43,
	kMacKeyKeypadMinus      = 0x4E,
	kMacKeyKeypadPlus       = 0x45,
	kMacKeyKeypadStop		= 0x41,
	kMacKeyKeypadEnter		= 0x4C,
	kMacKeyPrintScreen		= 0x69,
	kMacKeyPause			= 0x71,
	kMacKeyScreenLock		= 0x6B,
	kMacKeyNumLock			= 0x47,
	kMacKeyCapsLock			= 0x39,
};    

// Loads a file from an input stream into a newly allocated buffer (which must be deleted by the caller)
unsigned char * loadFileFromStream( TInputStream * is, unsigned size, TCRC32 * crc );

// Returns the size of the specified file
unsigned getFileSize( const char * name );

// Loads a single file required by the driver
unsigned char * loadFile( TMachine * machine, const TResourceFileInfo * info, const char * base, unsigned * bufsize );

// Loads all the files required by the driver
bool loadMachineFiles( TMachine * machine, TList & failedList );

#endif // UTIL_H_
