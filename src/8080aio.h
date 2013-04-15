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
the input8080a, and output8080a functions.  If this assumption becomes
untrue, then this file should contain a copy of the appropriate documentation
for the input8080a, and output8080a functions.


Contents:
--------
  1.	This file references header files which contain functions to read
	input, and write output.

*******************************************************************************/

#ifndef __8080AIO_H__
#define __8080AIO_H__

/*******************************************************************************
* Included files:
* --------------
*
*   sound.h - This file contains routines necessary for playing sounds.  The
*	sounds are played when the 8080a requests sound output with the OUTp
*	instruction.
*
*   input.h - This file contains routines for reading input when the 8080a
*	requests external device input with the INp instruction.
*
*******************************************************************************/
#include "sound.h"
#include "input.h"


/*******************************************************************************
* Function:
* --------
*
*   input8080a - This function determines what type of input should be read and
*	retrieves the data immediately.  Once the data has been retrieved, it
*	is returned to the calling function.
*******************************************************************************/
Uint8 input8080a (Uint8 data);


/*******************************************************************************
* Function:
* --------
*
*   output8080a - This function determins what type of output should be
*	written to external devices and sends it accordingly.
*******************************************************************************/
void output8080a (Uint8 data, Uint8 acc);

#endif
