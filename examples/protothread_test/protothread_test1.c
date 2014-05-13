#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>

#include <windows.h>
#include <mmsystem.h>

#include <raw_api.h>

#include <protothread/protothread.h>



#pragma comment( lib, "Winmm.lib" )



extern RAW_U8 test_started_raw;

#define  TEST_TASK_STK_SIZE (10 * 1024)

PORT_STACK p1_test_stack[TEST_TASK_STK_SIZE];



RAW_TASK_OBJ  p1_test_obj;
RAW_U32 idle_count;
RAW_U32 count;
RAW_U32 blinks;


extern clock_time_t current_clock;
extern int raw_printf(const char *format, ...);

PROCESS(hello_world_process, "Hello world process");
PROCESS(blink_process, "LED blink process");
PROCESS(process_yield_test, "LED blink process");



PROCESS_THREAD(hello_world_process, ev, data)
{
	static struct etimer et_hello;
	
	PROCESS_BEGIN();

	etimer_set(&et_hello, RAW_TICKS_PER_SECOND * 2);
	count = 0;

	while(1) {
		PROCESS_WAIT_EVENT();

		if(ev == PROCESS_EVENT_TIMER) {
			
			count ++;
			vc_port_printf("Sensor says #%u\n", count);
			//vc_port_printf("current_clock@@@@ is %d\n", current_clock);
			process_post(&process_yield_test, 0xaa, 0);

			etimer_reset(&et_hello);
		}
	}

	PROCESS_END();
}


PROCESS_THREAD(blink_process, ev, data)
{
	static struct etimer et_blink;
	
	PROCESS_BEGIN();

	blinks = 0;
	etimer_set(&et_blink, RAW_TICKS_PER_SECOND);
	while(1) {


	PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);


	blinks++;
	vc_port_printf("Blink is %d\n",  blinks);
	//vc_port_printf("current_clock^^^^ is %d\n", current_clock);
	etimer_reset(&et_blink);
	}

	PROCESS_END();
}


PROCESS_THREAD(process_yield_test, ev, data)
{
	
	

  PROCESS_BEGIN();

 

  while(1) {

	vc_port_printf("ev is %x\n", ev);
	vc_port_printf("state 1\n");
	PROCESS_WAIT_EVENT_UNTIL(ev == 0xaa);

	vc_port_printf("state 2\n");

    //vc_port_printf("state 2\n");
  }

  PROCESS_END();
}





void prothread_start()
{
	raw_printf("Initialising %d\n", test_started_raw);

	process_init();

	process_start(&etimer_process, NULL);
	process_start(&hello_world_process, NULL);
	process_start(&blink_process, NULL);
	process_start(&process_yield_test, NULL);

	vc_port_printf("Processes running\n");
	while(1) {

	do {

	} while(process_run() > 0);

	idle_count++;

	}


}



void p1_test_task(void * pParam)
{

	prothread_start();	

	while (1) {

		raw_sleep(10000);

	}
  
}




void p1_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	
	raw_task_create(&p1_test_obj, (RAW_U8  *)"task1", 0,
	                         1, 0,  p1_test_stack, 
	                         TEST_TASK_STK_SIZE ,  p1_test_task, 1); 



}




