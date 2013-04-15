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
the CleanUp function.  If this assumption becomes untrue, then this file should
contain a copy of the appropriate documentation for the CleanUp function.


Contents:
--------
  1.	This file references a header file which contains the global variable
  	named screen.

*******************************************************************************/



#ifndef __CLEANUP_H__
#define __CLEANUP_H__

/*******************************************************************************
* Included files:
* --------------
*
*   graphics.h - This file contains a pointer to the graphics surface, named
*	screen.
*
*   SDL.h - This file contains the following function definition needed
*	by graphics.c:  SDL_FreeSurface.
*
*******************************************************************************/
#include "graphics.h"
#include "SDL.h"


/*******************************************************************************
* Function:
* --------
*
*   CleanUp - This function frees the surface contained in the global variable
*	named screen.  This function also shuts down all initialized subsystems
*	that were created during the initialization phase.  To successfully
*	shut down the subsystems, this function relies on the fact that the
*	function SDL_Quit was registered to be called with the function atexit
*	when normal program termination occurs.
*******************************************************************************/
void CleanUp ();

#endif
