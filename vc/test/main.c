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


/* 	2012-8  Created by jorya_txj
  *	xxxxxx   please added here
  */


#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include	<conio.h>

#include	<windows.h>
#include	<mmsystem.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <stdarg.h>
#include <assert.h> 
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>

#include  	<stdio.h>
#include  	<string.h>
#include  	<ctype.h>
#include  	<stdlib.h>
#include	<conio.h>
#include 	<stdarg.h>
#include	<windows.h>
#include	<mmsystem.h>
#include  	<assert.h> 


#include <raw_api.h>


#pragma comment( lib, "Winmm.lib" )

extern void VCInit(void);
extern void fat_test(void);
void sqlite_test(int argc, char **argv);

RAW_U8 test_started_raw;


int main(int argc, char * argv[])
{
	VCInit();
	start_vc_timer(10);
	raw_os_init();	
	sqlite_test(argc, argv);
	raw_os_start();

	return 0;
	
}





