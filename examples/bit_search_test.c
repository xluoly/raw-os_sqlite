/*
     raw os - Copyright (C)  Lingjun Chen(jorya_txj).

    This file is part of raw os.

    raw os is free software; you can redistribute it it under the terms of the 
    GNU General Public License as published by the Free Software Foundation; 
    either version 3 of the License, or  (at your option) any later version.

    raw os is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
    without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. if not, write email to jorya.txj@gmail.com
                                      ---

    A special exception to the LGPL can be applied should you wish to distribute
    a combined work that includes raw os, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/


/* 	2012-11  Created by jorya_txj
  *	xxxxxx   please added here
  */


#include <raw_api.h>



RAW_U32 bit_search_array[8];
extern RAW_U8 test_started_raw;

void bit_search_test()
{

	RAW_U32 ret;
	RAW_S32 offset = 62;
	RAW_U32 *tmp  = bit_search_array;
	
	if (test_started_raw) {

	return;

	}

	test_started_raw = 1;
	
	*tmp |= 1 << 3;
	*tmp |= 1 << 4;

	*tmp |= 1 << 31;

	tmp++;

	*tmp |= 1;
	*tmp |= 1 << 1;
	//*tmp |= 1 << 30;

	*tmp |= 1 << 31;

	ret = bit_search_first_one(bit_search_array, offset,  256 - offset);

}



extern RAW_U8 test_started_raw;


void idle_schedule_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;

	
}




