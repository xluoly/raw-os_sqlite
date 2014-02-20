#include <raw_api.h>

#include <rf/rf_config.h>
#include <rf/active_object.h>
#include <rf/active_time_event.h>
#include <rf/active_memory.h>
#include <rf/active_queue_broadcast.h>
#include "dpp.h"


/* Active object class -----------------------------------------------------*/
typedef struct PhiloTag {
	ACTIVE_OBJECT_STRUCT super;
	TIME_EVENT_STRUCT timeEvt;                   /* for timing out thining or eating */
} Philo;

static RAW_U16 Philo_initial(Philo *me, STATE_EVENT *e);
static RAW_U16 Philo_thinking(Philo *me, STATE_EVENT *e);
static RAW_U16 Philo_hungry(Philo *me, STATE_EVENT *e);
static RAW_U16 Philo_eating(Philo *me, STATE_EVENT *e);

/* Local objects -----------------------------------------------------------*/
static Philo l_philo[N_PHILO];                    /* storage for all Philos */

#define THINK_TIME  (RAW_TICKS_PER_SECOND / 2)
#define EAT_TIME    (RAW_TICKS_PER_SECOND / 2)

                           /* helper macro to provide the ID of Philo "me_" */
#define PHILO_ID(me_)    ((RAW_U8)((me_) - l_philo))

enum InternalSignals {                                  /* internal signals */
    TIMEOUT_SIG = MAX_SIG
};

/* Global objects ----------------------------------------------------------*/
ACTIVE_OBJECT_STRUCT *AO_Philo[N_PHILO] = {     /* "opaque" pointers to Philo AO */
    (ACTIVE_OBJECT_STRUCT *)&l_philo[0],
    (ACTIVE_OBJECT_STRUCT *)&l_philo[1],
    (ACTIVE_OBJECT_STRUCT *)&l_philo[2],
    (ACTIVE_OBJECT_STRUCT *)&l_philo[3],
    (ACTIVE_OBJECT_STRUCT *)&l_philo[4]
};

/*..........................................................................*/
void Philo_ctor(void) {                    /* instantiate all Philo objects */
    RAW_U8 n;
    Philo *me;
    for (n = 0; n < N_PHILO; ++n) {
        me = &l_philo[n];

		HSM_CONSTRUCTOR(&me->super.father, Philo_initial);
		active_time_event_create(&me->timeEvt, TIMEOUT_SIG); 
    }
}
/*..........................................................................*/
RAW_U16 Philo_initial(Philo *me, STATE_EVENT *e) 
{
    static RAW_U8 registered;         /* starts off with 0, per C-standard */
    (void)e;        /* suppress the compiler warning about unused parameter */

	active_broadcast_queue_register((ACTIVE_OBJECT_STRUCT  *)me, EAT_SIG);


    return STM_TRAN(Philo_thinking);          /* top-most initial transition */
}


/*..........................................................................*/
RAW_U16 Philo_thinking(Philo *me, STATE_EVENT *e) 
{
    switch (e->sig) {
        case STM_ENTRY_SIG: {
			active_time_event_fire(&me->timeEvt, (ACTIVE_OBJECT_STRUCT *)me, THINK_TIME, 1);
            return STM_RET_HANDLED;
        }
        case TIMEOUT_SIG: {
            return STM_TRAN(Philo_hungry);
        }
        case EAT_SIG:                         /* intentionally fall-through */
        case DONE_SIG: {
                      /* EAT or DONE must be for other Philos than this one */
            
			if (((TableEvt  *)e)->philoNum == PHILO_ID(me)) {

				RAW_ASSERT(0);
			}
				
            return STM_RET_HANDLED;
        }
    }
    return STM_FATHER(hsm_top);
}

extern MEM_POOL ph_pool_test;

RAW_U16 Philo_hungry(Philo *me, STATE_EVENT *e)
{
    switch (e->sig) {
		
        case STM_ENTRY_SIG: {
           
			TableEvt *pe = active_event_memory_allocate(&ph_pool_test, HUNGRY_SIG);
            pe->philoNum = PHILO_ID(me);
            
			active_event_post_end(AO_Table, (STATE_EVENT *)pe);
            return STM_RET_HANDLED;
        }
        case EAT_SIG: {
            if (((TableEvt const *)e)->philoNum == PHILO_ID(me)) {
             
                return STM_TRAN(&Philo_eating);
            }
            break;
        }
        case DONE_SIG: {
                             /* DONE must be for other Philos than this one */
           if (((TableEvt  *)e)->philoNum == PHILO_ID(me)) {

				RAW_ASSERT(0);
			}
		   
            return STM_RET_HANDLED;
        }
    }
	
    return STM_FATHER(hsm_top);
	
}


RAW_U16 Philo_eating(Philo *me, STATE_EVENT *e) 
{
    switch (e->sig) {
		
        case STM_ENTRY_SIG: {
			active_time_event_fire(&me->timeEvt, (ACTIVE_OBJECT_STRUCT *)me, EAT_TIME, 1);
            return STM_RET_HANDLED;
        }
		
        case STM_EXIT_SIG: {
			
			TableEvt *pe = active_event_memory_allocate(&ph_pool_test, DONE_SIG);
            pe->philoNum = PHILO_ID(me);
			active_queue_event_broadcast((STATE_EVENT *)pe);
            return STM_RET_HANDLED;
        }
		
        case TIMEOUT_SIG: {
           
            return STM_TRAN(&Philo_thinking);
        }
		
        case EAT_SIG:                         /* intentionally fall-through */
        case DONE_SIG: {
                      /* EAT or DONE must be for other Philos than this one */
            if (((TableEvt  *)e)->philoNum == PHILO_ID(me)) {

				RAW_ASSERT(0);
			}
			
            return STM_RET_HANDLED;
        }
    }

	
    return STM_FATHER(hsm_top);
	
}






