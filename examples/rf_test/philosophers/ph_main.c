#include <raw_api.h>

#include <rf/rf_config.h>
#include <rf/active_object.h>
#include <rf/active_time_event.h>
#include <rf/active_memory.h>
#include <rf/active_queue_broadcast.h>
#include "dpp.h"



/* Local-scope objects -----------------------------------------------------*/
STATE_EVENT *l_tableQueueSto[2 *N_PHILO];
STATE_EVENT *l_philoQueueSto[N_PHILO][2 * N_PHILO];


PORT_STACK l_philoStk[N_PHILO][4096];      /* stacks for the Philosophers */
PORT_STACK l_tableStk[4096];                       /* stack for the Table */
PORT_STACK l_ucosTaskStk[4096];                 /* stack for the ucosTask */


extern RAW_U8 test_started_raw;
MEM_POOL ph_pool_test;
int ph_mem_pool[1024];




#define  TEST_TASK_STK_SIZE 10240


PORT_STACK 		test_philo_stack[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 	test_philo_obj;



void test_ph(void * pParam)
{

	
	RAW_U8 n;
	RAW_U16 ret;
	
	active_broadcast_queue_init();

	/* instantiate all Philosopher active objects */
   	Philo_ctor();           

	/* instantiate the Table active object */
    Table_ctor();                   
                                              
	/* initialize event pools... */
	ret = raw_block_pool_create(&ph_pool_test, (RAW_U8  *)"block", 12, ph_mem_pool, 1024 * 4);


	RAW_ASSERT(ret == RAW_SUCCESS);

    for (n = 0; n < N_PHILO; ++n) {

		active_object_create((ACTIVE_OBJECT_STRUCT *)AO_Philo[n], n + 20,
               (RAW_VOID **)l_philoQueueSto[n], 2 * N_PHILO,
               l_philoStk[n], 4096, 0);
		
    }
	


	active_object_create((ACTIVE_OBJECT_STRUCT *)AO_Table, 10,
               (RAW_VOID **)l_tableQueueSto, 2 * N_PHILO,
               l_tableStk, 4096, 0);



	while (1) {

		raw_sleep(RAW_TICKS_PER_SECOND);
	}


}


void ph_test()
{

	
	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	raw_task_create(&test_philo_obj, (RAW_U8  *)"task1", 0,
	                         10, 0,  test_philo_stack, 
	                         TEST_TASK_STK_SIZE , test_ph, 1); 


   
}

