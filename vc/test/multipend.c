#include <raw_api.h>

#define  TEST_TASK_STK_SIZE 2048


PORT_STACK test_multipend_stack[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ test_multipend_obj;


PORT_STACK test_multipend_stack2[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ test_multipend_obj2;

RAW_QUEUE q_multi;
RAW_VOID *test_multi_queue[100];


RAW_QUEUE q_multi2;
RAW_VOID *test_multi_queue2[100];



RAW_TASK_OBJ test_multipend_obj3;
PORT_STACK test_multipend_stack3[TEST_TASK_STK_SIZE];


RAW_TASK_OBJ test_multipend_obj4;
PORT_STACK test_multipend_stack4[TEST_TASK_STK_SIZE];


RAW_SEMAPHORE s1_multi;
RAW_EVENT event_queue_test;


void notify_queue_1(RAW_QUEUE *ptr)
{
	raw_semaphore_put(&s1_multi);

}

void notify_queue_2(RAW_QUEUE *ptr)
{

	
	raw_semaphore_put(&s1_multi);


}


void notify_queue_4(RAW_QUEUE *ptr)
{
	raw_event_set(&event_queue_test, 0x1, RAW_OR);

}

void notify_queue_5(RAW_QUEUE *ptr)
{

	raw_event_set(&event_queue_test, 0x2, RAW_OR);
}



void test_multipend(void * pParam)
{

	int c, d;
	c = 100;
	d = 120;

	
	raw_queue_create(&q_multi, "queue1", (RAW_VOID **)&test_multi_queue, 100);
	raw_queue_create(&q_multi2, "queue1", (RAW_VOID **)&test_multi_queue2, 100);

	raw_queue_send_notify(&q_multi, notify_queue_1);
	raw_queue_send_notify(&q_multi2, notify_queue_2);


	while (1) {

		raw_sleep(RAW_TICKS_PER_SECOND);
		raw_queue_post_notify(&q_multi, &c);
		//raw_queue_post_notify(&q_multi2, &d);
	}

}


void test_multipend2(void * pParam)
{
	RAW_U16 ret;
	int *msg_app2;
	
	raw_semaphore_create(&s1_multi, "sem1", 0);

	
	while (1) {
		
		vc_port_printf("Before get event\n");
		
		raw_semaphore_get(&s1_multi,  RAW_WAIT_FOREVER);
		ret = raw_queue_receive (&q_multi, RAW_NO_WAIT, (RAW_VOID **)&msg_app2);

		if (ret == RAW_SUCCESS) {

			vc_port_printf("queue11111 receive %x\n", *msg_app2);

		}


		ret = raw_queue_receive (&q_multi2, RAW_NO_WAIT, (RAW_VOID **)&msg_app2);

		if (ret == RAW_SUCCESS) {

			vc_port_printf("queue222222 receive %x\n", *msg_app2);

		}
		

	}

}


void test_multipend3(void * pParam)
{

	int c, d;
	c = 100;
	d = 120;

	
	raw_queue_create(&q_multi, "queue1", (RAW_VOID **)&test_multi_queue, 100);
	raw_queue_create(&q_multi2, "queue1", (RAW_VOID **)&test_multi_queue2, 100);

	raw_queue_send_notify(&q_multi, notify_queue_4);
	raw_queue_send_notify(&q_multi2, notify_queue_5);


	while (1) {

		raw_sleep(RAW_TICKS_PER_SECOND);
		raw_queue_end_post(&q_multi, &c);
		raw_queue_end_post(&q_multi2, &d);
	}

}



void test_multipend4(void * pParam)
{
	RAW_U16 ret;
	int *msg_app2;
	RAW_U32 flag;
	
	raw_event_create(&event_queue_test, (RAW_U8 *)"flag1", 0);
	
	while (1) {
		
		vc_port_printf("Before get event\n");

		raw_event_get(&event_queue_test, 0x3, RAW_AND_CLEAR,  &flag, RAW_WAIT_FOREVER);
		ret = raw_queue_receive (&q_multi, RAW_NO_WAIT, (RAW_VOID **)&msg_app2);

		if (ret == RAW_SUCCESS) {

			vc_port_printf("queue11111 receive %x\n", *msg_app2);

		}


		ret = raw_queue_receive (&q_multi2, RAW_NO_WAIT, (RAW_VOID **)&msg_app2);

		if (ret == RAW_SUCCESS) {

			vc_port_printf("queue222222 receive %x\n", *msg_app2);

		}
		

	}

}


extern RAW_U8 test_started_raw;

void multipend_or_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	raw_task_create(&test_multipend_obj, (RAW_U8  *)"task1", 0,
	                         10, 0,  test_multipend_stack, 
	                         TEST_TASK_STK_SIZE ,test_multipend, 1); 

	raw_task_create(&test_multipend_obj2, (RAW_U8  *)"task1", 0,
	                         11, 0,  test_multipend_stack2, 
	                         TEST_TASK_STK_SIZE ,test_multipend2, 1); 
  
}


void multipend_and_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	raw_task_create(&test_multipend_obj3, (RAW_U8  *)"task1", 0,
	                         10, 0,  test_multipend_stack3, 
	                         TEST_TASK_STK_SIZE ,test_multipend3, 1); 

	raw_task_create(&test_multipend_obj4, (RAW_U8  *)"task1", 0,
	                         11, 0,  test_multipend_stack4, 
	                         TEST_TASK_STK_SIZE ,test_multipend4, 1); 
  
}


