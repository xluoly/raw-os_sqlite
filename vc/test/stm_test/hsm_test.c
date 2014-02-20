#include  	<stdio.h>
#include  	<string.h>
#include  	<ctype.h>
#include  	<stdlib.h>
#include	<conio.h>

#include	<windows.h>
#include	<mmsystem.h>

#include 	<raw_api.h>


#pragma 	comment( lib, "Winmm.lib" )


enum QHsmTstSignals {
    A_SIG = STM_USER_SIG,
    B_SIG,
    C_SIG,
    D_SIG,
    E_SIG,
    F_SIG,
    G_SIG,
    H_SIG,
    I_SIG,
    J_SIG,
    K_SIG,
    TERMINATE_SIG,
    IGNORE_SIG,
    MAX_SIG
};



#define  TEST_TASK_STK_SIZE (10 * 1024)

PORT_STACK hsm_test_stack[TEST_TASK_STK_SIZE];



RAW_TASK_OBJ  hsm_test_obj;



typedef struct QHsmTst {

	STM_STRUCT super;

	RAW_U8 foo;
	
} QHsmTst;


static RAW_U16 QHsmTst_initial(QHsmTst *me, STATE_EVENT *e);
static RAW_U16 QHsmTst_s(QHsmTst *me, STATE_EVENT *e);
static RAW_U16 QHsmTst_s1(QHsmTst *me, STATE_EVENT *e);
static RAW_U16 QHsmTst_s11(QHsmTst *me, STATE_EVENT *e);
static RAW_U16 QHsmTst_s2(QHsmTst *me, STATE_EVENT *e);
static RAW_U16 QHsmTst_s21(QHsmTst *me, STATE_EVENT *e);
static RAW_U16 QHsmTst_s211(QHsmTst *me, STATE_EVENT *e);


static QHsmTst l_hsmtst; /* the only instance of the QHsmTst class */

/* global-scope definitions ---------------------------------------*/
STM_STRUCT *the_hsm = (STM_STRUCT *)&l_hsmtst;    /* the opaque pointer */



