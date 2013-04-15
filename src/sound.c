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

   The main functions in this file are InitAudio, and PlaySound. InitAudio
initializes SDL's audio subsystem, by opening the device with the following
setting:  8-bit mono audio at 11Khz.  InitAudio also sets mixaudio as the
callback for sound mixing and unpauses the sound device.  PlaySound determines
what sound needs to be played and queues the sound to be played.

*******************************************************************************/


/*******************************************************************************
* Included Files:
* --------------
*
*   sound.h - This file contains the function references for InitSound and
*	PlaySound.  It also references header files that help this file
*	initialize an audio device, open an audio device, load sounds, and mix
*	sounds.
*
*******************************************************************************/

#include "sound.h"



/*******************************************************************************

InitSound:
---------

  Initializes the audio subsystem for mixing and opens the audio device.  The
default audio settings are set to 8-bit mono audio at 11Khz.  Once the audio
device has been opened, playback is unpaused. If the audio device cannot be
opened or the target defaults can not be set, the function displays an error on
STDERR and forces the program to gracefully exit, freeing any previously
created subsystems.

*******************************************************************************/

void InitSound ()
{
	/***********************************************************************
	* Declarations:
	* ------------
	*
	*   mixaudio - This function definintion is used as a callback for
	* mixing sounds.
	*
	*   fmt - This structure contains information on how the audio device
	* should be opened.
	***********************************************************************/
	extern void mixaudio(void *unused, Uint8 *stream, int len);
	SDL_AudioSpec fmt;


	/***********************************************************************
	* Sound Format Initializations:
	* ----------------------------
	*
	*   fmt.freq - Sets the sampling rate to 11025 hz
	*   fmt.format - Sets the data format to 8 bit
	*   fmt.channels - Sets the channels to mono, 1 channel
	*   fmt.samples - Sets the size of the audio buffer to 512 samples. 
	*   fmt.callback - Sets the audio buffer filling callback to the
	*	mixaudio function.
	*   fmt.userdata - Sets the data that is passed into the callback to
	*	NULL.
	*
	***********************************************************************/
	fmt.freq = 11025;

	/***********************************************************************
	*   Patch provided by Caz [turok2 (at) currantbun (dot) com].
	* "AUDIO_S8 is not supported on BeOS"
	***********************************************************************/
#ifdef __BEOS__
	fmt.format = AUDIO_U8;
#else
	fmt.format = AUDIO_S8;
#endif

	fmt.channels = 1;
	fmt.samples = 512;
	fmt.callback = mixaudio;
	fmt.userdata = NULL;



	/***********************************************************************
	*   This function opens the sound device with the desired parameters
	* set in the SDL_AudioSpec data structure, fmt.  If the sound device
	* fails to open, an error is printed to STDERR, and the program
	* gracefully exits.
	***********************************************************************/
	if (SDL_OpenAudio(&fmt, NULL) != 0)
	{
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		CleanUp();
	}


	/***********************************************************************
	*   This function unpauses the audio callback processing.  The 0 value
	* passed into this function makes the audio device start playing sound.
	***********************************************************************/
	SDL_PauseAudio(0);
}





/*******************************************************************************
* Global Definitions and Variables:
* --------------------------------
*
*   NUM_SOUNDS - This defines the maximum number of sounds that can be played
*	at once.  This number is set to 5, which is the maximum number of
*	sounds Space Invaders can possibly play at once.  At most, the
*	following sounds can play simultaneously:  Invader walking, UFO on
*	screen, Base hit, Invader hit, and Free life.
*
*   CURSOUND - This structure stores an array of sounds that are currently
*	playing.
*
*   sounds - This structure stores information about a sample currently loaded
*	into memory.  Data is the actual sound data, dpos in the current
*	position that is being played, and dlen is the length of the data.
*
*******************************************************************************/
#define NUM_SOUNDS 5

Uint8 CURSOUND[NUM_SOUNDS];

struct sample
{
	Uint8 *data;
	Uint32 dpos;
	Uint32 dlen;
} sounds[NUM_SOUNDS];




/*******************************************************************************

mixaudio:
--------

  This function acts as a callback for filling the audio buffer.


Input:
-----

   unused - This variable is unused.  This variable usually acts as user data.
   stream - This variable is where the mixed sounds will eventually be placed.
   len - This is length of the audio buffer.

*******************************************************************************/

