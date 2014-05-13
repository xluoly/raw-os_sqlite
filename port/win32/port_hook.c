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

#include 	<raw_api.h>
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
#include    <rtc_config.h>
#include    <rtc.h>


#if (CONFIG_RAW_USER_HOOK > 0)

RAW_VOID raw_os_init_hook()
{

}

extern void port_delete_hook(RAW_TASK_OBJ *task_ptr);

RAW_VOID raw_task_delete_hook(RAW_TASK_OBJ *task_ptr)
{
	port_delete_hook(task_ptr);
	
}


RAW_VOID raw_task_abort_hook(RAW_TASK_OBJ *task_ptr)
{


}




RAW_VOID raw_idle_task_hook()
{

	
}

extern void prothread_start(void);

RAW_VOID raw_idle_coroutine_hook()
{
	//prothread_start();
	idle_run(); 

}


RAW_VOID raw_task_switch_hook()
{
	
}


typedef  void    (*SIMULTED_INTERRUPT_TYPE)();


SIMULTED_INTERRUPT_TYPE simulated_fun;
SIMULTED_INTERRUPT_TYPE event_fun;
static int tick_count_simulate;
static int simulated_fun_count;

extern void proto_tick_handler();


RAW_VOID raw_tick_hook()
{ 
	simulated_fun_count++;

	if (simulated_fun_count == 5) {
		simulated_fun_count = 0;
		if (simulated_fun) {

			simulated_fun();
		}
	}
	
	tick_count_simulate++;

	if (tick_count_simulate == 20) {
		tick_count_simulate = 0;
		if (event_fun) {

			event_fun();
		}
	}

	Clk_SignalClk();
	
	proto_tick_handler();

}


void task_create_hook(RAW_TASK_OBJ  *task_obj)
{

	
}



#endif



