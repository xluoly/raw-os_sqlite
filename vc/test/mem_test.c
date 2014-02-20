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

#include <raw_api.h>
#include <mm/raw_page.h>
#include <mm/raw_malloc.h>

#include <fifo.h>
#include <lib_string.h>
#include <mm/slab/config.h>
#include <mm/slab/misc.h>
#include <mm/slab/spinlock.h>
#include <mm/slab/arch.h>
#include <mm/slab/list.h>
#include <mm/slab/buddy.h>
#include <mm/slab/frame.h>
#include <mm/slab/slab.h>
#include <stdlib.h>
#include <mm/heap_4_config.h>


#pragma comment( lib, "Winmm.lib" )



RAW_U16 raw_os_start();



#define  TEST_TASK_STK_SIZE 10240

PORT_STACK test_mem_stack[TEST_TASK_STK_SIZE];


RAW_TASK_OBJ test_mem_obj;




PORT_STACK test_heap_stack[TEST_TASK_STK_SIZE];


RAW_TASK_OBJ test_heap_obj;



extern void  vc_port_printf(char*   f,   ...);
extern RAW_U8 free_check_switch;
void start_vc_timer(int tick_ms);

MEM_POOL mem_pool_test;
int mem_pool[1024];

void *aa;
void *bb;
void *cc;

void test_block(void * pParam)
{
	RAW_U16 ret;
	RAW_U32 i = 0;

	
	raw_block_pool_create(&mem_pool_test, (RAW_U8  *)"blockqewq", 12, mem_pool, 1024 * 4);

	while (1) {
		
		i++;

		ret = raw_block_allocate(&mem_pool_test, (RAW_VOID **)&aa);

		if (ret == RAW_NO_MEMORY) {
			vc_port_printf("RAW_NO_MEMORY1\n");
			RAW_ASSERT(0);
		}

		vc_port_printf("****************\n");
		ret = raw_block_allocate(&mem_pool_test, (RAW_VOID **)&bb);

		if (ret == RAW_NO_MEMORY) {
			vc_port_printf("RAW_NO_MEMORY2\n");
			RAW_ASSERT(0);
		}

		raw_block_release(&mem_pool_test, bb);

		vc_port_printf("allocate block %d\n", i);
	 
	}


}

void test_heap(void * pParam)
{
	RAW_U32 free_size;
	void *mem;
	void *mem2;
	
	while (1) {

		mem = mem_4_malloc(13);
		
		if (mem == NULL) {

			RAW_ASSERT(0);
			
		}
		
		mem2 = mem_4_malloc(17);

		if (mem2 == NULL) {

			RAW_ASSERT(0);
			
		}
		
		free_size = mem_4_free_get();
		vc_port_printf("free_size %d\n", free_size);
		mem_4_free(mem);
		mem_4_free(mem2);
		raw_sleep(RAW_TICKS_PER_SECOND);


	}

	

}



RAW_BYTE_POOL_STRUCT byte_mem_pool_test;
int byte_mem_pool[1024 * 1024 /4];

void test_byte(void * pParam)
{
	RAW_U16 ret;

	RAW_U32 i = 0;

	raw_byte_pool_create(&byte_mem_pool_test, (RAW_U8 *)"byte", byte_mem_pool, 1024 * 1024);

	while (1) {
		
		i++;
		ret = raw_byte_allocate(&byte_mem_pool_test, (RAW_VOID **)&aa, 2022);

		if (ret == RAW_NO_MEMORY) {
			vc_port_printf("RAW_NO_MEMORY\n");
			RAW_ASSERT(0);
		}

		ret = raw_byte_allocate(&byte_mem_pool_test, (RAW_VOID **)&bb, 2000);

		if (ret == RAW_NO_MEMORY) {
			vc_port_printf("RAW_NO_MEMORY\n");
			RAW_ASSERT(0);
		}


		raw_byte_release(&byte_mem_pool_test, bb);
		vc_port_printf("allocate byte %d\n", i);

	}

  

}



RAW_U32 test_page_mem[1024*1024];

extern MACB malloc_macb;
RAW_U32 free_page_count;
void test_page(void * pParam)
{
	void *test1;
	void *test2;

	PAGETBL *pg;
	
	int count = 0;
	raw_page_init(test_page_mem, test_page_mem + 1024*1024);

	pg = system_page_table_get();

	vc_port_printf("111 max page is %d, free page is %d, top page addr is %p\n", 
	pg->maxpage, pg->freepage, pg->top_page);
	
	raw_malloc_init();

	test1 = raw_malloc(188888);
	

	test2 = raw_malloc(18888);

	pg = system_page_table_get();

	vc_port_printf("222 max page is %d, free page is %d, top page addr is %p\n", 
	pg->maxpage, pg->freepage, pg->top_page);
	
	raw_free(test2);
	
	pg = system_page_table_get();

	vc_port_printf("333 max page is %d, free page is %d, top page addr is %p\n", 
	pg->maxpage, pg->freepage, pg->top_page);

	raw_free(test1);


	pg = system_page_table_get();

	vc_port_printf("4444 max page is %d, free page is %d, top page addr is %p\n", 
	pg->maxpage, pg->freepage, pg->top_page);

	
	while (1) {
	    test1 = raw_page_allocate(25);

		if (test1 == 0) {

			vc_port_printf("RAW_NO_MEMORY\n");
			RAW_ASSERT(0);

		}
		
		test2 = raw_page_allocate(25);
		
		if (test2 == 0) {

			vc_port_printf("RAW_NO_MEMORY\n");
			RAW_ASSERT(0);

		}

		raw_page_free(test2);

		vc_port_printf("page success is %d\n", count++);

	}
	
	

}


void test_slab(void * pParam)
{
	void *test_mem1;
	int count = 0;
	int i = 0;
	int ret = 0;
	
	frame_init((RAW_U32)test_page_mem, 48*1024);
	slab_cache_init();
	/*Decide whether the magazine work.*/
	slab_enable_cpucache();
	/*0--on, for user program debugging purpose; 
	  1--off, normally, it should be off.*/
	free_check_switch = 0;


	for(i = 0; i < 20; i++){	
		test_mem1 = raw_slab_malloc(1024);
		test_mem1 = raw_slab_malloc(1024);
		
		if (test_mem1) {
			vc_port_printf("test_mem1 is %p\n", test_mem1);
			ret = raw_slab_free(test_mem1);
			if(ret != 0)
				vc_port_printf("****raw_slab_free failed!\n");
			count++;
		}
		else {
			RAW_ASSERT(0);
		}
	}

	/*free a freed address*/
	if(free_check_switch == 0){
		ret = raw_slab_free(test_mem1);
		if(ret != 0)
			vc_port_printf("****Error: Free a freed obj!\n");
	}

	/*Some debug information*/
	slab_print_list();
	vc_port_printf("\n");
	zone_print_one(0);
	while (1);
}


extern RAW_U8 test_started_raw;

void mem_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	

	#if 0
	
	raw_task_create(&test_mem_obj, (RAW_U8  *)"task2", 0,
	                         10, 0,  test_mem_stack, 
	                         TEST_TASK_STK_SIZE , test_page, 1); 

	#endif

	raw_task_create(&test_heap_obj, (RAW_U8  *)"task_heap", 0,
	                         10, 0,  test_heap_stack, 
	                         TEST_TASK_STK_SIZE , test_heap, 1); 

}



