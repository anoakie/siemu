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
the ReadInputDevice function.  If this assumption becomes untrue, then this file
should contain a copy of the appropriate documentation for the ReadInputDevice
function.


Contents:
--------
  1.	This file contains the definition for the function ReadInputDevice.

*******************************************************************************/


#ifndef __INPUT_H__
#define __INPUT_H__


/*******************************************************************************
* Included Files:
* --------------
*
*   cleanup.h - This file contains the following function definition needed by
*	input.c:  CleanUp.
*
*   8080a.h - This file contains the following function definition needed by
*	input.c:  ResetProc.
*
*   SDL.h - This file contains the following function definitions needed
*	by input.c:  SDL_Event, SDL_PumpEvents, SDL_GetKeyState, and
*	SDL_PollEvent.
*
*******************************************************************************/

#include "cleanup.h"
#include "8080a.h"
#include "SDL.h"



/*******************************************************************************
* Input Flags:
* -----------
*
*   IN1_NOCOIN - Detects if a coin has not been inserted.  If this flag is
*	not set, then a coin has been inserted.
*   IN1_2PSTART - Used for starting a 2 player game.
*   IN1_1PSTART - Used for starting a 1 player game.
*   IN1_NOTILT - Detects if the machine has been tilted.  If this flag is
*	not set, then the machine tilted.
*   IN1_NOERR - Detects if the machine currently has an error.  If this flag
*	is not set, then the machine has an error.
*   IN_SHOT - Detects if the player has pressed the shoot button.
*   IN_LEFT - Detects if the player has pressed the left button.
*   IN_RIGHT - Detects if the player has pressed the right button.
*	
*******************************************************************************/

#define IN1_NOCOIN	0x01
#define IN1_2PSTART	0x02
#define IN1_1PSTART	0x04
#define IN1_NOTILT	0x08
#define IN1_NOERR	0x80
#define IN_SHOT		0x10
#define IN_LEFT		0x20
#define IN_RIGHT	0x40


/*******************************************************************************
* Dip switches:
* ------------
*
*   For IN2_?SHIPS, the default number of ships a player starts with is 3.
*   IN2_HSHIPS - If this switch is flipped, 2 is added to the number of ships
*	the player starts with.
*   IN2_LSHIPS - If this switch is flipped, 1 is added to the number of ships
*	the player starts with.
*   IN2_TILT - If this switch is flipped, the machine has been tilted.
*   IN2_FREESHIP - If this switch is flipped, the player gets a free life at
*	1000 points, else he/she gets a life at 1500 points..
*	
*******************************************************************************/
#define IN2_HSHIPS	0x01
#define IN2_LSHIPS	0x02
#define IN2_TILT	0x04
#define IN2_FREESHIP	0x08



/*******************************************************************************
* Function:
* --------
*
*   ReadInputDevice - This function reads the current keyboard values and
*	returns a single byte of data that is supposed to represent the input
*	values for the 8080a's IN instruction.
*
*******************************************************************************/

Uint8 ReadInputDevice (Uint8);

#endif
