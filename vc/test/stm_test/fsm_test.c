#include  	<stdio.h>
#include  	<string.h>
#include  	<ctype.h>
#include  	<stdlib.h>
#include	<conio.h>

#include	<windows.h>
#include	<mmsystem.h>

#include 	<raw_api.h>


#pragma 	comment( lib, "Winmm.lib" )


enum BombSignals {                          /* all signals for the Bomb FSM */
    UP_SIG = STM_USER_SIG,
    DOWN_SIG,
    ARM_SIG,
    TICK_SIG
};

typedef struct TickEvtTag {
    STATE_EVENT 	super;                       /* derive from the QEvt structure */
    RAW_U8 	fine_time;                           /* the fine 1/10 s counter */
	
} TickEvt;

typedef struct Bomb4Tag {
	
    STM_STRUCT 		super;                                         /* derive from QFsm */
    RAW_U8 			timeout;                    /* number of seconds till explosion */
    RAW_U8 			code;              /* currently entered code to disarm the bomb */
    RAW_U8 			defuse;                /* secret defuse code to disarm the bomb */
	
} Bomb4;

void Bomb4_ctor(Bomb4 *me, RAW_U8 defuse);

static RAW_U16 Bomb4_initial(Bomb4 *me, 	 	STATE_EVENT  *e);
static RAW_U16 Bomb4_setting(Bomb4 *me, 	STATE_EVENT  *e);
static RAW_U16 Bomb4_timing (Bomb4 *me, 	STATE_EVENT  *e);


static Bomb4 l_bomb;                                       /* time bomb FSM */

/*--------------------------------------------------------------------------*/
                                        /* the initial value of the timeout */
#define INIT_TIMEOUT   10

/*..........................................................................*/
static void Bomb4_ctor(Bomb4 *me, RAW_U8 defuse) {

	FSM_CONSTRUCTOR(&me->super, (stm_state_handler)&Bomb4_initial);/* superclass ctor */
    me->defuse = defuse;    /* the defuse code is assigned at instantiation */
}
/*..........................................................................*/
static RAW_U16 Bomb4_initial(Bomb4 *me, STATE_EVENT  *e) {
    (void)e;
    me->timeout = INIT_TIMEOUT;
    return STM_TRAN(&Bomb4_setting);
}
/*..........................................................................*/
static RAW_U16 Bomb4_setting(Bomb4 *me, STATE_EVENT  *e) {
    switch (e->sig) {

		case STM_ENTRY_SIG: {
            me->code = 0;                          /* clear the defuse code */
            return STM_RET_HANDLED;
        }
		
        case UP_SIG: {
            if (me->timeout < 60) {
                ++me->timeout;
                vc_port_printf("me->timeout is %d\n", me->timeout);
            }
            return STM_RET_HANDLED;
        }
        case DOWN_SIG: {
            if (me->timeout > 1) {
                --me->timeout;
				vc_port_printf("me->timeout is %d\n", me->timeout);
            }
            return STM_RET_HANDLED;
        }
        case ARM_SIG: {
            return STM_TRAN(&Bomb4_timing);         /* transition to "timing" */
        }

		case STM_EXIT_SIG:
			vc_port_printf("setting state exit\n");
			return STM_RET_HANDLED;
			
    }
    return STM_RET_IGNORED;
}
/*..........................................................................*/
static RAW_U16 Bomb4_timing(Bomb4 *me, STATE_EVENT *e) {
    switch (e->sig) {
        case STM_ENTRY_SIG: {
			vc_port_printf("timing state enter\n");
            me->code = 0;                          /* clear the defuse code */
            return STM_RET_HANDLED;
        }
        case UP_SIG: {
            me->code <<= 1;
            me->code |= 1;
            return STM_RET_HANDLED;
        }
        case DOWN_SIG: {
            me->code <<= 1;
            return STM_RET_HANDLED;
        }
        case ARM_SIG: {
            if (me->code == me->defuse) {
                return STM_TRAN(&Bomb4_setting);
            }
            return STM_RET_HANDLED;
        }
        case TICK_SIG: {
            if (((TickEvt const *)e)->fine_time == 0) {
	            --me->timeout;
	           vc_port_printf("me->timeout is %d\n", me->timeout);
			   
                if (me->timeout == 0) {
					vc_port_printf("boom aaaaaa^^^^^^%%%%%$$$$$$\n");
					exit(0);
                }
            }
            return STM_RET_HANDLED;
        }
    }
    return STM_RET_IGNORED;
}


#define  TEST_TASK_STK_SIZE (10 * 1024)

PORT_STACK fsm_test_stack[TEST_TASK_STK_SIZE];



RAW_TASK_OBJ  fsm_test_obj;


void fsm_test_task(void * pParam)
{
	 vc_port_printf("Time Bomb (raw os fsm case)\n"
           "Press 'u'   for UP   event\n"
           "Press 'd'   for DOWN event\n"
           "Press 'a'   for ARM  event\n"
           "Press <Esc> to quit.\n");


	Bomb4_ctor(&l_bomb, 0xd);       /* the secret defuse code, 1101 binary */
	fsm_init((STM_STRUCT *)&l_bomb, 0); /* take the initial transition */
	
	while (1){

		static TickEvt tick_evt = { TICK_SIG, 0, 0, 0 };

        raw_sleep(RAW_TICKS_PER_SECOND / 20);                                         /* 50 ms delay */

        if (++tick_evt.fine_time == 10) {
            tick_evt.fine_time = 0;
        }
		

        fsm_exceute((STM_STRUCT *)&l_bomb, (STATE_EVENT *)&tick_evt);

        if (_kbhit()) {
            static STATE_EVENT  up_evt   = { UP_SIG,   0, 0 };
            static STATE_EVENT  down_evt = { DOWN_SIG, 0, 0 };
            static STATE_EVENT  arm_evt  = { ARM_SIG,  0, 0 };
            STATE_EVENT  *e = 0;

            switch (_getch()) {
                case 'u': {                                     /* UP event */
                    vc_port_printf("\nUP  : ");
                    e = &up_evt;                   /* generate the UP event */
                    break;
                }
                case 'd': {                                   /* DOWN event */
                    vc_port_printf("\nDOWN: ");
                    e = &down_evt;               /* generate the DOWN event */
                    break;
                }
                case 'a': {                                    /* ARM event */
                    vc_port_printf("\nARM : ");
                    e = &arm_evt;                 /* generate the ARM event */
                    break;
                }
                case '\33': {                                  /* <Esc> key */
                    vc_port_printf("\nESC : Bye! Bye!");
                    exit(0);
                    break;
                }
            }
            if (e != 0) {            /* keyboard event available? */
                fsm_exceute((STM_STRUCT *)&l_bomb, e);    /* dispatch the event */
            }
        }



	}

}




extern RAW_U8 test_started_raw;

void fsm_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	
	raw_task_create(&fsm_test_obj, (RAW_U8  *)"task1", 0,
	                         1, 0,  fsm_test_stack, 
	                         TEST_TASK_STK_SIZE ,  fsm_test_task, 1); 



}




