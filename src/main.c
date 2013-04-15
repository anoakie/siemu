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


Preface:
-------

   This program is Intel 8080A simulator (or emulator in common slang). It was
written specifically for the arcade game Space Invaders (c) from Midway
Entertainment 1978 and it's many clones.  This simulator can run all 8080A
code.  It can also execute most 8080 code, but not all code due to undocumented
instructions.  Support for undocumented instructions is contained in the CPU
core and can be added into the simulator at compile time.

   This simulator should be mostly accurate with timing and inputs.  Complete
accuracy can not be achieved because of the inability of most operating systems
to have a timer granularity of less than 10ms (see graphics.c for more
information).

   This simulator relies heavily on the SDL library (http://www.libsdl.org).
Since most of the graphics, input, and audio processing is done through SDL, it
is possible to take the core of the program (8080a.c, 8080a.h, 8080avar.h, and
opcodes.h) and attach other functions to it, making it very modular.  The
ReadInputDevice, PlaySound, and UpdateScreen functions are the only functions
8080a.c calls. It is quite possible to use any graphics/input/audio library as
long as the user keeps the same naming scheme.

*******************************************************************************/


/*******************************************************************************
* Included Files:
* --------------
*
*   main.h - This file references header files which contain functions
*  	for initialization of the graphical and audio subsystems and the
*	start point for 8080a processor simulation.
*
*******************************************************************************/

#include "main.h"


/*******************************************************************************

Main:
----

  This routine is the starting point for the program.  It flows through three
phases:  initalization, execution, and cleanup.  During the initialization
phase, The global variables are created and initialized to their default
values. Initialization of the graphics and audio subsystems via the
InitGraphics and InitSound functions follow respectively. The execution phase
begins with the main8080a function.  The 8080a processor simulation occurs
during this phase and only returns when the user force exits the loop.
Upon completion of the execution phase, the cleanup phase begins by calling
the CleanUp function.  This function then shuts the graphics and audio
subsystems down and frees any allocated memory.

Inputs:
------

  argv - This pointer is an array of command line argument strings.

  argc - This integer is the number of character pointers argv contains.

*******************************************************************************/
int main (int argc, char *argv[])
{

	/***********************************************************************
	*   Display the program name and version number.
	*
	*
	* Note:
	* ----
	*
	*   VERSION_MAJOR and VERSION_MINOR are defined in the Makefile.
	***********************************************************************/
	printf ("=== 8080a Simulator version %s.%s ===\n\n",
		VERSION_MAJOR, VERSION_MINOR);


	/***********************************************************************
	* Initialization phase:
	* --------------------
	*
	*   This function sets the USE_SOUND global to TRUE.  In future
	* revisions, this function may set other global variables relating to
	* command line arguments.
	***********************************************************************/	
	SetGlobals(argc, argv);


	/***********************************************************************
	*   This function initializes the graphics subsystem.  While
	* initializing the graphics subsystem it also initializes the audio
	* subsystem and timer subsystem.  The keyboard's repeat rate is set
	* at the highest possible repetitions per second.  The graphics mode is
	* set to 224x256x16 bit for Linux and 256x256x16 bit for Windows.  The
	* global variable screen points to the newly created hardware surface.
	* If the initialization of any of the subsystems fails or a hardware
	* surface can not be created, the function displays an error on STDERR
	* and forces the program to gracefully exit, freeing any created
	* subsystems.
	***********************************************************************/	
	InitGraphics();


	/***********************************************************************
	*   This function initializes the audio subsystem for mixing and opens
	* the audio device.  The default audio settings are set to 16-bit
	* stereo audio at 22Khz.  Once the audio device has been opened,
	* playback is unpaused. If the audio device cannot be opened or the
	* target defaults cannot be set, the function displays an error on
	* STDERR and forces the program to gracefully exit, freeing any
	* previously created subsystems.
	***********************************************************************/	
	InitSound();


	/***********************************************************************
	*   If there are any arguments specified on the command line, it is
	* safe to assume that the first argument in the set of these arguments
	* is the ROM set the user intends to load. If a ROM set is not specified
	* on the command line, then the default set, named invaders, is loaded.
	* To verify the tautology of this assumption, usage information is
	* printed to STDOUT.  If the invaders set does not exist, a final
	* error message is printed to STDOUT, and the program terminates.
	*
	* Note:
	* ----
	* In the future, this code will be moved into the SetGlobals function
	* provided that there is support for more then one command line
	* argument.
	***********************************************************************/	
	if (argc > 1) 
	{

	/***********************************************************************
	*   If the user specifies an invalid or non-existant ROM set, a failure
	* notice is printed to STDOUT, and the default ROM set, named invaders,
	* is loaded into memory.  If the invaders set does not exist, a final
	* error message is printed to STDOUT and the program terminates.
	***********************************************************************/
		if (!LoadSet(argv[1]))
		{
			printf (
		"*** Failed loading ROM set!  Defaulting to \'invaders\'\n"
		"*** Make sure that the set is in a directory called\n"
		"*** \'roms\' (all lower case).\n*** \n"
		"*** The following sets are known to work:\n"
		"*** invaders\n*** invrvnge\n*** spaceatt\n\n"
			);


	/***********************************************************************
	*   If the invaders set does not exist, a final error message is
	* printed to STDOUT and the program terminates.
	***********************************************************************/
			if (!LoadSet("invaders"))
			{
				printf (
			"*** Default ROM set \'invaders\' failed to load.\n"
			"*** Exiting.\n\n"
				);
				exit(0);
			}
		}
	} else
	{
		printf (
	"*** No ROM set specified!  Defaulting to \'invaders\'\n"
	"*** Make sure that the set is in a directory called\n"
	"*** \'roms\' (all lower case).\n*** \n"
	"*** The following sets are known to work:\n"
	"*** invaders\n*** invrvnge\n*** spaceatt\n\n"
		);
	
	/***********************************************************************
	*   If the invaders set does not exist, a final error message is
	* printed to STDOUT and the program terminates.
	***********************************************************************/
		if (!LoadSet("invaders"))
		{
			printf (
		"*** Default ROM set \'invaders\' failed to load.\n"
		"*** Exiting.\n\n"
			);
			exit(0);
		}
	}


	/***********************************************************************
	* Execution phase:
	* ---------------
	*
	*   This function enters the main simulation loop.  This function will
	* only return if the user force quits the loop.
	***********************************************************************/
	main8080a();


	/***********************************************************************
	* Cleanup phase:
	* -------------
	*
	*   This function shuts the graphical and audio subsystems down and
	* frees the memory associated with the global variable screen.
	***********************************************************************/
	CleanUp();

	/***********************************************************************
	*   This program should not reach this function.  This function is
	* required for a correct ANSI C main() function.
	***********************************************************************/
	exit (0);
}
