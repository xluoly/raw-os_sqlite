#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include	<conio.h>

#include	<windows.h>
#include	<mmsystem.h>

#include <raw_api.h>

#pragma comment( lib, "Winmm.lib" )


extern RAW_U8 test_started_raw;


#define  TEST_TASK_STK_SIZE 10240

PORT_STACK zero_interrupt_test_stack1[TEST_TASK_STK_SIZE];
RAW_TASK_OBJ zero_interrupt_test_obj;

static RAW_SEMAPHORE s11;


typedef  void  (*SIMULTED_INTERRUPT_TYPE)();


extern SIMULTED_INTERRUPT_TYPE simulated_zero_fun;

static int count_simulated_interrupt;

static void simulated_interrupt_test()
{
	count_simulated_interrupt++;
	
	if (count_simulated_interrupt == 1) {
		count_simulated_interrupt = 0;
		raw_semaphore_put(&s11);
		raw_semaphore_put(&s11);
		raw_semaphore_put(&s11);
		raw_semaphore_put(&s11);
	}

}

RAW_U32 count_zero_interrupt;
void test_zero_interrupt(void *pa)
{
	RAW_U32 space;

	raw_semaphore_create(&s11, "sem1", 0);
	simulated_zero_fun = simulated_interrupt_test;

	space = raw_get_system_global_space();
	
	vc_port_printf("space is %d\n", space);
	
	while (1) {

		raw_semaphore_get(&s11,  RAW_WAIT_FOREVER);
		count_zero_interrupt++;
		vc_port_printf("get semphore %d\n", count_zero_interrupt);


	}




}



void zero_interrupt_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;

	simulated_zero_fun = 0;
	raw_task_create(&zero_interrupt_test_obj, (RAW_U8  *)"task1", 0,
	                         4, 0,  zero_interrupt_test_stack1, 
	                         TEST_TASK_STK_SIZE ,  test_zero_interrupt, 1);

}

