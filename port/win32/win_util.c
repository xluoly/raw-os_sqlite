
/*
     raw os - Copyright (C)  Lingjun Chen(jorya_txj).

    This file is very private and it should not open to any person.If you see this file, please delete it immediately.

*/


/* 	2012-4  Created by jorya_txj
  *	
  */


#include 	<raw_api.h>
#include    <simple_printf.h>
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
#include	<win_util.h>

#define  WINDOWS_ASSERT(CON)    if (!(CON)) { \
									printf("If you see this error, please contact author txj, thanks\n");\
									assert(0);\
								}


void user_put_char(unsigned char c)
{

	putchar(c);

}


