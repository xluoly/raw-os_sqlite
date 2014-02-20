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


/* 	2012-4  Created by jorya_txj
  *	xxxxxx   please added here
  */


#include 	<raw_api.h>

#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>

#include  	<stdio.h>
#include  	<string.h>
#include  	<ctype.h>
#include  	<stdlib.h>
#include	<conio.h>
#include 	<stdarg.h>
#include	<windows.h>
#include	<mmsystem.h>
#include  	<assert.h> 




#define  WINDOWS_ASSERT(CON)    if (!(CON)) { \
									printf("If you see this error, please contact author txj, thanks\n");\
									assert(0);\
								}


static void simulated_interrupt_process( void );



/*-----------------------------------------------------------*/

/* The WIN32 simulator runs each task in a thread.  The context switching is
managed by the threads, so the task stack does not have to be managed directly,
although the task stack is still used to hold an xThreadState structure this is
the only thing it will ever hold.  The structure indirectly maps the task handle 
to a thread handle. */
typedef struct
{
	/* Handle of the thread that executes the task. */
	void *pvThread;

} xThreadState;



/* An event used to inform the simulated interrupt processing thread (a high 
priority thread that simulated interrupt processing) that an interrupt is
pending. */
static void *timer_or_task_switch_event = NULL;

/* Mutex used to protect all the simulated interrupt variables that are accessed 
by multiple threads. */
static void *cpu_global_interrupt_mask = NULL;

volatile unsigned long ulCriticalNesting;
unsigned long port_interrupt_switch;

int port_switch_flag;

void vc_port_printf(char*   f,   ...)
{
	va_list   args;
	
	RAW_CRITICAL_ENTER();
	
	va_start(args, f);
	vprintf(f,args);  
	va_end(args);
	
	RAW_CRITICAL_EXIT();

}



void VCInit(void)
{
	
}



void  *port_stack_init(PORT_STACK  *p_stk_base, RAW_U32 stk_size,  RAW_VOID   *p_arg, RAW_TASK_ENTRY p_task)
{
    
	xThreadState *pxThreadState = NULL;

	/* In this simulated case a stack is not initialised, but instead a thread
	is created that will execute the task being created.  The thread handles
	the context switching itself.  The xThreadState object is placed onto
	the stack that was created for the task - so the stack buffer is still
	used, just not in the conventional way.  It will not be used for anything
	other than holding this structure. */
	pxThreadState = ( xThreadState * ) (p_stk_base + stk_size - 2 - (sizeof(xThreadState) / 4) );

	/* Create the thread itself. */
	pxThreadState->pvThread = CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE ) p_task, p_arg, CREATE_SUSPENDED, NULL );
	
	SetThreadAffinityMask( pxThreadState->pvThread, 0x01 );
	SetThreadPriority( pxThreadState->pvThread, THREAD_PRIORITY_IDLE );
	SetThreadPriorityBoost( pxThreadState->pvThread, TRUE );
	
	return pxThreadState;
	
}



static unsigned int vc_timer_value = 10;

void start_vc_timer(int tick_ms)
{

	vc_timer_value = tick_ms;
}



static volatile RAW_U8 done_timer_init;


static void CALLBACK os_timer_job(unsigned int a,unsigned int b,unsigned long c,unsigned long d,unsigned long e)
{	
	
	ReleaseSemaphore(timer_or_task_switch_event, 1, 0);
				
}


static void start_internal_timer(int tick_ms) 
{	
	done_timer_init = 0;
	timeSetEvent(tick_ms, 0, os_timer_job, 0, TIME_PERIODIC);
	done_timer_init = 1;	
}


typedef  void  (*SIMULTED_INTERRUPT_TYPE)();


SIMULTED_INTERRUPT_TYPE simulated_zero_fun;
SIMULTED_INTERRUPT_TYPE simulated_interrupt_fun;


void raw_start_first_task(void)
{
	void *pvHandle;
	xThreadState *pxThreadState;

	/*Max is assumed to 2*/
	timer_or_task_switch_event = CreateSemaphore( NULL, 0, 2, L"sys_sem_port" );
	
	cpu_global_interrupt_mask = CreateSemaphore( NULL, 1, 1, L"sys_sem_lock" );
	
	if( ( cpu_global_interrupt_mask == NULL ) || ( timer_or_task_switch_event == NULL ) ) {
	
		WINDOWS_ASSERT(0);
	}

	/* Set the priority of this thread such that it is above the priority of 
	the threads that run tasks.  This higher priority is required to ensure
	simulated interrupts take priority over tasks. */
	pvHandle = GetCurrentThread();
	
	if ( pvHandle == NULL ) {
	
		 WINDOWS_ASSERT(0);
	}
	

	
	if (SetThreadPriority( pvHandle, THREAD_PRIORITY_TIME_CRITICAL) == 0) {

		WINDOWS_ASSERT(0);
	}
	
	SetThreadPriorityBoost(pvHandle, TRUE);
	SetThreadAffinityMask( pvHandle, 0x01 );

	start_internal_timer(vc_timer_value);

	
	pxThreadState = ( xThreadState * ) *( ( unsigned long * ) raw_task_active );
	ulCriticalNesting = 0;

	/* Bump up the priority of the thread that is going to run, in the
	hope that this will asist in getting the Windows thread scheduler to
	behave as an embedded engineer might expect. */
	ResumeThread( pxThreadState->pvThread );

	/* Handle all simulated interrupts - including yield requests and 
	simulated ticks. */
	simulated_interrupt_process();
		
	
	
}