void mixaudio(void *unused, Uint8 *stream, int len)
{
	/***********************************************************************
	* Declarations:
	* ------------
	*
	*   i - This variable is a temporary looping variable.
	*
	*   amount - This variable contains the amount of data left in a sound.
	***********************************************************************/
	Uint8 i;
	Uint32 amount;


	/***********************************************************************
	*   This loop visits each sound in the sound queue and mixes the
	* remaining data that is still in the buffer.
	***********************************************************************/
	for (i=0; i < NUM_SOUNDS; ++i)
	{

	/***********************************************************************
	*   This sets the variable named amount to the current length of the
	* sound minus the current position.  This value is the amount of data
	* that remains to be played.
	***********************************************************************/
		amount = (sounds[i].dlen-sounds[i].dpos);


	/***********************************************************************
	*   If there is more data to be played then can currently be mixed,
	* the variable amount is reset to the variable length.  This setting
	* ensures only len bytes of data will be mixed.
	***********************************************************************/
		if (amount > len)
		{
			amount = len;
		}


	/***********************************************************************
	*   This function mixes the number of bytes contained in the variable
	* amount of data from selected sound with the data in the stream.  This
	* sound is mixed at full volume.
	***********************************************************************/
		SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos],
				amount, SDL_MIX_MAXVOLUME);


	/***********************************************************************
	*   This assignment adjusts the selected sound's position to the piece
	* of data will be played next.
	***********************************************************************/
		sounds[i].dpos += amount;
	}
}





/*******************************************************************************

QueueSound:
----------

   This function loads a sound into the queue and begins playing it.  If the
sound fails to load, an error message is printed to STDERR, and this function
gracefully exits.


Input:
-----

   FileName - This stores the name of the file that will be retrieved from
	the disk and played.
   index - This variable is a pointer to a blank sound slot in the structure
	named sounds.

*******************************************************************************/
void QueueSound (char *FileName, Uint8 index)
{

	/***********************************************************************
	* Declarations:
	* ------------
	*
	*   wave - This variable contains information about a wave file loaded
	*	from the disk.
	*
	*   data - This variable contains the wave file that will be loaded
	*	from the disk.
	*
	*   dlen - This varible contains the length of the data that will be
	*	loaded from the disk.
	*
	*   cvt - This variable stores the converted data once it has been
	*	loaded from the disk and converted to a mono 8-bit sound.
	***********************************************************************/
	SDL_AudioSpec wave;
	Uint8 *data;
	Uint32 dlen;
	SDL_AudioCVT cvt;



	/***********************************************************************
	*   This routine loads a wave from the disk, where FileName is the name
	* of the file to load.  Once the data is retrieved, it is stored in the
	* variable named data.  The length of the data is stored in the
	* variable named dlen, and the wave information is stored in the
	* variable wave.  If loading fails, a debug message is printed to
	* STDERR, and this function returns.
	***********************************************************************/
	if (SDL_LoadWAV(FileName, &wave, &data, &dlen) == NULL)
	{
		fprintf(stderr, "Couldn't load sound %s: %s\n", FileName,
			SDL_GetError());
		return;
	}



	/***********************************************************************
	*   This routine initializes the SDL_AudioCVT structure for conversion.
	* The wave data will be converted into an 8-bit 11khz mono sound.
	***********************************************************************/

	/***********************************************************************
	*   Patch provided by Caz [turok2 (at) currantbun (dot) com].
	* "AUDIO_S8 is not supported on BeOS"
	***********************************************************************/
#ifdef __BEOS__
	SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq,
				AUDIO_U8,    1,		    11025);
#else
	SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq,
				AUDIO_S8,    1,		    11025);
