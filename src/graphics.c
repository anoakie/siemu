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

   The main functions in this file are InitGraphics and UpdateScreen. 
InitGraphics initializes the SDL library, then creates a 224x256x16 bit surface
(or 256x256x16 bit if the user is running Windows).  InitGraphics also sets the
key repeat rate.  UpdateScreen draws images to the window based on the contents
of the 8080a's video ram.  UpdateScreen also controls the speed of the simulator
and the number of frames that are displayed per second.

*******************************************************************************/


/*******************************************************************************
* Included Files:
* --------------
*
*   graphics.h - This file contains a pointer to the graphics surface, named
*	screen.  This header file also contains the function references
*	InitGraphics and UpdateScreen.
*
*******************************************************************************/

#include "graphics.h"



/*******************************************************************************

InitGraphics:
------------

  This function initializes the graphics, audio, and timer subsystems.  The
keyboard's repeat rate is set to the highest number of repetitions per second.
A window is created with the following resolutions:  224x256x16 bit for Linux
and 256x256x16 bit for Windows.  If the window was successfully created, the
global variable, named screen, points to the newly created hardware surface,
and a confirmation is printed to STDOUT. If the initialization of the
subsystems fails or a hardware surface cannot be created, the function displays
an error on STDERR and forces the program to gracefully exit, freeing any
created subsystems.

*******************************************************************************/

void InitGraphics ()
{

	/***********************************************************************
	*   This function initializes SDL and the following subsystems:
	* audio, video, and timer, respectively.  If initialization of any
	* of the subsystems fails, then an error is displayed on STDERR and
	* the program gracefully exits.
	***********************************************************************/
	if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 )
	{
		perror("Unable to init SDL");
		exit(0);
	}


	/***********************************************************************
	*   This function registers to call the SDL_Quit function when the
	* program is terminated.  If a call to the exit function occurs after
	* this line of code, all of the SDL subsystems are shut down and
	* their resources are freed.  This allows the program gracefully exit
	* without a segmentation fault.
	***********************************************************************/
	atexit(SDL_Quit);


	/***********************************************************************
	*   This function determines how long a key must be pressed before the
	* key stroke begins to repeat.  The actual delay is set to zero, which
	* disables key repeating and sets the repeat rate to the default rate. 
	* This function generates an event every time a key is despressed while
	* checking the event queue.
	***********************************************************************/
	SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);


	/***********************************************************************
	*   This function creates a window with a certain height, width,
	* and bits-per-pixel mode, respectively.  In Windows, if WIN32 is
	* defined, the program creates a software surface that is 256 pixels
	* high and 256 pixel wide with a 16-bit depth.  The surface must be at
	* this resolution, even though Space Invaders uses a 224x256
	* resolution, because Windows cannot create directx surfaces below
	* 256x256.  The default resolution is a 224x256x16 bit surface.  On a
	* Unix-based operating systems or on systems that run XWindows, this is
	* a software surface because X11 can not create a hardware surface.  If
	* the user employs DGA2 or SVGALib, then the program will create a
	* hardware surface.  The 16-bit depth buffer creates a speed to
	* simpleness tradeoff.  Using an 8-bit depth buffer puts the program
	* into a palletized mode. Although there are many speed increases with
	* palletized mode, the complexity of plotting points to a surface
	* increases because of pallet allocation.  Bit depths higher than
	* 16-bits have very detrimental speed decreases without a hardware
	* surface because of the increased number of bits-per-pixel.  The
	* program does not use hardware surface blitting because it is too
	* expensive to perform per-pixel blits on a hardware surface.  The
	* SDL_FULLSCREEN identifier is currently commented out so the program
	* can be run in a window.  Uncommenting the line below will force the
	* program to run in a fullscreen mode.  When using SVGALib or DGA2, the
	* program defaults to fullscreen, despite the SDL_FULLSCREEN tag.
	***********************************************************************/
        screen = 

	#ifdef WIN32
	/***********************************************************************
	*   This block is defined as WIN32.  The SDL graphics buffer, named
	* screen, defaults to a 256x256x16 software surface.
	***********************************************************************/
		SDL_SetVideoMode(SCREEN_MAX, SCREEN_MAX, BIT_DEPTH,

	#else
	/***********************************************************************
	*   This block is defined as non-WIN32.  The SDL graphics buffer, named
	* screen, defaults to a 224x256x16 software surface, unless DGA or
	* SVGALib are the external video drivers.
	***********************************************************************/
		SDL_SetVideoMode(SCREEN_XMAX, SCREEN_YMAX, BIT_DEPTH,
	#endif
		/*  Uncomment the following line to enable fullscreen access */
				//SDL_FULLSCREEN |
				SDL_SWSURFACE
				);

	/***********************************************************************
	*   If creation of the surface named screen fails, the program prints a
	* message to STDERR and exits gracefully.
	***********************************************************************/
	if (screen == NULL)
	{
		perror("Could not create window!");
                CleanUp(0);
	}


	/***********************************************************************
	*   The final video mode is displayed to STDOUT.  This mode was
	* obtained from SDL_SetVideoMode.  This result may be different from
	* the program's original request.
	***********************************************************************/
	printf("> Video display set to %dx%d at %d bits-per-pixel"
		" mode\n", screen->w, screen->h, screen->format->BitsPerPixel);


	/***********************************************************************
	*   These functions set the window caption and icon, respectively.
	***********************************************************************/
	SDL_WM_SetCaption("Space Invaders Emulator", "SI Emu");
	SDL_WM_SetIcon(SDL_LoadBMP("ICON"), NULL);


	/***********************************************************************
	*   This function hides the mouse cursor when the user hovers it over
	* the window.
	***********************************************************************/
	SDL_ShowCursor(SDL_DISABLE);
}





