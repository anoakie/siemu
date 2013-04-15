/*******************************************************************************

8080A Simulator
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
the main function.  If this assumption becomes untrue, then this file should
contain a copy of the appropriate documentation for the main function.


Contents:
--------
  1.	This file references header files which contain functions for
  	initialization of the graphical and audio subsystems as well as the
	start point for 8080a processor simulation.
*******************************************************************************/


#ifndef __MAIN_H__
#define __MAIN_H__

/*******************************************************************************
* Included Files:
* --------------
*
*   global.h - This file contains a procedure named SetGlobals that initializes
*	global variables.
*
*   graphics.h - This file contains a pointer to the graphics surface named
*	screen.	 This file also contains a procedure named InitGraphics which
*	is used to initialize the graphics subsystem with the surface named
*	screen.
*
*   sound.h - This file contains a procedure named InitSound to initialize
*	the sound subsystem and sound mixer.
*
*   8080a.h - This file contains procedures named LoadSet and main8080a.
*	The LoadSet procedure is used to load a ROM set from disk into memory.
*	The main8080a procedure is used to begin the simulation of the 8080a
*	processor.
*
*   cleanup.h - This file contains a procedure named CleanUp that frees the
*	surface named screen created by the InitGraphics procedure, closes
*	the sound subsystem and mixer created by the InitSound procedure, 
*	cleans up extra allocated memory, and exits gracefully
*
*   stdio.h - This file contains the following function definition needed
*	by graphics.c:  printf.
*
*******************************************************************************/

#include "global.h"
#include "graphics.h"
#include "sound.h"
#include "8080a.h"
#include "cleanup.h"
#include <stdio.h>

#endif