#endif


	/***********************************************************************
	*   This function allocates memory for the converted audio buffer.
	* The buffer must be dlen*cvt.len_mult bytes.  This is defined in
	* the SDL_AudioCVT man page.
	***********************************************************************/
	cvt.buf = malloc(dlen*cvt.len_mult);


	/***********************************************************************
	*   This function copies the wave data to be converted into the new
	* buffer.
	***********************************************************************/
	memcpy(cvt.buf, data, dlen);


	/***********************************************************************
	*   This sets the length of the original audio buffer to the size of
	* the data that was read from disk.
	***********************************************************************/
	cvt.len = dlen;


	/***********************************************************************
	*   This function converts the wave into the format specified by the
	* SDL_BuildAudioCVT function above.
	***********************************************************************/
	SDL_ConvertAudio(&cvt);


	/***********************************************************************
	*   This function frees the memory allocated to the variable named
	* data.
	***********************************************************************/
	SDL_FreeWAV(data);


	/***********************************************************************
	*   This function frees the sound slot if there is currently any data
	* left.
	***********************************************************************/
	if (sounds[index].data)
	{
		free(sounds[index].data);
	}


	/***********************************************************************
	*   This function sets a lock on the callback function so it cannot be
	* called, and the sound data can be loaded.
	***********************************************************************/
	SDL_LockAudio();


	/***********************************************************************
	*   These assignments load the sound data, sound length, and the
	* current sound position into the queue, respectively.
	***********************************************************************/
	sounds[index].data = cvt.buf;
	sounds[index].dlen = cvt.len_cvt;
	sounds[index].dpos = 0;


	/***********************************************************************
	*   This function unlocks the callback function, allowing the new sound
	* data added to the sound queue to begin playing immediately.
	***********************************************************************/
	SDL_UnlockAudio();
}




/*******************************************************************************
* Global Definition:
* -----------------
*
*   SOUNDFILES - This array of strings contains all of the sounds that Space
*	Invaders uses.
*
*
* Sound Definitions:
* -----------------
*
*   0.wav - UFO on screen (repeats)
*   1.wav - Base shot 
*   2.wav - Base hit
*   3.wav - Invader hit
*   4.wav - Invader step 4
*   5.wav - Invader step 1
*   6.wav - Invader step 2
*   7.wav - Invader step 3
*   8.wav - UFO hit
*   9.wav - Free base
*
*******************************************************************************/
const char SOUNDFILES[][16] =
{
	"samp/0.wav",	"samp/1.wav",	"samp/2.wav",	"samp/3.wav",
	"samp/9.wav", 	"",		"",		"",
	"samp/4.wav",	"samp/5.wav",	"samp/6.wav",	"samp/7.wav",
	"samp/8.wav",	"",		"",		""
};



/*******************************************************************************

SwitchSound:
-----------

   This function loads a sound into the queue and begins playing it.  If the
sound fails to load, an error message is printed to STDERR, and the function
gracefully exits.


Input:
-----

   type - This stores the type of sound that is requested.

   index - This variable indicates the sound that the calling function is
   	interested in fetching.

*******************************************************************************/
char *SwitchSound (Uint8 type, Uint8 index)
{

	/***********************************************************************
	* Declarations:
	* ------------
	*
	*   i - This is a temporary looping variable
	***********************************************************************/
	Uint8 i;


	/***********************************************************************
	*   This for loop loops through each value of 2**i, and attempts to
	* find the value of the log base 2 of index.
	***********************************************************************/
	for (i = 0; i < 8; i ++)
		if ((1<<i) == index) break;


	/***********************************************************************
	*   This statement returns the selected sound file.  If the variable
	* type is not equal to 3, the second 8 set of sounds is selected.
	***********************************************************************/
	return (char *)SOUNDFILES[i + ((type == 3) ? 0 : 8)];
}




