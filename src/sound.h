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
the main function.  If this assumption becomes untrue, then this file should
contain a copy of the appropriate documentation for the main function.


Contents:
--------
  1.	This file contains the definition for the function ReadInputDevice.

*******************************************************************************/


#ifndef __SOUND_H__
#define __SOUND_H__


/*******************************************************************************
* Included Files:
* --------------
*
*   SDL.h - This file contains the following function definitions needed
*	by sound.c:  SDL_OpenAudio, SDL_GetError, SDL_PauseAudio, SDL_MixAudio,
*	SDL_LoadWAV, SDL_BuildAudioCVT, SDL_ConvertAudio, SDL_FreeWAV,
*	SDL_LockAudio, and SDL_UnlockAudio.
*
*   stdlib.h - This file contains the following function definitions needed by
*	sound.c:  free and malloc.
*
*   cleanup.h - This file contains a procedure named CleanUp that frees the
*	surface named screen created by the InitGraphics procedure, closes
*	the sound subsystem and mixer created by the InitSound procedure, 
*	cleans up extra allocated memory, and exits gracefully
*
*******************************************************************************/

#include "SDL.h"
#include <stdlib.h>
#include "cleanup.h"



/*******************************************************************************
* Function:
* --------
*
*   InitSound - This function initializes the audio subsystem for mixing and
*	opens the audio device.  The default audio settings are set to 8-bit
*	mono audio at 11Khz.  Once the audio device has been opened, playback
*	is unpaused. If the audio device cannot be opened, or the target
*	defaults cannot be set, the function displays an error on STDERR and
*	forces the program to gracefully exit, freeing any previously created
*	subsystems.
*******************************************************************************/
void InitSound ();


/*******************************************************************************
* Function:
* --------
*
*   QueueSound - This function loads a sound into the queue and begins playing
*	it.  If the sound fails to load, an error message is printed to STDERR,
*	and this function gracefully exits.
*******************************************************************************/
void QueueSound (char *, Uint8);


/*******************************************************************************
* Function:
* --------
*
*   PlaySound - This function searches for an empty sound slot.  If a slot is
*	not found, this funtion returns.  If a slot is found, this function
*	then determines what sound to play by the values of OUT and type.  If
*	an appropriate sound is found, it is queued with the QueueSound
*	function.
*******************************************************************************/
void PlaySound (Uint8, Uint8);

#endif
