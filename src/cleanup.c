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


Preface:
-------

   The main function in this file is CleanUp.  This function frees the main
graphical surface, and exits the program.  In freeing the surface, the main
window and input queue are destroyed.  While exiting, the SDL_Quit function
is called.  This function shuts down all of the subsystems and frees all of
the resources allocated to them.

Note:
----

   In the event that the program becomes more complex, I decide to implement
threading, or terminating the program becomes unstable, I will remove the
atexit in the InitGraphics function and call SDL_Quit directly from this
function.

*******************************************************************************/


/*******************************************************************************
* Included Files:
* --------------
*
*   cleanup.h - This file references a header file which contains the global
*	variable named screen.
*
*******************************************************************************/

#include "cleanup.h"


/*******************************************************************************

CleanUp:
-------

   This function frees the graphics buffer, named screen. This function also
shuts down the various subsystems created during the initialization phase,
namely the graphical and audio subsystems. For proper functionality, atexit
must be called with the proper subsystem destruction functions.  If running
subsystems exist, then the previous assumption must be true.

*******************************************************************************/
void CleanUp ()
{
	/***********************************************************************
	*   This function frees the memory that was allocated to the variable
	* named screen, destroys the window, and disables the keyboard input.
	***********************************************************************/
	SDL_FreeSurface(screen);


	/***********************************************************************
	*   This function indirectly shuts down the various subsystems by the
	* use of the atexit function.  The audio and graphics subsystems are
	* destroyed, then the program exits.  The previous order of operations
	* is not guaranteed.  It is largely dependent on the nature of atexit
	* and the operating system that this program runs on.
	***********************************************************************/
	exit (1);
}
