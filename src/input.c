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

   The main function in this file is called ReadInputDevice.  This function
retrieves events from the event queue and creates an array containing keys that
have been pressed.  From this array, the return value can be set depending on
the state of certain keys.  Finally, the program loops through the queued
events looking for quit events and calls CleanUp if any are found.

*******************************************************************************/


/*******************************************************************************
* Included Files:
* --------------
*
*   input.h - This file contains the function reference for ReadInputDevice. 
*	This file also references header files that help this file read
*	input devices.
*
*******************************************************************************/

#include "input.h"


/*******************************************************************************

ReadInputDevice:
---------------

  This function handles the IN instruction for main8080a.


Input:
-----

  IN - This 8-bit unsigned integer identifies the input device that is being
	read. If the value is 1, player 1's keys are read.  If the value is
	2, player 2's keys are read.


Output:
------

  (return value) - This function returns an unsigned character that identifies
	what input switches have been switched.

*******************************************************************************/

Uint8 ReadInputDevice (Uint8 IN)
{

	/***********************************************************************
	* Declarations:
	* ------------
	*
	*   HSHIPS = Sets the number of ships a player begins the game with.
	*	If the value is 1, 1 ship is added to the players total number
	*	of ships.
	*
	*   LSHIPS = Sets the number of ships a player begins the game with.
	*	If the value is 1, 2 ships are added to the players total
	*	number of ships
	*
	*   FREESHIP = Sets the number of points the player needs to earn a
	*	free ship.  If the value is 0, 1500 points are needed.  If the
	*	value is 1, 1000 points are needed.
	***********************************************************************/
	static Uint8 HSHIPS = 0;
	static Uint8 LSHIPS = 0;
	static Uint8 FREESHIP = 0;


	/***********************************************************************
	*   This is the general event structure for SDL input.
	***********************************************************************/
	SDL_Event event;


	/***********************************************************************
	*   This variable is the return value.
	***********************************************************************/
	Uint8 retval;


	/***********************************************************************
	*   This variable is a pointer that will point to the key states.
	***********************************************************************/
	static Uint8 *keys;


	/***********************************************************************
	*   This function pumps the event loop to gather all of the events from
	* the input devices.  This includes all key presses that occured after
	* the function was previously called.
	***********************************************************************/
	SDL_PumpEvents();


	/***********************************************************************
	*   This function gets a snapshot of the current keyboard state.  This
	* is current as of the last SDL_PumpEvents.
	***********************************************************************/
	keys = SDL_GetKeyState(NULL);


	/***********************************************************************
	*   By default, the return value should be set to 0x00 and will be
	* modified by the function. The value 0x00 implies there is currently
	* no input data.
	***********************************************************************/
	retval = 0x00;


	/***********************************************************************
	*   The processor is asking for the first set of inputs.
	***********************************************************************/
	if (IN == 0x01)
	{

	/***********************************************************************
	*   The first flags that must be set are the NOERR and NOTILT flags.
	* This tells the 8080a that there are currently no errors, and the
	* machine itself is not being tilted.  There is no reason not to use
	* these flags, unless the user is interested in seeing a TILT screen.
	*
	* Note:
	* ----
	*
	*   When IN == 2, it must also return a TILT (IN2_TILT) for the TILT
	* screen to be seen.
	***********************************************************************/
		retval = IN1_NOERR | IN1_NOTILT;


	/***********************************************************************
	*   This flag should be on if there is not a coin being inserted.  If
	* the user is interested in inserting a coin, he/she can press the '3'
	* key on the keyboard to simulate inserting a coin (disables the flag).
	***********************************************************************/
		retval = (!keys[SDLK_3])*IN1_NOCOIN | retval;


	/***********************************************************************
	*   These flags control the 1 and 2 player start buttons.  If the user
	* presses '1' or '2' on the keyboard, he/she can attempt to start
	* playing a game, assuming all in-game conditions are met when the
	* he/she presses the key.
	***********************************************************************/
		retval = (keys[SDLK_2]*IN1_2PSTART) |
			 (keys[SDLK_1]*IN1_1PSTART) |
			 retval;
	} else
	{

	/***********************************************************************
	*   These flags control the TILT, HSHIPS, LSHIPS, and FREESHIPS
	* switches.  If the TILT switch is flipped, the game goes into TILT
	* mode.  Once in this mode, the machine must be reset for anything else
	* to happen.  If the HSHIPS variable is set, 1 free ship is added
	* to the players total number of ships.  If the LSHIPS variable is set,
	* 2 free ships are added to the players total number of ships.  If the
	* FREESHIP variable is set, the player gets a free ship at 1000 points
	* instead of 1500.
	***********************************************************************/
		retval = (keys[SDLK_PAGEUP]*IN2_TILT) |
			 (HSHIPS*IN2_HSHIPS) |
			 (LSHIPS*IN2_LSHIPS) |
			 (FREESHIP*IN2_FREESHIP);
	}


	/***********************************************************************
	*   These flags control the 1 and 2 player shoot, left, and right
	* buttons.  If the user presses the space bar, the left key, or the
	* right key on the keyboard, he/she can attempt to shoot, move his/her
	* player left, or move his/her player right, assuming all in-game
	* conditions are met when the user presses the key.
	***********************************************************************/
	retval = (keys[SDLK_SPACE]*IN_SHOT)  |
		 (keys[SDLK_LEFT]*IN_LEFT)   |
		 (keys[SDLK_RIGHT]*IN_RIGHT) |
		 retval;


	/***********************************************************************
	*   This while loop loops through all of the previously generated SDL
	* events (all of the events gathered with the SDL_PumpEvents function
	* earlier).  The main function of this loop is to determine if the user
	* wants to exit the program.  These events can be generated by either
	* the user hitting the escape key, or the user closing the window.
	***********************************************************************/
	while (SDL_PollEvent (&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:

	/***********************************************************************
	*   If the user hits the r key, the program prints "Processor reset
	* called." to STDOUT and resets the processor by calling ResetProc.
	***********************************************************************/
				if (keys[SDLK_r]) {
					printf ("[%d%d%d] Processor reset "
						"called.\n", HSHIPS, LSHIPS,
						FREESHIP);
					ResetProc();
				}


	/***********************************************************************
	*   If the user hits the left bracket key, the program toggles
	* the HSHIPS variable.  Debug information is printed to STDOUT.
	***********************************************************************/
				if (keys[SDLK_LEFTBRACKET])
				{
					HSHIPS = HSHIPS ? 0 : 1;
					printf ("Number of ships set to %d.\n",
						3+HSHIPS+LSHIPS*2);
				}


	/***********************************************************************
	*   If the user hits the left bracket key, the program toggles
	* the LSHIPS variable.  Debug information is printed to STDOUT.
	***********************************************************************/
				if (keys[SDLK_RIGHTBRACKET])
				{
					LSHIPS = LSHIPS ? 0 : 1;
					printf ("Number of ships set to %d.\n",
						3+HSHIPS+LSHIPS*2);
				}


	/***********************************************************************
	*   If the user hits the back slash key, the program toggles
	* the FREESHIP variable.  Debug information is printed to STDOUT.
	***********************************************************************/
				if (keys[SDLK_BACKSLASH])
				{
					FREESHIP = FREESHIP ? 0 : 1;
					printf ("Free ship at %d points.\n",
						1500 - FREESHIP*500);
				}


	/***********************************************************************
	*   If the user hits the c key, the program toggles the USE_COLOR
	* global variable.  Debug information is printed to STDOUT.
	***********************************************************************/
				if (keys[SDLK_c])
				{

	
	/***********************************************************************
	*   If the GREEN macro is defined and the COLOR macro is not defined,
	* we toggle color between 0 (white only) and 1 (green only).  Debug
	* information is printed to STDOUT.
	***********************************************************************/
				#ifdef GREEN
				#ifndef COLOR
					USE_COLOR = USE_COLOR ? 0 : 1;
				#endif	// for #ifndef COLOR
				#endif	// for #ifdef GREEN


	/***********************************************************************
	*   If the COLOR macro is defined and the GREEN macro is not defined,
	* we toggle color between 1 (white only) and 2 (red/white/green), and
	* 3 (red/white/green with color overlays).  If the GREEN macro is
	* defined while the COLOR macro is defined, we toggle the color between
	* 0 (white only), 1 (green only), 2 (red/white/green), and 3
	* (red/white/green with color overlays).
	***********************************************************************/
				#ifdef COLOR
					USE_COLOR = USE_COLOR == 3 ?
				#ifndef GREEN
						1 : USE_COLOR + 1;
				#else
						0 : USE_COLOR + 1;
				#endif	// for #ifndef GREEN
				#endif	// for #ifdef COLOR

					printf ("Color simulation [%d] toggled."
						"\n", USE_COLOR);
				}


	/***********************************************************************
	*   If the user hits the f key, the program toggles the full screen
	* setting.  Debug information is printed to STDOUT.
	***********************************************************************/
				if (keys[SDLK_f])
				{
					if (!SDL_WM_ToggleFullScreen(screen))
						printf ("FullScreen toggle"
							"FAILED!\n");
				}


	/***********************************************************************
	*   If the user hits the escape key, the program enters the CleanUp
	* function.
	***********************************************************************/
				if (keys[SDLK_ESCAPE])
				{			

	/***********************************************************************
	*   If the user closes the window, the program enters the CleanUp
	* function.
	***********************************************************************/
			case SDL_QUIT:
					CleanUp();
					break;
				}
				break;


			default:
				break;
		}
	}


	/***********************************************************************
	*   This function returns the flags we have set to the main simulation
	* loop.
	***********************************************************************/
	return (retval);
}
