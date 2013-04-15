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

        Contact:  Anoakie Turner
                  Anoakie.Turner@asu.edu

                  13240 N. 94th Pl.
                  Scottsdale, AZ 85260


Contents:
--------
  1.	This file contains the 8080a's registers, the 8080a's interrupt
	information, the 8080a's default processor values (when reset),
	definitions for the PSW, memory map information, and the memory itself.

*******************************************************************************/


#ifndef __8080AVAR_H__
#define __8080AVAR_H__


#include "byteord.h"




/*******************************************************************************
* Big/Little Endian:
* -----------------
* 
*   This macro sets the LITTLEENDIAN or BIGENDIAN macros if they are not
* already defined.  If neither of the above macros are defined, this routine
* uses the predetermined detection scheme from the fine byteord.h to guess
* what system the target binary will be compiled for.  If autodetection is
* used, a warning message is displayed informing the user of what the target
* endian will be.
* 
*******************************************************************************/
#if !LITTLEENDIAN && !BIGENDIAN
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define LITTLEENDIAN 1
#warning Little endian auto-detected.
#else
#define BIGENDIAN 1
#warning Big endian auto-detected.
#endif
#warning Edit the Makefile if this is incorrect.
#endif



/*******************************************************************************
* Conflicting Endian:
* ------------------
* 
*   If an endian type is already defined in the Makefile and it does not match
* the endian type that is autodetected, a warning is printed to the screen
* informing the user that a possible error has occured in endian selection.
* 
*******************************************************************************/
#if (((SDL_BYTEORDER == SDL_LIL_ENDIAN) && BIGENDIAN) || \
     ((SDL_BYTEORDER == SDL_BIG_ENDIAN) && LITTLEENDIAN))
#warning CONFLICTING ENDIAN TYPES DETECTED!
#warning Edit the Makefile if this is incorrect.
#endif



/*******************************************************************************
* Variables:
* ---------
* 
*   INTDISABLE - This variable is used when the DI instruction is called.  It
*	signifies that interrupts are disabled.
*
*   INTERRUPT - This variable is used to determine if an interrupt occured,
*	and if an interrupt did occur, what type of interrupt occured.
*
*   NMI - This variable is used to determine if a non-maskable interrupt
*	occured.
*
*   COUNTER - This variable counts the number of cycles per instruction in an
*	attempt to get accurate interrupt timing.
*
*   RAM is the 8080a's RAM.  This is really RAM and ROM put together.  The
* 	simulator does not discriminate with read/write permissions, the whole
* 	RAM is writable.  Although this allows for self modifying code, access
* 	violations are not caught.  Rom size is 64k (0x10000 hex/65536 bytes).
*
*
* 16-bit registers:
* ----------------
*
*   PC is the Program Counter, which is a 16 bit register
*
*   SP is the Stack Pointer, which is a 16 bit register
*
*
* 8-bit registers:
* ---------------
*
*   A - This variable is the 8-bit accumulator.
*
*   PSW - This variable is the processor status word, which is also an 8-bit
*	register.
*
*   BC, DE, and HL are 16-bit paired registers.  Each register pair contains
*	2 8-bit registers.  BC contains B and C, DE contains D and E, and HL
*	contains H and L. Each pair is part of a union struct, where .pair
*	is the value of the pair and .reg. is the value of a single register.
*
*******************************************************************************/

/*  Interrupt information  */
static Uint8 INTDISABLE;
static Uint8 INTERRUPT;
static Uint8 NMI;
static Uint16 COUNTER;

/*  Registers  */
static Uint8 A, PSW;
static Uint16 SP, PC;


/*  Register Pairs  */
static union
{
	Uint16 pair;

	struct
	{
	#ifdef BIGENDIAN
                Uint8 B, C;
	#else
                Uint8 C, B;
	#endif
	} reg;
} BC;


static union
{
	Uint16 pair;

	struct
	{
	#ifdef BIGENDIAN
                Uint8 D, E;
	#else
                Uint8 E, D;
	#endif
	} reg;
} DE;


static union
{
	Uint16 pair;

	struct
	{
	#ifdef BIGENDIAN
                Uint8 H, L;
	#else
                Uint8 L, H;
	#endif
	} reg;
} HL;



/*******************************************************************************
* Unused temporary registers:
* --------------------------
*
*   These registers are used for the internal execution of instructions only.
*

static Uint8 W, Z;

*
*******************************************************************************/


/*  8080a's RAM  */
static Uint8 RAM[0x10000];


