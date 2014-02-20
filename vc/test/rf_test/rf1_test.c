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


#include  	<stdio.h>
#include  	<string.h>
#include  	<ctype.h>
#include  	<stdlib.h>
#include	<conio.h>

#include	<windows.h>
#include	<mmsystem.h>

#include 	<raw_api.h>

#include 	<rf/rf_config.h>
#include 	<rf/active_object.h>
#include 	<rf/active_time_event.h>
#include 	<rf/active_memory.h>
#include 	<rf/active_queue_broadcast.h>


#pragma comment( lib, "Winmm.lib" )



RAW_U16 raw_os_start();



#define  TEST_TASK_STK_SIZE 10240


PORT_STACK 		test_rf1_stack[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 	test_rf1_obj;

PORT_STACK 		test_active_stack[TEST_TASK_STK_SIZE];




void start_vc_timer(int tick_ms);


extern void  vc_port_printf(char*   f,   ...);




/*..........................................................................*/
enum SensorSignals {
    TIMEOUT_SIG = STM_USER_SIG,                /* the periodic timeout signal */
    DATA_READY_SIG,                         /* the invented reminder signal */
    TERMINATE_SIG                              /* terminate the application */
};
/*..........................................................................*/
typedef struct SensorTag {                      /* the Sensor active object */
    ACTIVE_OBJECT_STRUCT super;                                   /* derive from QActive */

    TIME_EVENT_STRUCT timeEvt;                       /* private time event generator */

	RAW_U16 pollCtr;
    RAW_U16 procCtr;
   
} Sensor;


void Sensor_ctor(Sensor *me);
                                          /* hierarchical state machine ... */
static RAW_U16 Sensor_initial   (Sensor *me, STATE_EVENT *e);
static RAW_U16 Sensor_polling   (Sensor *me, STATE_EVENT *e);
static RAW_U16 Sensor_processing(Sensor *me, STATE_EVENT *e);
static RAW_U16 Sensor_idle      (Sensor *me, STATE_EVENT *e);
static RAW_U16 Sensor_busy      (Sensor *me, STATE_EVENT *e);
static RAW_U16 Sensor_final     (Sensor *me, STATE_EVENT *e);


void Sensor_ctor(Sensor *me) 
{
	HSM_CONSTRUCTOR(&me->super.father, Sensor_initial);
	active_time_event_create(&me->timeEvt, TIMEOUT_SIG); 
}


RAW_U16 Sensor_initial(Sensor *me, STATE_EVENT *e) 
{
    (void)e;               /* avoid compiler warning about unused parameter */

    me->pollCtr = 0;
    me->procCtr = 0;

    return STM_TRAN(&Sensor_polling);
}


/*..........................................................................*/
RAW_U16 Sensor_final(Sensor *me, STATE_EVENT *e) 
{
    (void)me;              /* avoid compiler warning about unused parameter */
    (void)e;               /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case STM_ENTRY_SIG: {
            vc_port_printf("final-ENTRY;\nBye!Bye!\n");
           
            return STM_RET_HANDLED;
        }
    }
    return STM_FATHER(hsm_top);
}



RAW_U16 Sensor_polling(Sensor *me, STATE_EVENT *e)
{
    switch (e->sig) {
		
        case STM_ENTRY_SIG: {

			active_time_event_fire(&me->timeEvt, (ACTIVE_OBJECT_STRUCT *)me, RAW_TICKS_PER_SECOND / 2, 0);
            return STM_RET_HANDLED;
        }
        case STM_EXIT_SIG: {
            active_time_event_deactivate(&me->timeEvt);
            return STM_RET_HANDLED;
        }
        case STM_INIT_SIG: {
            return STM_TRAN(&Sensor_processing);
        }
        case TIMEOUT_SIG: {
            /* NOTE: this constant event is statically pre-allocated.
            * It can be posted/published as any other event.
            */
            static STATE_EVENT reminderEvt = { DATA_READY_SIG, 0, 0 };

            ++me->pollCtr;
            vc_port_printf("polling %3d\n", me->pollCtr);
            if ((me->pollCtr & 0x3) == 0) {                     /* modulo 4 */
                active_event_post_end((ACTIVE_OBJECT_STRUCT *)me, &reminderEvt);
            }
            return STM_RET_HANDLED;
        }
        case TERMINATE_SIG: {
            return STM_TRAN(Sensor_final);
        }
    }
	
     return STM_FATHER(hsm_top);
	 
}



RAW_U16 Sensor_processing(Sensor *me, STATE_EVENT *e) 
{
    switch (e->sig) {
        case STM_INIT_SIG: {
            return STM_TRAN(Sensor_idle);
        }
    }
    return STM_FATHER(Sensor_polling);
}


RAW_U16 Sensor_idle(Sensor *me, STATE_EVENT *e) 
{
    switch (e->sig) {
        case STM_ENTRY_SIG: {
            vc_port_printf("idle-ENTRY;\n");
            return STM_RET_HANDLED;
        }
        case DATA_READY_SIG: {
			 vc_port_printf("DATA_READY_SIG\n");
            return STM_TRAN(&Sensor_busy);
        }
    }
    return STM_FATHER(&Sensor_processing);
}


RAW_U16 Sensor_busy(Sensor *me, STATE_EVENT *e) 
{
    switch (e->sig) {
        case STM_ENTRY_SIG: {
            vc_port_printf("busy-ENTRY;\n");
            return STM_RET_HANDLED;
        }
        case TIMEOUT_SIG: {
            ++me->procCtr;
            vc_port_printf("processing %3d\n", me->procCtr);
            if ((me->procCtr & 0x1) == 0) {                     /* modulo 2 */
                return STM_TRAN(Sensor_idle);
            }
            return STM_RET_HANDLED;
        }
    }
    return STM_FATHER(Sensor_processing);
}

/* test harness ============================================================*/

/* Local-scope objects -----------------------------------------------------*/
static Sensor l_sensor;                         /* the Sensor active object */
static STATE_EVENT *l_sensorQSto[10];    /* Event queue storage for Sensor */

unsigned int test_qp1_count;

void test_rf1(void * pParam) 
{	
	                   
    Sensor_ctor(&l_sensor);
    
	active_object_create((ACTIVE_OBJECT_STRUCT *)&l_sensor, 20,
                   (RAW_VOID **)l_sensorQSto, 10,
                   test_active_stack, TEST_TASK_STK_SIZE, 0);
                   
	
	while(1) {
		
		test_qp1_count++;
		raw_sleep(RAW_TICKS_PER_SECOND);	
		
	}
	
}



extern RAW_U8 test_started_raw;

void rf1_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	raw_task_create(&test_rf1_obj, (RAW_U8  *)"task1", 0,
	                         10, 0,  test_rf1_stack, 
	                         TEST_TASK_STK_SIZE , test_rf1, 1); 
	

}