/*******************************************************************************

UpdateScreen:
------------

   This function updates the surface, named screen, based on the contents of
the 8080a's video ram (VIDEORAM).  This function also attempts to draw the
correct number of frames-per-second based on Space Invader's screen refresh
rate, which is 60 frames-per-second.  If the global definition FRAMESKIP is
higher than zero, the program skips FRAMESKIP frames for every frame drawn to
the screen. If a frame is skipped, the program still attempts to keep accurate
timing so that the simulator continues to run at a normal speed.  If GAMESPEED
is set, then the time this function waits to update the screen is delayed.


Input:
-----

   VIDEORAM - A pointer to the 8080a's video memory.  The size of the video
memory is 0x1C00 bytes.  Note that 224*256 resolution = 0x1C00 bytes = 57344
bytes.  Space Invaders uses 1 bit per pixel, or 8 pixels per byte. (57344
bytes)/(8 pixels per byte) = 7168 bytes.


Area in memory:
--------------
  
   8080a's VIDEORAM section
   |...........|
   +-----------+
   |           |
   | VIDEORAM  | 0x2400h-0x3FFFh
   +-----------+
   |...........|


Note:
----

   This function is called directly by main8080a.

*******************************************************************************/

void UpdateScreen(unsigned char *VIDEORAM)
{

	/***********************************************************************
	* Declarations:
	* ------------
	*
	*   lastframe - This stores the time in which the last frame is drawn.
	*	This value is determines if it is time to display a frame.  It
	*	is initialized to 0, so the first frame is drawn immediately.
	*
	*   curframe - This stores the time the current frame is drawn. This
	*	value determines if it is time to display a frame.
	*
	*   frames - This is a counter of the number of frames to skip per
	*	frame drawn. This variable is only useful if FRAMESKIP is
	*	greater	than 0. It is initialized to FRAMESKIP, which is the
	*	default number of frames to skip for every frame that is drawn.
	*
	*   i, j - These are temporary counting variables for loops.
	*
	*   pixel - This stores the value of a pixel that will be painted to
	*	the screen.
	*
	*   bits - This points to the program's frame buffer.
	***********************************************************************/
	static Uint32 lastframe = 0;
	static Uint8 frames = FRAMESKIP;
	Uint32 pixel;
	Uint32 curframe;
	Uint16 i;
	Uint16 *bits;
	Uint8 j;



	/***********************************************************************
	* Pixel Color Declarations:
	* ------------------------
	*
	*   pixelr - This stores the value of the red pixel for the color
	*	overlay.
	*
	*   pixelw - This stores the value of the white pixel for the color
	*	overlay.
	*
	*   pixelg - This stores the value of the green pixel for the color
	*	overlay.
	*
	*
	* Note:
	* ----
	*
	*   The color macro must be defined.
	***********************************************************************/
#ifdef COLOR
	const Uint32 pixelr = SDL_MapRGB(screen->format, 0xFF, 0x40, 0x00);
	const Uint32 pixelw = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	const Uint32 pixelg = SDL_MapRGB(screen->format, 0x40, 0xFF, 0x00);
#endif



#ifndef NODELAY
	/***********************************************************************
	*   If the last frame was not displayed, decrease the number of frames
	* displayed.
	***********************************************************************/
	if (frames) frames --;


	/***********************************************************************
	*   This do loop attempts to time the screen updates accurately, based
	* on the refresh rate of Space Invaders.  This, however, is impossible
	* because of the granularity of SDL_Delay.  Kernel scheduling itself is
	* inaccurate. It is almost impossible to get a granularity of less then
	* 10ms without a busy-wait loop and no delay. To decrease the amount of
	* delay time in Linux, the user should edit include/ams-i386/param.h
	* and change the HZ value.  If the TIMING tag is defined, this program
	* will use as much CPU time as it can to ensure accurate timing.
	***********************************************************************/
	do
	{

	/***********************************************************************
	*   This retrieves the current time and compares it to the time the
	* last frame was displayed.  If it is time to display another frame,
	* lastframe is set equal to curframe.
	*   SDL_GetTicks returns the number of milliseconds since the SDL
	* Library was initialized.  If the difference between the current frame
	* and the last frame is greater than or equal to 999 milliseconds
	* divided by 60 frames divided by the GAMESPEED plus one (or 16 ms by
	* default), then the lastframe is set to the curframe and the loop
	* exits.
	***********************************************************************/
		curframe = SDL_GetTicks();

		if((curframe - lastframe) >= (999/(FPS/(1+GAMESPEED))))
		{


	/***********************************************************************
	* lastframe is set to the value of curframe so the while loop will exit
	***********************************************************************/
			lastframe = curframe;
		} else
		{


	/***********************************************************************
	*   If the TIMING macro is not specified, SDL_Delay will be called.
	* Unless the necessary changes are made to the kernel, SDL_Delay will
	* wait a minimum of 10ms.
	***********************************************************************/
		#ifndef TIMING
			SDL_Delay(1);
		#endif
		}
	} while (lastframe != curframe);


	/***********************************************************************
	*   If FRAMESKIP is enabled, check to see if it is time to display a
	* frame (if frames == 0).  If it is time to display a frame, then the
	* the number of frames to skip is reset, and this function continues.
	* If it is not time to display a frame, then this function returns.
	***********************************************************************/
	if (FRAMESKIP)
	{


	/***********************************************************************
	*   If it is time to display a frame, the program enters this block.
	***********************************************************************/
		if (!frames)
		{


	/***********************************************************************
	*   This assignment resets the number of frames to skip. The variable,
	* named frames, is set back to the value of FRAMESKIP.
	***********************************************************************/
			frames = FRAMESKIP;
		} else
		{


	/***********************************************************************
	*   If it is not time to display a frame, this function returns.
	***********************************************************************/
			return;
		}
	}
#else
	frames++;
	curframe = SDL_GetTicks();

	if ((curframe - lastframe) > 1000) {
		lastframe = curframe;
		printf("%d frames per second\n", frames);
		frames = 0;
	}	
#endif


	/***********************************************************************
	*   This program must lock the surface for direct pixel access. If the
	* lock fails, then an error is printed to STDERR, and the program exits
	* gracefully.
	***********************************************************************/
	if (SDL_MUSTLOCK(screen) && (SDL_LockSurface(screen) == -1))
	{
		perror( "Failed to lock surface when surface needed "
			"to be locked.\n");
		CleanUp(0);
	}





	/***********************************************************************
	*   This function zeros all of the pixels in the window.  This has the
	* effect of making the screen black.
	***********************************************************************/
	memset(screen->pixels, 0, SCREEN_YMAX*screen->pitch);


	/***********************************************************************
	*   If the COLOR macro is defined and USE_COLOR is equal to 3, in other
	* words red/white/green overlay is enabled.  A dark red overlay is
	* painted on the top 64 pixels and a dark green overlay is painted on
	* the bottom 72 pixels.
	***********************************************************************/
#ifdef COLOR
	if (USE_COLOR == 3)
	{
		memset(screen->pixels, 32, 64*screen->pitch);
		memset(screen->pixels + (SCREEN_YMAX-72)*screen->pitch, 1,
			72*screen->pitch);
	}
#endif



	/***********************************************************************
	*   If the GREEN macro and the USE_COLOR global variable are set, with
	* SDL_MapRGB, the color green can be mapped to the current pixel format
	* (16 bits per pixel plane).  This value is stored in the variable
	* named pixel and is used for the on pixels.  This function does not
	* give the most accurate color match.
	*
	* Color - BRIGHT GREEN - 25% Red, 100% Green, 0% Blue.
	*
	*   If the GREEN macro or the USE_COLOR global are not set, the pixel
	* is set to white.
	*
	* Color - BRIGHT GREEN - 100% Red, 100% Green, 100% Blue.
	***********************************************************************/
#ifdef GREEN
	pixel = USE_COLOR ? SDL_MapRGB(screen->format, 0x40, 0xFF, 0x00) :
			    SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
#else
	pixel = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
#endif




	/***********************************************************************
	*   This is the accurate pixel plotting algorithm.  It is the same as
	* the algorithm below, but it is not optimized for Space Invaders. This
	* algorithm is only used when the OLDGFX macro is set.  This routine is
	* not documented because it is for demonstration purposes only.
	***********************************************************************/
#ifdef OLDGFX
	if (1) {
		for (i = 0; i < SCREEN_YMAX; i++) {
			for (j = 0; j < SCREEN_XMAX; j++) {
				bits = (Uint16 *)screen->pixels + j +
					/* SCREEN_XMAX*SCREEN_YMAX -*/
					 i*SCREEN_XMAX;
				if (VIDEORAM[
					((SCREEN_YMAX>>3)*SCREEN_XMAX)-
					((SCREEN_YMAX>>3)*(SCREEN_XMAX-j))+
					((SCREEN_YMAX-1)>>3) - (i>>3)
					     ] & (1<<(7-(i%8))))
					*bits = pixel;
			}
		}
	} else
#endif

	/***********************************************************************
	*   This routine loops 7168 times, checking to see if a pixel is turned
	* on or not.  First it checks if any pixel in a row of 8 pixels is
	* enabled. If one of them is enabled, this routine loops through the
	* individual bits, checking to see if they are enabled or not.  If a
	* pixel is enabled, this routine plots it on the screen.
	***********************************************************************/
	for (i = 0; i < 0x1C00; i++)
	{

	    if (VIDEORAM[(i)^(ENDIAN)])
		for (j = 0; j < 8; j++)
		    if (VIDEORAM[(i)^(ENDIAN)] & (1<<j))
		    {


	/***********************************************************************
	*   Before plotting the pixels on the surface, certain calculations
	* must be made to determine where the pixel needs to be plotted.
	* Explaination of calculations below.
	*
	*
	*   First this routine obtains a pointer to the actual graphics buffer.
	* Since it is a 16-bit surface, it must be cast to an unsigned 16-bit
	* integer.
	*	(Uint16 *)screen->pixels
	*
	*   Next this routine must calculate the offset.
	*   This is calculated from the value of i%32 and shift it's value to
	* the left.
	*	(i%0x20) << 3
	*
	*   This value is added to j, which is the offset of bits in the video
	* memory.
	*	(i%0x20) << 3) + j
	*
	*   After this value is calculated, it must be subtracted from 255.
	* This value is the y value.
	*	255 - ((i%0x20) << 3) + j)
	*
	*   Once this is calculated, this routine must multiply this with the
	* width of the surface.  This value is the y offset.
	*	(255 - ((i%0x20) << 3) + j)) * (screen->w)
	*
	*   Finally, the x offset must be added to the y offset to get the
	* total offset.
	*	(255 - ((i%0x20) << 3) + j)) * (screen->w) + (i>>5)
	*
	*   Once the total offset has been calculated, it can be added it
	* back into the graphics buffer pointer to move to the pixel that needs
	* to be changed.
	*	(Uint16 *)screen->pixels + (255 - ((i%20) << 3) + j)) *
	*		(screen->w) + (i>>5)
	***********************************************************************/
			bits = (Uint16 *)screen->pixels + 
				((SCREEN_YMAX - 1 - (((i%0x20) << 3) + j))
				 * (screen->w)) + (i>>5);




	/***********************************************************************
	*   If the COLOR macro is defined and USE_COLOR is equal to or greater
	* than 2, in other words red/white/green is enabled.  If the y offset
	* is less than 72, the pixel plotted will be red.  If the y offset is
	* greater than 184, the pixel plotted will be green.  If the y offset
	* is between 72 and 184, the pixel plotted will be white. If USE_COLOR
	* is lesser than 2, the pixel will be white.
	***********************************************************************/
	#ifdef COLOR
		    if (USE_COLOR >= 2)
		    {
			if (SCREEN_YMAX - ((i%0x20)<<3) < 72)
			    *bits = pixelr;
			else if (SCREEN_YMAX - ((i%0x20)<< 3) > 184)
			    *bits = pixelg;
			else
			    *bits = pixelw;
		    } else
	#endif


	/***********************************************************************
	*   Set the pixel in the graphics buffer.
	***********************************************************************/
			*bits = pixel;
		    }
	}


	/***********************************************************************
	*   If the surface must be locked (and is currently locked), it must
	* be unlocked to continue.
	***********************************************************************/
	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);


	/***********************************************************************
	*   Finally, the screen must be updated (force it to copy the current
	* buffer to the onscreen buffer.)
	***********************************************************************/
	SDL_UpdateRect(screen, 0, 0, 0, 0);

}

