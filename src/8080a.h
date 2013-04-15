/*******************************************************************************

8080A simulator
Copyright (C) 2002  Anoakie Ray Turner

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
USA.


Note:
----
  This file assumes that the user can access the source file for details about
the main8080a, LoadSet, and ResetProc functions.  If this assumption becomes
untrue, then this file should contain a copy of the appropriate documentation
for the main8080a, LoadSet, and ResetProc functions.


Contents:
--------
  1.	This file references header files which contain functions to get input,
	draw the contents of the 8080a's graphics buffer, and play sounds.
	This file also contains the 8080a's registers, the 8080a's interrupt
	information, the 8080a's default processor values (when reset),
	definitions for the PSW, memory map information, and the memory itself.

*******************************************************************************/


#ifndef __8080A_H__
#define __8080A_H__

/*******************************************************************************
* Included files:
* --------------
*
*   graphics.h - This file contains routines to draw the contents of the
*	8080a's graphics buffer named UpdateScreen.
*
*   ctype.h - This file contains the following function definition needed
*	by 8080a.c: tolower.
*
*******************************************************************************/
#include "graphics.h"
#include <ctype.h>


/*******************************************************************************
* Function:
* --------
* 
*   main8080a - This function is the core of the 8080a simulator.  This
*	function assumes that the ROMs have been loaded with the LoadSet
*	routine. When this function is invoked, it calls ResetProc, and enters
*	the main loop. Once in the main loop, it performs the following tasks
*	in order:  Copy opcode from ROM, add the number of cycles for the
*	selected opcode, decode and execute opcode, and test for interrupt.
*
*******************************************************************************/
void main8080a ();


/*******************************************************************************
* Function:
* --------
* 
*   LoadSet - This function takes a set name and loads that set's ROMs.  Before
* 	loading the set, this function clears the memory where the set will be
* 	loaded.  This function assumes the ROMs will be layed out in a certain
* 	order with a certain directory structure with certain names.  This
*	function returns TRUE if the set was loaded correctly, or FALSE if the
*	set failed to load.
* 
* 
* Directory structure:
* -------------------
* 
* . Root directory
*  \_ roms
*     \_ ROMNAME
*        \_ ROMNAME.h  (Loaded into STARTROM1)
*        \_ ROMNAME.g  (Loaded into STARTROM2)
*        \_ ROMNAME.f  (Loaded into STARTROM3)
*        \_ ROMNAME.e  (Loaded into STARTROM4)
* 
* 
* Example with set invaders:
* -------------------------
* 
* . Root directory
*  \_ roms
*     \_ invaders
*        \_ invaders.h
*        \_ invaders.g
*        \_ invaders.f
*        \_ invaders.e
* 
*******************************************************************************/
Uint8 LoadSet (char *);


/*******************************************************************************
* Function:
* --------
* 
*    ResetProc - This function Resets the 8080a processor and clears all
*	non-ROM	memory.
* 
*******************************************************************************/
void ResetProc ();



/*******************************************************************************
* 8080a Inline Function:
* ---------------------
* 
*    WriteRAM - This function writes the 8-bit data, named data, at the memory
*	location named offset in the 8080a's RAM.  If SELFMOD is not defined,
*	then the code cannot be self modifying.  This means all data below
*	WORKRAM is protected from writes.  If a write occurs to protected data,
*	an error message is written to STDOUT, and the data is NOT written.
*  
* Inputs:
* ------
*
*    offset - This is a 16-bit unsigned integer.  The content of this variable
*	is where the data will be written.
*
*    data - This is an 8-bit unsigned integer.  The content of this variable is
*	the data that will be written.
*
* Endian:
* ------
*
*    If ENDIAN is set, this implies the system is a big endian system and the
*	byte offset written should be swapped.
*
*******************************************************************************/
#ifndef SELFMOD
#define WriteRAM(offset, data) \
	if ( (offset)^(ENDIAN) >= WORKRAM ) { RAM[(offset)^(ENDIAN)] = data; } \
	else { printf ( "!!! Attempted write at offset %d with data" \
			"%d failed.\n", offset, data); }
#else
#define WriteRAM(offset, data) RAM[(offset)^(ENDIAN)] = data;
#endif



/*******************************************************************************
* 8080a Inline Function:
* ---------------------
* 
*    ReadRAM - This function reads 8-bits of data at the memory location named
*	offset in the 8080a's RAM.
*  
* Input:
* -----
*
*    offset - This is a 16-bit unsigned integer.  The content of this variable
*	is where the data will be read.
*
* Output:
* ------
*
*    (return value) - This is an 8-bit unsigned integer that was read from
*	memory.
*
* Endian:
* ------
*
*    If ENDIAN is set, this implies the system is a big endian system and the
*	byte offset read should be swapped.
*
*******************************************************************************/
#define ReadRAM(offset) RAM[(offset)^(ENDIAN)]



/*******************************************************************************
* 8080a Inline Function:
* ---------------------
* 
*    Parity8 - This function calculates the parity from the supplied 8-bits of
*	data.
*  
* Input:
* -----
*
*    byte - This is an 8-bit unsigned integer.  The content of this variable
*	byte for which the parity will be found.
*
* Output:
* ------
*
*    (return value) - This is an 8-bit unsigned integer that contains 1 (true)
*	if the byte is parity and 0 (false) if the byte is non-parity.
*
*******************************************************************************/
#define Parity8(byte) \
	(!!((byte)&0x80)) ^ \
	(!!((byte)&0x40)) ^ \
	(!!((byte)&0x20)) ^ \
	(!!((byte)&0x10)) ^ \
	(!!((byte)&0x08)) ^ \
	(!!((byte)&0x04)) ^ \
	(!!((byte)&0x02)) ^ \
	(!!((byte)&0x01))



#endif
