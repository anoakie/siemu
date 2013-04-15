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

   The main functions in this file are input8080a and output8080a. The function
input8080a collects input data from various 8080a input devices, such as
bit-shifters and joysticks, and returns it to the calling function.  The
function output8080a sends data to various 8080a output devices, such as sound
generation and bit-shift data setting devices.

*******************************************************************************/


/*******************************************************************************
* Included Files:
* --------------
*
*   8080aio.h - This file contains shift values for bitmap shifting.  This
*	header file also contains the function references input8080a and
*	output8080a.
*
*******************************************************************************/

#include "8080aio.h"



/*******************************************************************************
* Declaration and Initialization of Global Variables:
* --------------------------------------------------
*
*   LSHIFT1 - This integer is used as bitmap shift data.  It is set during the
*	OUTp instruction and used during the INp instruction.  This variable
*	defaults to 0.
* 
*   LSHIFT2 - This integer is used as bitmap shift data.  It is set to
*	LSHIFT1's old value during the OUTp instruction and used during the INp
*	instruction.  This variable defaults to 0.
*
*   LSHIFTAMT - This integer is set during the OUTp instruction and used to
*	shift data LSHIFTAMT times in the INp instruction.  This variable
*	defaults to 0.
*
*
* Note:
* ----
*
*   Although these values are changed almost immediately, it is still
* good practice to reset them in case they are accessed before they are set
* again.
*******************************************************************************/
Uint16 LSHIFT1 = 0;
Uint16 LSHIFT2 = 0;
Uint16 LSHIFTAMT = 0;



/*******************************************************************************

input8080a:
----------

   This case switches on the content of data.  If the content of data is equal
to 1, first player input is requested.  If the content of data is equal to 2,
second player input is requested.  If the content of data is equal to 3, a
bitmap shift on the accumulator is requested based on the bit-shift values set
with the OUTp instruction. If the content of data is any other value, then the
request is bad and debugging information is printed to STDOUT.  The program
counter, named PC, is incremented by one.


Input:
-----

   data - This variable is the byte after the INp instruction, RAM[PC].


Output:
------

   This variable is the data that has been read or calculated from the input
device and returned to the main simulator core.


Note:
----

   This function is called directly by main8080a.

*******************************************************************************/

Uint8 input8080a (Uint8 data)
{

	/***********************************************************************
	* Declaration:
	* -----------
	*
	*   retval - This variable contains data read from an input device.
	***********************************************************************/
	Uint8 retval;


	switch (data)
	{

	/***********************************************************************
	*   If the value of the 8080a's memory, RAM[PC], is equal to 1 or 2,
	* then first or second player input is requested.
	***********************************************************************/
		case 1:
		case 2:
			retval = ReadInputDevice(data);
			break;


	/***********************************************************************
	*   If the value of the 8080a's memory, RAM[PC], is equal to 3 then
	* this program requests a bit-shift on the accumulator.  This shift is
	* based on the LSHIFT1, LSHIFT2, and LSHIFTAMT values set when the OUTp
	* instruction is executed with the proper output values.
	***********************************************************************/
		case 3:
			retval = ((LSHIFT1 << LSHIFTAMT) & 0xFF)
			  | ((LSHIFT2 >> (8 - LSHIFTAMT)) & 0xFF);
			break;


	/***********************************************************************
	*   If the value of the 8080a's memory, RAM[PC], is not equal to any of
	* the above cases, then the program requested bad input data. Debug
	* information containing the value of the data is printed to STDOUT.
	***********************************************************************/
		default:
			printf( "INp BAD -> Data = %d\n", data);

			retval = 0;
			break;
	}


	/***********************************************************************
	*   This keyword returns the read input data to the main simulator core.
	***********************************************************************/
	return retval;
}



/*******************************************************************************

output8080a:
-----------

   This case switches on the contents of the data.  If the content of data is
equal to 2, the left shift amount is set, depending on the content of the
accumulator.  If the content of data is equal to 3 or 5, a sound is queued,
depending on the content of the accumulator. If the content of data is equal to
4, the second left shift value is set to the first left shift value.  The first
left shift value is set to the content of the accumulator. If the content of
data is any other value, then the request is bad and debugging information is
printed to STDOUT.


Input:
-----

   data - This variable is the byte after the OUTp instruction, RAM[PC].
   acc - This variable is the accumulator, which is the 8080a's A register.


Note:
----

   This function is called directly by main8080a.

*******************************************************************************/

void output8080a (Uint8 data, Uint8 acc)
{
	switch (data)
	{

	/***********************************************************************
	*   If the value of the 8080a's memory, RAM[PC], is equal to 2, the
	* content of the A register is moved into the left shift amount.
	***********************************************************************/
		case 2:
			LSHIFTAMT = acc;
			break;


	/***********************************************************************
	*   If the value of the 8080a's memory, RAM[PC], is equal to 3, a sound
	* is queued, based on the content of the accumulator.
	***********************************************************************/
		case 3:
			PlaySound(acc, data);
			break;


	/***********************************************************************
	*   If the value of the 8080a's memory, RAM[PC], is equal to 4, the
	* second left shift value is set to the first left shift value.  The
	* first left shift value is set to the content of the accumeulator.
	***********************************************************************/
		case 4:
			LSHIFT2 = LSHIFT1;
			LSHIFT1 = acc;
			break;


	/***********************************************************************
	*   If the value of the 8080a's memory, RAM[PC], is equal to 5, a
	* sound is queued, based on the content of the accumulator.
	***********************************************************************/
		case 5:
			PlaySound(acc, data);
			break;


	/***********************************************************************
	*   If the value of the 8080a's memory, RAM[PC], is equal to 6, the
	* processor ignores this output request.
	***********************************************************************/
		case 6:
			/*
			   Unknown.  May be analog sound generation logic.
			*/
			break;


	/***********************************************************************
	*   If the value of the 8080a's memory, RAM[PC], is not equal to any of
	* the above cases, then the program requested bad output data.  Debug
	* information containing the value of the data and the accumulator are
	* printed to STDOUT.
	***********************************************************************/
		default:
			printf("OUTp BAD -> Data = %d   A = %d\n", data, acc);
			break;
	}
}

