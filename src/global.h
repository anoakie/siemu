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



Note:
----
  This file assumes that the user can access the source file for details about
the SetGlobals function.  If this assumption becomes untrue, then this file
should contain a copy of the appropriate documentation for the SetGlobals
function.


Contents:
--------
  1.	This file contains the definitions for TRUE and FALSE.  It also
  	contains the global variables USE_SOUND, which lets you enable and
	disable the sound system, and FATAL_ERROR, which logs what type of
	fatal error just occurred.

*******************************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__



/*******************************************************************************
* Included files:
* --------------
*
*   SDL_endian.h - This header file defines the type Uint8.
*
*******************************************************************************/
#include "SDL_endian.h"



/*******************************************************************************
* Global Definitions:
* ------------------
*
*   TRUE - Defined as true, which is an integer that is not zero.
*   
*   FALSE - Defined as false, which is an integer that is zero.
*   
*   FRAMESKIP - Defined as 1 by default.  Indicates the number of frames to
*	not draw for every frame that is drawn.  This number should be
*	increased if the user is on a slow machine.  A frame skip of 0 (when
*	FPS is set to 60) is 60 frames per second (fps), 1 is 30 fps, 2 is 15
*	fps, and so forth if.
*	
*   GAMESPEED - Defined as 0 by default.  Indicates the number of frames to
*	delay before drawing the current frame.  A speed of 0 is full speed,
*	1 is 1/2 speed, 2 is 1/3 speed, and so forth.
*
*   FPS - Defined as 60 by default.  Indicated the number of frames to draw
*	per second.
*******************************************************************************/
#define TRUE  1
#define FALSE 0

#ifndef FRAMESKIP
#define FRAMESKIP 1
#endif

#ifndef GAMESPEED
#define GAMESPEED 0
#endif

#ifndef FPS
#define FPS 60
#endif

#define SCREEN_XMAX 224
#define SCREEN_YMAX 256
#define SCREEN_MAX 256
#define BIT_DEPTH 16


/*******************************************************************************
* Global Variables:
* ----------------
*
*   USE_SOUND - An integer indicating whether the sound system should be used
*	(TRUE) or not (FALSE).
*	
*   FATAL_ERROR - An integer that stores the current fatal error.  This
*   	variable is currenlty unused.
*
*   USE_COLOR - An integer that is used to determine if color simulation is
*	is used.  This variable is set to 0 by default.
*******************************************************************************/
Uint8 USE_SOUND;
Uint8 FATAL_ERROR;
Uint8 USE_COLOR;


/*******************************************************************************
* Function:
* --------
*
*   SetGlobals - This function sets the program's global variables based on the
*   	command line arguments.  If a particular argument is not specified,
*   	then the global variable relating to that argument is set to its
*   	default value.
*******************************************************************************/
void SetGlobals (int argc, char *argv[]);


/*******************************************************************************
* Global Endian:
* -------------
* 
*   This macro sets the ENDIAN value.  This value is used in the WriteRAM and
* ReadRAM macros to ensure proper byte locations on big-endian systems.
* 
*******************************************************************************/
#ifdef BIGENDIAN
#define ENDIAN 1
#else
#define ENDIAN 0
#endif



#endif

