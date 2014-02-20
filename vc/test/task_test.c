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

PORT_STACK test_task_stack1[TEST_TASK_STK_SIZE];
PORT_STACK test_task_stack2[TEST_TASK_STK_SIZE];
PORT_STACK test_task_stack3[TEST_TASK_STK_SIZE];
PORT_STACK test_task_stack4[TEST_TASK_STK_SIZE];
PORT_STACK test_task_stack5[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 		test_task_obj[10];


volatile unsigned long  t1;
volatile unsigned long  t2;
volatile unsigned long  t3;
volatile unsigned long  t4;
volatile unsigned long  t5;

volatile unsigned long flag1;
volatile unsigned long flag2;
volatile unsigned long flag3;
volatile unsigned long flag4;
volatile unsigned long flag5;

void start_vc_timer(int tick_ms);

int task_count_numbers;

extern void  vc_port_printf(char*   f,   ...);

void test_task1(void * pParam) 
{	
	
	RAW_U32 stack_size;
	
	t2  = 100;
	
	while(1) {
	
		vc_port_printf("t2 is %x\n", t2);
		vc_port_printf("t3 is %x\n", t3);
		vc_port_printf("t4 is %x\n", t4);
		vc_port_printf("t5 is %x\n", t5);
		vc_port_printf("task_count_numbers %d\n", task_count_numbers);

		raw_task_stack_check(&test_task_obj[1], 	&stack_size);
		vc_port_printf("t11* stack_size is %d\n", stack_size);

		raw_task_resume(&test_task_obj[5]);
		raw_sleep(RAW_TICKS_PER_SECOND);
		task_count_numbers++;
		
	}
	
}



void test_task2(void * pParam) 
{	

	while(1) {

			t2 += 1;
			t2 -= 1;
		
	}
}



void test_task3(void * pParam) 
{	
	
	while(1) {
		 raw_disable_sche();
		t3++;
		raw_enable_sche();
	}
}


void test_task4(void * pParam) 
{	
	
	while(1) {
		raw_disable_sche();
		t4++;
		raw_enable_sche();
	}
}



void test_task5(void * pParam) 
{	
	
	while(1) {	
		raw_disable_sche();
		raw_disable_sche();
		t5++;  
		raw_enable_sche();
		raw_enable_sche();
	}
}

extern RAW_U8 test_started_raw;
void task_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;

	raw_task_create(&test_task_obj[1], (RAW_U8  *)"task1", 0,
	                         3, 0,  test_task_stack1, 
	                         TEST_TASK_STK_SIZE ,  test_task1, 1); 




	raw_task_create(&test_task_obj[2], (RAW_U8  *)"task2", 0,
	                         30, 10,   test_task_stack2, 
	                         TEST_TASK_STK_SIZE ,  test_task2, 1); 




	raw_task_create(&test_task_obj[3], (RAW_U8  *)"task3", 0,
	                         30, 10,   test_task_stack3, 
	                         TEST_TASK_STK_SIZE ,  test_task3, 1); 



	raw_task_create(&test_task_obj[4], (RAW_U8  *)"task4", 0,
	                         30, 15,   test_task_stack4, 
	                         TEST_TASK_STK_SIZE ,  test_task4, 1); 

	raw_task_create(&test_task_obj[5], (RAW_U8  *)"task5", 0,
	                         30, 10,   test_task_stack5, 
	                         TEST_TASK_STK_SIZE ,  test_task5, 0); 


	

}