static RAW_U16 QHsmTst_s(QHsmTst *me, STATE_EVENT *e) {
    RAW_U16 status;
    switch (e->sig) {
        /* @(/1/0/1/1) */
        case STM_ENTRY_SIG: {
            vc_port_printf("s-ENTRY;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1) */
        case STM_EXIT_SIG: {
            vc_port_printf("s-EXIT;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/0) */
        case STM_INIT_SIG: {
            vc_port_printf("s-INIT;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/1) */
        case I_SIG: {
            /* @(/1/0/1/1/1/0) */
            if (me->foo) {
                me->foo = 0U;
                vc_port_printf("s-I;\n");
                status = STM_RET_HANDLED;
            }
            else {
                status = STM_RET_HANDLED;
            }
            break;
        }
        /* @(/1/0/1/1/2) */
        case E_SIG: {
            vc_port_printf("s-E;\n");
            status = STM_TRAN(&QHsmTst_s11);
            break;
        }
        /* @(/1/0/1/1/3) */
        case TERMINATE_SIG: {
            vc_port_printf("bye bye!\n");
			exit(0);
            status = STM_RET_HANDLED;
            break;
        }
        default: {
            status = STM_FATHER(&hsm_top);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/4) ...........................................................*/
static RAW_U16 QHsmTst_s1(QHsmTst *me, STATE_EVENT *e) {

    RAW_U16 status;
    switch (e->sig) {
        /* @(/1/0/1/1/4) */
        case STM_ENTRY_SIG: {
            vc_port_printf("s1-ENTRY;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/4) */
        case STM_EXIT_SIG: {
            vc_port_printf("s1-EXIT;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/4/0) */
        case STM_INIT_SIG: {
            vc_port_printf("s1-INIT;\n");
            status = STM_TRAN(&QHsmTst_s11);
            break;
        }
        /* @(/1/0/1/1/4/1) */
        case I_SIG: {
            vc_port_printf("s1-I;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/4/2) */
        case D_SIG: {
            /* @(/1/0/1/1/4/2/0) */
            if (!me->foo) {
                me->foo = 1U;
                vc_port_printf("s1-D;\n");
                status = STM_TRAN(&QHsmTst_s);
            }
            else {
                status = STM_RET_UNHANDLED;
            }
            break;
        }
        /* @(/1/0/1/1/4/3) */
        case A_SIG: {
            vc_port_printf("s1-A;\n");
            status = STM_TRAN(&QHsmTst_s1);
            break;
        }
        /* @(/1/0/1/1/4/4) */
        case B_SIG: {
            vc_port_printf("s1-B;\n");
            status = STM_TRAN(&QHsmTst_s11);
            break;
        }
        /* @(/1/0/1/1/4/5) */
        case F_SIG: {
            vc_port_printf("s1-F;\n");
            status = STM_TRAN(&QHsmTst_s211);
            break;
        }
        /* @(/1/0/1/1/4/6) */
        case C_SIG: {
            vc_port_printf("s1-C;\n");
            status = STM_TRAN(&QHsmTst_s2);
            break;
        }
        default: {
            status = STM_FATHER(&QHsmTst_s);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/4/7) .........................................................*/
static RAW_U16 QHsmTst_s11(QHsmTst *me, STATE_EVENT *e) {
   	RAW_U16 status;
    switch (e->sig) {
        /* @(/1/0/1/1/4/7) */
        case STM_ENTRY_SIG: {
            vc_port_printf("s11-ENTRY;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/4/7) */
        case STM_EXIT_SIG: {
            vc_port_printf("s11-EXIT;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/4/7/0) */
        case H_SIG: {
            vc_port_printf("s11-H;\n");
            status = STM_TRAN(&QHsmTst_s);
            break;
        }
        /* @(/1/0/1/1/4/7/1) */
        case D_SIG: {
            /* @(/1/0/1/1/4/7/1/0) */
            if (me->foo) {
                me->foo = 0U;
                vc_port_printf("s11-D;\n");
                status = STM_TRAN(&QHsmTst_s1);
            }
            else {
                status = STM_RET_UNHANDLED;
            }
            break;
        }
        /* @(/1/0/1/1/4/7/2) */
        case G_SIG: {
            vc_port_printf("s11-G;\n");
            status = STM_TRAN(&QHsmTst_s211);
            break;
        }

		 case STM_INIT_SIG: {
            vc_port_printf("s11-INIT;\n");
            status = STM_RET_HANDLED;
            break;
        }
		 
        default: {
            status = STM_FATHER(&QHsmTst_s1);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/5) ...........................................................*/
static RAW_U16 QHsmTst_s2(QHsmTst *me, STATE_EVENT *e) {
    RAW_U16 status;
    switch (e->sig) {
        /* @(/1/0/1/1/5) */
        case STM_ENTRY_SIG: {
            vc_port_printf("s2-ENTRY;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/5) */
        case STM_EXIT_SIG: {
            vc_port_printf("s2-EXIT;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/5/0) */
        case STM_INIT_SIG: {
            vc_port_printf("s2-INIT;\n");
            status = STM_TRAN(&QHsmTst_s21);
            break;
        }
        /* @(/1/0/1/1/5/1) */
        case I_SIG: {
            /* @(/1/0/1/1/5/1/0) */
            if (!me->foo) {
                me->foo = 1U;
                vc_port_printf("s2-I;\n");
                status = STM_RET_HANDLED;
            }
            else {
                status = STM_RET_UNHANDLED;
            }
            break;
        }
        /* @(/1/0/1/1/5/2) */
        case F_SIG: {
            vc_port_printf("s2-F;\n");
            status = STM_TRAN(&QHsmTst_s11);
            break;
        }
        /* @(/1/0/1/1/5/3) */
        case C_SIG: {
            vc_port_printf("s2-C;\n");
            status = STM_TRAN(&QHsmTst_s1);
            break;
        }
        default: {
            status = STM_FATHER(&QHsmTst_s);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/5/4) .........................................................*/
static RAW_U16 QHsmTst_s21(QHsmTst *me, STATE_EVENT *e) {
    RAW_U16 status;
    switch (e->sig) {
        /* @(/1/0/1/1/5/4) */
        case STM_ENTRY_SIG: {
            vc_port_printf("s21-ENTRY;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/5/4) */
        case STM_EXIT_SIG: {
            vc_port_printf("s21-EXIT;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/5/4/0) */
        case STM_INIT_SIG: {
            vc_port_printf("s21-INIT;\n");
             status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/5/4/1) */
        case G_SIG: {
            vc_port_printf("s21-G;\n");
            status = STM_TRAN(&QHsmTst_s1);
            break;
        }
        /* @(/1/0/1/1/5/4/2) */
        case A_SIG: {
            vc_port_printf("s21-A;\n");
            status = STM_TRAN(&QHsmTst_s11);
            break;
        }
        /* @(/1/0/1/1/5/4/3) */
        case B_SIG: {
            vc_port_printf("s21-B;\n");
            status = STM_TRAN(&QHsmTst_s211);
            break;
        }

		
		case J_SIG: {
		   vc_port_printf("s21-J;\n");
		   status = STM_TRAN(&QHsmTst_s211);
		   break;
		 }

		
        default: {
            status = STM_FATHER(&QHsmTst_s2);
            break;
        }
    }
    return status;
}
/* @(/1/0/1/1/5/4/4) .......................................................*/
static RAW_U16 QHsmTst_s211(QHsmTst *me, STATE_EVENT *e) {
    RAW_U16 status;
    switch (e->sig) {
        /* @(/1/0/1/1/5/4/4) */
        case STM_ENTRY_SIG: {
            vc_port_printf("s211-ENTRY;\n");
            status = STM_RET_HANDLED;
            break;
        }
        /* @(/1/0/1/1/5/4/4) */
        case STM_EXIT_SIG: {
            vc_port_printf("s211-EXIT;\n");
            status = STM_RET_HANDLED;
            break;
        }
        
        /* @(/1/0/1/1/5/4/4) */
        case STM_INIT_SIG: {
            vc_port_printf("s211-INIT;\n");
            status = STM_RET_HANDLED;
            break;
        }
        
        /* @(/1/0/1/1/5/4/4/0) */
        case H_SIG: {
            vc_port_printf("s211-H;\n");
            status = STM_TRAN(&QHsmTst_s);
            break;
        }
        /* @(/1/0/1/1/5/4/4/1) */
        case D_SIG: {
            vc_port_printf("s211-D;\n");
            status = STM_TRAN(&QHsmTst_s21);
            break;
        }

		 case K_SIG: {
            vc_port_printf("s211-K;\n");
            status = STM_TRAN(&QHsmTst_s211);
            break;
        }
        default: {
            status = STM_FATHER(&QHsmTst_s21);
            break;
        }
    }
    return status;
}


static void dispatch(STM_SIGNAL sig)
{
    STATE_EVENT e;
    e.sig = sig;
  
    hsm_exceute(the_hsm, &e);                       /* dispatch the event */
}

static RAW_U16 QHsmTst_initial(QHsmTst *me, STATE_EVENT *e) {
    (void)e; /* avoid compiler warning */
    me->foo = 0U;
    vc_port_printf("top-INIT;\n");
    return STM_TRAN(&QHsmTst_s2);
}




void hsm_test_task(void * pParam)
{

	QHsmTst *me = &l_hsmtst;
    HSM_CONSTRUCTOR(&me->super, STM_STATE_CAST(&QHsmTst_initial));
	
	hsm_init(the_hsm, 0);      /* the top-most initial tran. */

	                       /* testing of dynamic transitions... */
	dispatch(J_SIG);
	dispatch(K_SIG);
	dispatch(A_SIG);
	dispatch(B_SIG);
	dispatch(D_SIG);
	dispatch(E_SIG);
	dispatch(I_SIG);
	dispatch(F_SIG);
	dispatch(I_SIG);
	dispatch(I_SIG);
	dispatch(F_SIG);
	dispatch(A_SIG);
	dispatch(B_SIG);
	dispatch(D_SIG);
	dispatch(D_SIG);
	dispatch(E_SIG);
	dispatch(G_SIG);
	dispatch(H_SIG);
	dispatch(H_SIG);
	dispatch(C_SIG);
	dispatch(G_SIG);
	dispatch(C_SIG);
	dispatch(C_SIG);


	while (1);

}


extern RAW_U8 test_started_raw;

void hsm_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	
	raw_task_create(&hsm_test_obj, (RAW_U8  *)"task1", 0,
	                         1, 0,  hsm_test_stack, 
	                         TEST_TASK_STK_SIZE ,  hsm_test_task, 1); 



}

