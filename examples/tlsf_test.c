#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <conio.h>

#include  <windows.h>
#include  <mmsystem.h>

#include <raw_api.h>
#include <mm/raw_tlsf.h>

#pragma comment( lib, "Winmm.lib" )



extern RAW_U8 test_started_raw;

#define  TEST_TASK_STK_SIZE (10 * 1024)

static PORT_STACK mem_test_stack_1[TEST_TASK_STK_SIZE];
static RAW_TASK_OBJ mem_test_obj_1;


static PORT_STACK mem_test_stack_2[TEST_TASK_STK_SIZE];
static RAW_TASK_OBJ mem_test_obj_2;

static PORT_STACK mem_test_stack_3[TEST_TASK_STK_SIZE];
static RAW_TASK_OBJ mem_test_obj_3;

static PORT_STACK mem_test_stack_4[TEST_TASK_STK_SIZE];
static RAW_TASK_OBJ mem_test_obj_4;


#define POOL_SIZE 1024 * 1024

// Pool size is in bytes.
char pool[POOL_SIZE];



#define POOL_SIZE_T 1024 * 10


// Pool size is in bytes.
char p0[POOL_SIZE_T];
char p1[POOL_SIZE_T];
char p2[POOL_SIZE_T];
char p3[POOL_SIZE_T];


#define POOL_SIZE_D 1024 * 100
char p_test_4[POOL_SIZE_D];


static void mem_test_1(void * pParam)
{

	int *ptr[100];
	int i, free_mem;

	free_mem = init_memory_pool(POOL_SIZE, pool);
	vc_port_printf("Total free memory= %d\n", free_mem);
	for (i=0; i< 100; i++)
		if (!(ptr[i]=(int*)malloc_ex(1024, pool))){
			vc_port_printf("Error\n");
		}
	for (i=0; i< 100; i++)
		free_ex(ptr[i], pool);

	destroy_memory_pool(pool);
	vc_port_printf("Test OK 1\n");

	while (1) {

		

	}
  
}

static void mem_test_2(void * pParam)
{

	char *ptr;

	vc_port_printf("Pools' addresses: (0) %p (1) %p (2) %p (3) %p\n", p0, p1, p2, p3);
	init_memory_pool(POOL_SIZE_T, p0);
	add_new_area(p3, POOL_SIZE_T, p0);
	add_new_area(p1, POOL_SIZE_T, p0);
	add_new_area(p2, POOL_SIZE_T, p0);
 
	if (!(ptr=(char*)malloc_ex(POOL_SIZE_T*2, p0))){
		vc_port_printf("Error\n");

	}

	free_ex(ptr, p0);

	destroy_memory_pool(p0);
	vc_port_printf("Test OK 2\n");
	while (1) {

		

	}
  
}

static void mem_test_3(void * pParam)
{
	int *ptr[100];
	int i;
	static int j=0;
	init_memory_pool(POOL_SIZE_D,p_test_4);

	for (i=0; i< 100; i++)
		if (!(ptr[i]=(int*)tlsf_malloc(256 + rand()%1024))){
			vc_port_printf("%d Error\n",j);
			while(1);
		}
		else{
			j++;
		}

	for (i=0; i< 100; i++)
		tlsf_free(ptr[i]);
	
	vc_port_printf("Test OK 3\n");

	while (1) {

		

	}
  
}

static void mem_test_4(void * pParam)
{

	while (1) {

		

	}
  
}




void tlsf_test_1()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	
	raw_task_create(&mem_test_obj_1, (RAW_U8  *)"task1", 0,
	                         1, 0,  mem_test_stack_1, 
	                         TEST_TASK_STK_SIZE ,  mem_test_1, 1); 



}


void tlsf_test_2()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	
	raw_task_create(&mem_test_obj_2, (RAW_U8  *)"task1", 0,
	                         2, 0,  mem_test_stack_2, 
	                         TEST_TASK_STK_SIZE ,  mem_test_2, 1); 



}


void tlsf_test_3()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	
	raw_task_create(&mem_test_obj_3, (RAW_U8  *)"task1", 0,
	                         1, 0,  mem_test_stack_3, 
	                         TEST_TASK_STK_SIZE ,  mem_test_3, 1); 



}

