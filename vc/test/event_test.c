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

#include <raw_api.h>

#pragma comment( lib, "Winmm.lib" )



RAW_U16 raw_os_start();



#define  TEST_TASK_STK_SIZE 10240

PORT_STACK test_flag_stack1[TEST_TASK_STK_SIZE];
PORT_STACK test_flag_stack2[TEST_TASK_STK_SIZE];
PORT_STACK test_flag_stack3[TEST_TASK_STK_SIZE];
PORT_STACK test_flag_stack4[TEST_TASK_STK_SIZE];
PORT_STACK test_flag_stack5[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 		test_flag_obj[10];



volatile unsigned long  t2_flag;
volatile unsigned long  t3_flag;
volatile unsigned long  t4_flag;
volatile unsigned long  t5_flag;


RAW_EVENT e1;

extern void  vc_port_printf(char*   f,   ...);
void start_vc_timer(int tick_ms);



void test_flag1(void * pParam) 
{	
	
	raw_event_create(&e1, (RAW_U8 *)"flag1", 0);
	e1.common_block_obj.block_way = RAW_BLOCKED_WAY_FIFO;
	
	while(1)
	{		
		
		raw_event_set(&e1, 0x5, RAW_OR);
		raw_sleep(RAW_TICKS_PER_SECOND);
		
	}
}




void test_flag2(void * pParam) 
{	
	RAW_U32 flag2;
	
	while(1) {
	
		raw_event_get(&e1, 0x3, RAW_OR_CLEAR,  &flag2, RAW_WAIT_FOREVER);
		t2_flag++;
		t2_flag++;
		vc_port_printf("t2_flag is %x\n",  t2_flag);
		vc_port_printf("flag2 is %x\n",  flag2);
	}
}



void test_flag3(void * pParam) 
{	
	RAW_U32 flag3;
	
	while(1) {
		
		 raw_event_get(&e1, 0x5, RAW_AND_CLEAR,  &flag3, RAW_WAIT_FOREVER);
		 t3_flag++;
		vc_port_printf("t3_flag is %x\n",  t3_flag);
		vc_port_printf("flag3 is %x\n",  flag3);
		
	}
}


void test_flag4(void * pParam) 
{	
	RAW_U32 flag4;
	
	while(1)
	{		
		t4_flag++;
	   raw_event_get(&e1, 0x2,  RAW_AND_CLEAR,  &flag4, RAW_WAIT_FOREVER);
      vc_port_printf("44444444 is %x\n",  t4_flag);
		
	}
}



void test_flag5(void * pParam) 
{	
	RAW_U32 flag5;
	
	while(1)
	{		
		t5_flag++;
	   raw_event_get(&e1, 0x2, RAW_AND_CLEAR, &flag5, RAW_WAIT_FOREVER);
      vc_port_printf("5555555555555 is %x\n",  t5_flag);  	
	}
}

extern RAW_U8 test_started_raw;

void flag_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	raw_task_create(&test_flag_obj[1], (RAW_U8  *)"task1", 0,
	                         10, 0,  test_flag_stack1, 
	                         TEST_TASK_STK_SIZE ,  test_flag1, 1); 


	raw_task_create(&test_flag_obj[2], (RAW_U8  *)"task2", 0,
	                         40, 10,   test_flag_stack2, 
	                         TEST_TASK_STK_SIZE ,  test_flag2, 1); 

	raw_task_create(&test_flag_obj[3], (RAW_U8  *)"task3", 0,
	                         41, 10,   test_flag_stack3, 
	                         TEST_TASK_STK_SIZE ,  test_flag3, 1); 

	raw_task_create(&test_flag_obj[4], (RAW_U8  *)"task4", 0,
	                         40, 10,   test_flag_stack4, 
	                         TEST_TASK_STK_SIZE ,  test_flag4, 1); 

	raw_task_create(&test_flag_obj[5], (RAW_U8  *)"task5", 0,
	                         40, 10,   test_flag_stack5, 
	                         TEST_TASK_STK_SIZE ,  test_flag5, 1); 

}