/*******************************************************************************

PlaySound:
---------

   This function is sent an OUT variable and a type variable and determines
what sounds need to be played from these variables.  Once a sound is found,
it is queued to be played.  If the queue is full, this function returns.


Input:
-----

   OUT - This stores a list of current sounds that need to be played.

   type - This stores the type of sound that is requested.

*******************************************************************************/
void PlaySound (Uint8 OUT, Uint8 type)
{

	/***********************************************************************
	* Declarations:
	* ------------
	*
	*   index - This is the index marking an empty slot where a sound can
	*	be inserted.
	*
	*   i, j - These are temporary looping variables.
	*
	*   lastout - This variable stores the last type 3 sound that has been
	*	played.  If the default value is 0, then there was no last
	*	sound played.
	*  
	*   soundfile - This stores the name of the file that will be queued.
	***********************************************************************/
	Uint8 index, j;
	Uint16 i;
	static Uint8 lastout = 0;
	char *soundfile;



	/***********************************************************************
	*   This for loop visits each sound in the queue and checks to see if
	* it has finished playing.  If the sound has finished playing, it is
	* removed from the queue, and an indexing variable is set to that newly
	* created blank space.
	***********************************************************************/
	for (i=0, index=NUM_SOUNDS; i < NUM_SOUNDS; ++i)
	{

	/***********************************************************************
	*   If the sound's position has reached its length, that means the
	* sound has finished playing.
	***********************************************************************/
		if (sounds[i].dpos == sounds[i].dlen)
		{

	/***********************************************************************
	*   This assignment removes sound i from the queue.
	***********************************************************************/
			CURSOUND[i] = 0;


	/***********************************************************************
	*   This assignment sets the indexing variable to the new empty sound
	* slot.
	***********************************************************************/
			index = i;
		}
	}


	/***********************************************************************
	*   If the the program reaches the end of the loop and no sound has
	* been found, this function returns to the calling function without
	* queueing a sound.
	***********************************************************************/
	if (index == NUM_SOUNDS)
		return;



	/***********************************************************************
	*   This for loop visits each bit in the OUT variable and tests it.  If
	* a bit is set, a sound is selected and queued.
	***********************************************************************/
	for (i=0x01; i < 0x100; i = i<<1)
	{


	/***********************************************************************
	*   This for loop visits each sound in the queue and checks to see if
	* the sound is currently queued.  If the sound is found in the queue,
	* this for loop breaks, and the next sound is tested.
	***********************************************************************/
		for (j=0; j < NUM_SOUNDS; j++)
		{

	/***********************************************************************
	*   If the sound is found in the queue, break.
	***********************************************************************/
			if (i&CURSOUND[j])
				break;
		}



	/***********************************************************************
	*   If the sound is currently not queued, and the it is corresponding
	* value is set in the OUT variable, an attempt is made to select and
	* queue the sound.
	***********************************************************************/
		if (!(i&CURSOUND[j]) && (OUT&i))
		{

	/***********************************************************************
	*   This inserts the new sound to the list of queued sounds.
	***********************************************************************/
			CURSOUND[index] = i;


	/***********************************************************************
	*   This function returns a sound based on the value of type and i.
	* The resultant value is stored in the variable named soundfile.
	***********************************************************************/
			soundfile = SwitchSound(type, i);


	/***********************************************************************
	*   If the sound type is a type 3 sound, and it is the sound requested
	* is between 0x00 and 0x40, then many requests may be generated.
	***********************************************************************/
			if (type == 3 && i > 0x00 && i < 0x40) 
			{


	/***********************************************************************
	*   If the newly requested sound is not the last sound that is 
	* requested, and there are no other sounds requested except for a sound
	* 0x02 is requested, or sound 0x04 is requested this routine resets the
	* value of the variable lastout to the sound that is requested and
	* adds that sound to the queue.
	***********************************************************************/
				if (lastout != i &&
				    ((!(OUT&0x08) && !(OUT&0x04) && i == 0x02)
				    || i == 0x04))
				{
					lastout = i;

					QueueSound(soundfile, index);
				}


	/***********************************************************************
	*   If the newly requested sound is either sound 0x01, 0x08, or 0x10,
	* the sound is immediately queued.
	***********************************************************************/
				if (i == 0x01 || i == 0x08 || i == 0x10)
					QueueSound(soundfile, index);


	/***********************************************************************
	*   If the only sound requested is sound 0x20, the lastout is cleared.
	* This request is usually only generated after the sound that last
	* set the variable named lastout has finished playing.
	***********************************************************************/
				if (OUT == 0x20)
					lastout = 0;
			}



	/***********************************************************************
	*   If the sound type is a type 3 sound and it is the sound requested
	* is between 0x00 and 0x20, then the sound is immediately queued. Once
	* the sound has been queued, this function returns.
	***********************************************************************/
			if (type == 5 && i > 0x00 && i < 0x20)
			{
				QueueSound(soundfile, index);

				return;
			}


	/***********************************************************************
	*   If the sound requested is equal to 0x00 or greater than 0x10, the
	* sound is removed from the list of queued sounds.
	***********************************************************************/
			if (i == 0x00 || i > 0x10)
				CURSOUND[index] = 0;


	/***********************************************************************
	*   If the sound requested is equal to or greater than 0x20, then an
	* unknown sound has been requested.  Debug information is printed to
	* STDOUT.
	***********************************************************************/
			if (i > 0x20)
				printf("Unknown sound %d: 0x%X\n", type, OUT);


	/***********************************************************************
	*   This for loop finds a new blank spot for a sound.
	***********************************************************************/
			for (index=0; index < NUM_SOUNDS; ++index)
				if (CURSOUND [index] == 0)
					break;
		}
	}
}