int enter_interrupt_flag;

void raw_int_switch()
{

	port_interrupt_switch = 1;
}

extern RAW_U32 idle_tick_start;

static void simulated_interrupt_process( void )
{
	DWORD ret = 0xffffffff;
	BOOL end_ret = 0;
	
	xThreadState *pxThreadState;
	void *pvObjectList[ 2 ];
	

	pvObjectList[ 0 ] = cpu_global_interrupt_mask;
	pvObjectList[ 1 ] = timer_or_task_switch_event;

	for(;;)
	{
		ret = WaitForMultipleObjects( sizeof( pvObjectList ) / sizeof( void * ), pvObjectList, TRUE, INFINITE );
		
		
		if (ret == 0xffffffff) {
			
			WINDOWS_ASSERT(0);

		}


		/*Must be timer interrupt here*/
		if (port_switch_flag == 0) {

			enter_interrupt_flag = 1;
			raw_enter_interrupt();
			
			if (simulated_zero_fun) {
				simulated_zero_fun();

			}

			if (simulated_interrupt_fun) {
				simulated_interrupt_fun();

			}

			if (idle_tick_start) {
				idle_tick_isr();
			}
			//raw_time_tick();
			task_0_tick_post();
			raw_finish_int();

			enter_interrupt_flag = 0;
			
			if (port_interrupt_switch == 1) {
					
				port_interrupt_switch = 0;
			
				pxThreadState = ( xThreadState *) *(( unsigned long * )raw_task_active);

				if (raw_task_active->task_state == RAW_DELETED) {
				
	
					end_ret = TerminateThread(pxThreadState->pvThread, 0);

					if (end_ret == 0) {

						WINDOWS_ASSERT(0);
					}
				

				}
				
				else {
					
					SuspendThread(pxThreadState->pvThread);
				}

			

				raw_task_active = high_ready_obj;

				/* Obtain the state of the task now selected to enter the 
				Running state. */
				pxThreadState = ( xThreadState * ) ( *( unsigned long *) raw_task_active );
				ResumeThread( pxThreadState->pvThread );
						
			}

		}


		/*Must be task switch here*/
		else {
			
			port_switch_flag = 0;
			
			pxThreadState = ( xThreadState *) *( ( unsigned long * ) raw_task_active);

			if (raw_task_active->task_state == RAW_DELETED) {
				
	
				end_ret = TerminateThread(pxThreadState->pvThread, 0);

				if (end_ret == 0) {

					WINDOWS_ASSERT(0);
				}
				

			}
				

			else {
				
				SuspendThread(pxThreadState->pvThread);
			}


			raw_task_active = high_ready_obj;

			/* Obtain the state of the task now selected to enter the 
			Running state. */
			pxThreadState = ( xThreadState * ) ( *( unsigned long *) raw_task_active );
			ResumeThread( pxThreadState->pvThread );

		}

		
		
		
		ReleaseSemaphore(cpu_global_interrupt_mask, 1, 0);

	}
}


void port_task_switch(void)
{
	/*global interrupt is disabled here so it is safe to change value here*/
	port_switch_flag = 1;	
	ReleaseSemaphore(timer_or_task_switch_event, 1, 0);
		
}



void port_delete_hook(RAW_TASK_OBJ *task_ptr)
{
	xThreadState *terminated_thread;
	BOOL end_ret;
	
	if (raw_task_active != task_ptr) {

		terminated_thread = ( xThreadState *) *( ( unsigned long * ) task_ptr);
		end_ret = TerminateThread(terminated_thread->pvThread, 0);

		if (end_ret == 0) {
		
			WINDOWS_ASSERT(0);
		}
		
	}

	

}

void port_enter_critical( void )
{
	if (raw_os_active == RAW_OS_RUNNING) {
	
		if (enter_interrupt_flag == 1) {

			return;
		}


		if (ulCriticalNesting >= 1) {
			/*interupt is disabled here, so writer is safe here*/
			ulCriticalNesting++;
			return;

		}


		/* The interrupt event mutex is held for the entire critical section,
		effectively disabling (simulated) interrupts. */
		WaitForSingleObject(cpu_global_interrupt_mask, INFINITE);
		
		/*ulCriticalNesting must be 0 here*/
		if (ulCriticalNesting != 0) {

			WINDOWS_ASSERT(0);

		}
		
		/*interupt is disabled, so writer is safe here*/
		ulCriticalNesting++;
		
	}
	
	else {
	
	}	
	
}



void port_exit_critical( void )
{
	
	/* The interrupt event mutex should already be held by this thread as it was
	obtained on entry to the critical section. */

	if(raw_os_active == RAW_OS_STOPPED) { 

		return; 
	}
	

	if (enter_interrupt_flag == 1) {

		return;
	}

	if (ulCriticalNesting >= 2) {
		ulCriticalNesting--;
		return;
	}
	
	ulCriticalNesting--;

	if (ulCriticalNesting != 0) {

		WINDOWS_ASSERT(0);

	}

	ReleaseSemaphore(cpu_global_interrupt_mask, 1, 0);

		
}





