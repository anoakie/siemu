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
the InitGraphics and UpdateScreen functions.  If this assumption becomes
untrue, then this file should contain a copy of the appropriate documentation
for the InitGraphics and UpdateScreen functions.


Contents:
--------
  1.	This file contains the function definitions for InitGraphics and
  	UpdateScreen.  This file also contains a pointer to the screen buffer.

*******************************************************************************/


#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

/*******************************************************************************
* Included Files:
* --------------
*
*   global.h - This file contains the definitions for TRUE and FALSE.  This
*	also contains the default screen resolutions and frames per second
*	settings.
*
*   SDL.h - This file contains the following function definitions needed
*	by graphics.c:  SDL_Init, SDL_EnableKeyRepeat, SDL_SetVideoMode, 
*	SDL_MUSTLOCK, SDL_LockSurface, SDL_MapRGB, SDL_UnlockSurface, and
*	SDL_UpdateRect.
*
*   stdlib.h - This file contains the following function definitions needed
*	by graphics.c:  atexit and exit.
*
*   string.h - This file contains the following function definition needed
*	by graphics.c:  memset.
*
*   stdio.h - This file contains the following function definitions needed
*	by graphics.c:  perror and printf.
*
*   cleanup.h - This file contains a procedure named CleanUp that frees the
*	surface named screen created by the InitGraphics procedure, closes
*	the sound subsystem and mixer created by the InitSound procedure, 
*	cleans up extra allocated memory, and exits gracefully
*
*******************************************************************************/

#include "global.h"
#include "SDL.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cleanup.h"



/*******************************************************************************
* Function:
* --------
*
*   InitGraphics - This function initializes the graphics subsystem.  While
* initializing the graphics subsystem it also initializes the audio
* subsystem and timer subsystem.  The keyboard's repeat rate is set
* to highest possible repititions per second.  The graphics mode is
* set to 224x256x16 bit for Linux and 256x256x16 bit for Windows.  The
* global variable screen points to the newly created hardware surface.
* If the initialization of any of the subsystems fails or a hardware
* surface can not be created, the function displays an error on STDERR
* and forces the program to gracefully exit, freeing any created
* subsystems.
*
*******************************************************************************/
void InitGraphics ();


/*******************************************************************************
* Function:
* --------
*
*   UpdateScreen - Updates the surface based on the contents of the 8080a's
*	video ram.  If FRAMESKIP is not equal to zero, then this function skips
*	FRAMESKIP frames for every frame drawn.  If GAMESPEED is not equal to
*	zero, then this function slows the speed at which the frames are drawn.
*
*******************************************************************************/
void UpdateScreen(unsigned char *VIDEORAM);


/*******************************************************************************
* Global Variables:
* ----------------
*
*   screen - The main windows' graphical surface structure.  Contains a pointer
*	to the screen buffer.
*
* Note:
* ----
*   To facilitate ease of use, I will refer to this the screen buffer from here
*	on out.
*
*******************************************************************************/
SDL_Surface *screen;

#endif
