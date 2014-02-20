#include <raw_api.h>

extern RAW_U8 test_started_raw;


typedef  void    (*SIMULTED_INTERRUPT_TYPE)();
extern SIMULTED_INTERRUPT_TYPE simulated_interrupt_fun;

EVENT_HANLDER p11;
EVENT_HANLDER p22;


static RAW_U32  handle_count;
static RAW_U32  handle_count2;


#define  TEST_TASK_STK_SIZE 10240

static PORT_STACK test_task_stack11[TEST_TASK_STK_SIZE];

static RAW_TASK_OBJ test_task_obj11;


void  handle_event1(RAW_U8 ev, void *data)
{
	handle_count++;
	vc_port_printf("111 is %d\n", handle_count);
}


void  handle_event2(RAW_U8 ev, void *data)
{
	handle_count2++;
	vc_port_printf("222 is %d\n", handle_count);

}



void post_event()
{
	raw_task_0_post(&p11, 1, 0);
	raw_task_0_post(&p22, 2, 0);
}


static void test_task11(void * pParam) 
{	
	
	p11.handle_event = handle_event1;
	p22.handle_event = handle_event2;
	
	simulated_interrupt_fun = post_event;
	
	while(1) {
	
		raw_sleep(100);
		
	}
	
}

void task_0_test()
{

	if (test_started_raw) {

		return;

	}

	//test_started_raw = 1;


	
	raw_task_create(&test_task_obj11, (RAW_U8	*)"task1", 0,
								 2, 0,	test_task_stack11, 
								 TEST_TASK_STK_SIZE ,  test_task11, 1); 

}


