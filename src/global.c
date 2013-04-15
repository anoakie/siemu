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

   The main function in this file is SetGlobals.  This function reads the
command line arguments and sets the gobal variables accordingly.

Note:
----

   SetGlobal is currently not used for setting the global variables based
on the command line arguments because this program lacks command line
arguments. See main.c for information about loading the ROM set from the
command line.

*******************************************************************************/



/*******************************************************************************
* Included Files:
* --------------
*
*   global.h - This file contains a procedure named SetGlobals that initializes
*	the global variables.  It also contains the global variable used by
*	SetGlobals: USE_SOUND.
*
*******************************************************************************/

#include "global.h"



/*******************************************************************************

SetGlobals:
----------

   This function sets the various global variables based on the command line
arguments.  If a particular argument is not specified, then the global variable
relating to that argument is set to its default value.


Inputs:
------

  argv - This pointer is an array of character pointers to the command line
	arguments.

  argc - This integer is the number of character pointers the above variable, 
  	argv, contains.


Outputs:
-------
  USE_SOUND - This global variable is set to TRUE by default.  However, it is
  	not available for modification through the command line arguments at
	run time.

Note:
----

   At this time, this function does not read the command line arguments because
of the lack of global options currently in this program.  By default, it sets
the USE_SOUND global value to TRUE and the USE_COLOR variable to 0 (white only).

*******************************************************************************/
void SetGlobals (int argc, char *argv[])
{
	USE_SOUND = TRUE;


	/***********************************************************************
	*   This assignment sets the USE_COLOR variable to 0 (white only).
	***********************************************************************/
	USE_COLOR = 0;


	/***********************************************************************
	*   If the GREEN macro is set, this assignment sets the USE_COLOR
	* variable to 0 (green only).
	***********************************************************************/
#ifdef GREEN
	USE_COLOR = 1;
#endif


	/***********************************************************************
	*   If the COLOR macro is set, this assignment sets the USE_COLOR
	* variable to 2 (red/white/green).
	***********************************************************************/
#ifdef COLOR
	USE_COLOR = 2;
#endif

}

