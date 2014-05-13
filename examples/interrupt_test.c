#include  <raw_api.h>
#include  <mm/raw_page.h>
#include  <mm/raw_malloc.h>
#include  <fifo.h>




#define  TEST_TASK_STK_SIZE (30 * 1024)

PORT_STACK interrupt_test_stack[TEST_TASK_STK_SIZE];
PORT_STACK interrupt_test_stack2[TEST_TASK_STK_SIZE];


RAW_TASK_OBJ  interrupt_test_obj;
RAW_TASK_OBJ  interrupt_test_obj2;

extern RAW_U32 test_page_mem[];
MEM_POOL mem_pool_test2;
RAW_QUEUE q11;
RAW_VOID *test_queue11[1000];

int flag_hook;
int flag_hook2;

unsigned int queue_number_max;
struct raw_fifo fifo2;
RAW_SEMAPHORE s1_i;

typedef  void    (*SIMULTED_INTERRUPT_TYPE)();

extern SIMULTED_INTERRUPT_TYPE simulated_fun;


void simulated_interrupt1()
{
	RAW_U16 ret;
	void *aa1;
	unsigned char *data;
	int i; 
	
	if (flag_hook) {
		ret = raw_block_allocate(&mem_pool_test2, (RAW_VOID **)&aa1);

		if (ret == RAW_SUCCESS) {
			data = aa1;
			
			for (i = 0; i < 256; i++) {
			
				*data++ = i;
		
			}
			
			raw_queue_end_post(&q11, aa1);

			if (q11.msg_q.current_numbers > queue_number_max)
				queue_number_max = q11.msg_q.current_numbers;

			
		}
		
		
	}

}


unsigned int simulated_interrupt2_count;
void simulated_interrupt2()
{
	int i; 
	unsigned char *temp;
	unsigned char data[128];
	
	if (flag_hook2) {

		temp = data;
		
		for (i = 0; i < 128; i++) {
			
				*temp++ = i;
		
		}
	
		fifo_in(&fifo2, data, 128);
		simulated_interrupt2_count++;

		raw_semaphore_put(&s1_i);
			
	}
		
		
}






void i_test_task(void * pParam)
{
	void *mem_pool2;
	void *queue_ret;
	int i;
	unsigned char *data;

	simulated_fun = simulated_interrupt1;
	
	raw_page_init(test_page_mem, test_page_mem + 1024*1024);
	raw_malloc_init();

	mem_pool2 = raw_malloc(10240);
	
	raw_block_pool_create(&mem_pool_test2, (RAW_U8  *)"block", 256, mem_pool2, 256 * 4);
	raw_queue_create(&q11, "queue1", (RAW_VOID **)&test_queue11, 1000);
	flag_hook = 1;
	
	while (1) {

		raw_queue_receive (&q11, RAW_WAIT_FOREVER, &queue_ret);
		
		vc_port_printf("queue ret is %p\n", queue_ret);
		data = queue_ret;
		
		for (i = 0; i < 256; i++) {
			
			vc_port_printf("data is %d\n", *data++);
			
		}
		
		raw_block_release(&mem_pool_test2, queue_ret);
	
	}

}


unsigned char fifo_interrupt_data[2048];


void i_test_task2(void * pParam)
{
	unsigned char *data;
	RAW_U8 *fifo_space_data;
	
	int i;
	int dd = 0;
	simulated_fun = simulated_interrupt2;
	
	raw_page_init(test_page_mem, test_page_mem + 1024 * 1024);
	raw_malloc_init();
	
	raw_semaphore_create(&s1_i, "sem1", 0);
	fifo_space_data = raw_malloc(2048);

	fifo_init(&fifo2, fifo_space_data, 2048);
	flag_hook2 = 1;
	
	while (1) {
		
		raw_semaphore_get(&s1_i, RAW_WAIT_FOREVER);

		i = fifo_out_all(&fifo2, fifo_interrupt_data);
			
		data = fifo_interrupt_data;
		
		for (i = 0; i < 128; i++) {
			dd++;
			vc_port_printf("data is %d %d\n", *data++, dd);
			
		}

	}

}


extern RAW_U8 test_started_raw;

void interrupt_mode_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;

	#if 0
	
	raw_task_create(&interrupt_test_obj, (RAW_U8  *)"task1", 0,
	                         1, 0,  interrupt_test_stack, 
	                         TEST_TASK_STK_SIZE ,  i_test_task, 1); 

	#else
	
	
	raw_task_create(&interrupt_test_obj2, (RAW_U8  *)"task1", 0,
	                         2, 0,  interrupt_test_stack2, 
	                         TEST_TASK_STK_SIZE ,  i_test_task2, 1); 
	#endif
	

}