/*******************************************************************************
* Interrupt timings:
* -----------------
* 
* Processor	= 2048000 hz
*
* Refresh Rate	= 60 hz
* Interrupt	= 2048000 hz/60 hz ~= 34133/2 ~= 17066 cycles per interrupt
*
* Refresh Rate	= 50 hz
* Interrupt	= 2048000 hz/50 hz ~= 40960/2 ~= 20480 cycles per interrupt
* 
*
*    Although this number is inaccurate, it is still accurate enough to ensure
* proper timing and simulation.  After 33333 cycles (number of cycles may be
* higher, instruction and hertz rate depending), the program calls an
* interrupt.
* 
* 
* Resets:
* ------
* 
*   RST0 is a full restart.
*   RST1 is for vsync.  This is usually called during an non-maskable
*	interrupt.
*   RST2 is checking for input.  This opcode is called during a normal
*	interrupt.
* 
*******************************************************************************/

#ifndef PROCHZ
#define PROCHZ 2048000	// 2 Mhz
#endif

#ifndef REFRESH
#define REFRESH 60	// 60 hertz
#endif

#define INTHIGH	(PROCHZ/REFRESH)/2



/*******************************************************************************
* Default processor values:
* ------------------------
* 
*   The stack pointer starts at 0x2400.
*   The program counter starts at 0x0000.
*   The program status word value starts at 0x02
* 
*******************************************************************************/
/*  Defaults  */
#define SPSET	0x2400
#define PCSET	0x0000
#define PSWSET	0x02


/*******************************************************************************
* Processor status word:
* ---------------------
* 
*   The PSW is an 8 bit register that stores the 8080a's current instruction
* status.
* 
* 
* Map:
* ---
* 
* bit	7654 3210
* 	SZ_A P_1C
*
* S = Sign (negative), Z = Zero, A = Auxiliary carry,
* P = Parity, C = Carry, _ = Unknown, 1 = Always on.
* 
*******************************************************************************/
/*  PSW  */
#define SIGN	0x80
#define ZERO	0x40
#define AUX	0x10
	// Even parity
#define PARITY	0x04
#define CARRY	0x01
#define NSIGN	(0xFF - SIGN)
#define NZERO	(0xFF - ZERO)
#define NAUX	(0xFF - AUX)
	// Odd parity
#define NPARITY	(0xFF - PARITY)
#define NCARRY	(0xFF - CARRY)



/*******************************************************************************
* Memory Map:
* ----------
* 
*   This simplifies ROM set loading and 8080a memory access.
* 
* 
* Assumptions:
* -----------
*   Roms are split into four 800 hex (2048 bytes) files and are loaded
* sequentially.  Space Invaders, which follows the above assumption, is the
* main ROM set that is loaded.
* 
* 
* Definitions:
* -----------
* 
*   STARTROM1 - This is the memory address where the first ROM should be
*	loaded.
*
*   STARTROM2 - This is the memory address where the second ROM should be
* 	loaded.
*
*   STARTROM3 - This is the memory address where the third ROM should be
*	loaded.
*
*   STARTROM4 - This is the memory address where the fourth ROM should be
*	loaded.
*
*   ROMSIZE - This is the size of each ROM.  800hex (2048bytes).
*
*   PROGROM - This is where the processor's PROGROM ROM area begins.  Note that
*	memory is not protected by this simulator.
*
*   WORKRAM - This is where the processor's WORK RAM begins.  Note that the
*	work ram contains the stack starting at 0x2400.  The stack grows
*	downwards.  This simulator does not check bounds on the stack, so stack
*	overflows can occur without an error being generated.
*
*   VIDEORAM - This is where the processor's VIDEO RAM begins.
*
*   UNUSED - This is where the processor's UNUSED memory area begins.  This
*	definition is only true for Space Invaders.
* 
*
* Diagram:
* -------
*
*  The 8080a's	
*  memory map	 Address ranges
* +-----------+
* |           |
* |  PROGROM  | 0x0000h-0x1FFFh
* +-----------+
* |  WORKRAM  | 0x2000h-0x23FFh
* +-----------+
* |           |
* | VIDEORAM  | 0x2400h-0x3FFFh
* +-----------+
* |           |
* |           |
* |           |
* |  UNUSED   | 0x4000h-0xFFFFh
* +-----------+
* 
*
* Note:
* ----
* 
*    The above information may be Space Invaders specific.
*
*******************************************************************************/
/*  Memory Map  */
#define STARTROM1 0x0000
#define STARTROM2 0x0800
#define STARTROM3 0x1000
#define STARTROM4 0x1800
#define ROMSIZE   0x0800
#define PROGROM   0x0000
#define WORKRAM   0x2000
#define VIDEORAM  0x2400
#define UNUSED    0x4000


#endif
