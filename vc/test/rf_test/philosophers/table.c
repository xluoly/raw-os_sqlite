#include <raw_api.h>

#include <rf/rf_config.h>
#include <rf/active_object.h>
#include <rf/active_time_event.h>
#include <rf/active_memory.h>
#include <rf/active_queue_broadcast.h>
#include "dpp.h"



/* Active object class -----------------------------------------------------*/
typedef struct TableTag {
    ACTIVE_OBJECT_STRUCT super;
    RAW_U8 fork[N_PHILO];
    RAW_U8 isHungry[N_PHILO];
} Table;

static RAW_U16 Table_initial(Table *me, STATE_EVENT *e);
static RAW_U16 Table_active(Table * const me, STATE_EVENT *e);

static RAW_U16 Table_serving(Table *me, STATE_EVENT *e);
extern MEM_POOL ph_pool_test;

#define RIGHT(n_) ((RAW_U8)(((n_) + (N_PHILO - 1)) % N_PHILO))
#define LEFT(n_)  ((RAW_U8)(((n_) + 1) % N_PHILO))
enum ForkState { FREE, USED };

/* Local objects -----------------------------------------------------------*/
static Table l_table;     /* the single instance of the Table active object */

/* Global-scope objects ----------------------------------------------------*/
ACTIVE_OBJECT_STRUCT *AO_Table = (ACTIVE_OBJECT_STRUCT *)&l_table;      /* "opaque" AO pointer */

/*..........................................................................*/
void Table_ctor(void) 
{
    RAW_U8 n;
    Table *me = &l_table;

	HSM_CONSTRUCTOR(&me->super.father, Table_initial);

    for (n = 0; n < N_PHILO; ++n) {
        me->fork[n] = FREE;
        me->isHungry[n] = 0;
    }
}
/*..........................................................................*/
RAW_U16 Table_initial(Table *me, STATE_EVENT *e) 
{
    (void)e;        /* suppress the compiler warning about unused parameter */


	active_broadcast_queue_register((ACTIVE_OBJECT_STRUCT  *)me, DONE_SIG);
	active_broadcast_queue_register((ACTIVE_OBJECT_STRUCT  *)me, TERMINATE_SIG);

    return STM_TRAN(&Table_serving);
}
static int cccc;

/*..........................................................................*/
RAW_U16 Table_serving(Table *me, STATE_EVENT *e) 
{
    RAW_U8 n, m;
    TableEvt *pe;

    switch (e->sig) {

		 case STM_ENTRY_SIG: {
            RAW_U8 n;
            for (n = 0U; n < N_PHILO; ++n) { /* give permissions to eat... */
                if ((me->isHungry[n] != 0U)
                    && (me->fork[LEFT(n)] == FREE)
                    && (me->fork[n] == FREE))
                {
                    TableEvt *te;

                    me->fork[LEFT(n)] = USED;
                    me->fork[n] = USED;
        
					te = active_event_memory_allocate(&ph_pool_test, EAT_SIG);
                    te->philoNum = n;
                  
					active_queue_event_broadcast((STATE_EVENT *)te);
                    me->isHungry[n] = 0U;
                    vc_port_printf("Pilo %d is eating\n", n);
                }
            }
            return STM_RET_HANDLED;
            break;
        }

		 
        case HUNGRY_SIG: {
           
            n = ((TableEvt const *)e)->philoNum;
            /* phil ID must be in range and he must be not hungry */

			RAW_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

           
			vc_port_printf("Pilo %d is hungry\n", n);
            m = LEFT(n);
            if ((me->fork[m] == FREE) && (me->fork[n] == FREE)) {
                me->fork[m] = me->fork[n] = USED;
				pe = active_event_memory_allocate(&ph_pool_test, EAT_SIG);
				
                pe->philoNum = n;


				active_queue_event_broadcast((STATE_EVENT *)pe);
              
				vc_port_printf("Pilo %d is eating\n", n);
            }
            else {
				
                me->isHungry[n] = 1;
            }
			
            return STM_RET_HANDLED;
        }
        case DONE_SIG: {

		  
            n = ((TableEvt const *)e)->philoNum;
                      /* phil ID must be in range and he must be not hungry */
            RAW_ASSERT((n < N_PHILO) && (!me->isHungry[n]));
					  
			vc_port_printf("Pilo %d is thinking\n", n);
            m = LEFT(n);
                                      /* both forks of Phil[n] must be used */
            RAW_ASSERT((me->fork[n] == USED) && (me->fork[m] == USED));

            me->fork[m] = me->fork[n] = FREE;
            m = RIGHT(n);                       /* check the right neighbor */
            if (me->isHungry[m] && (me->fork[m] == FREE)) {
                me->fork[n] = me->fork[m] = USED;
                me->isHungry[m] = 0;
				cccc++;
				
				pe = active_event_memory_allocate(&ph_pool_test, EAT_SIG);
				
                pe->philoNum = m;
				active_queue_event_broadcast((STATE_EVENT *)pe);
               
				vc_port_printf("Pilo %d is eating\n", n);
            }
            m = LEFT(n);                         /* check the left neighbor */
            n = LEFT(m);                  /* left fork of the left neighbor */
            if (me->isHungry[m] && (me->fork[n] == FREE)) {
                me->fork[m] = me->fork[n] = USED;
                me->isHungry[m] = 0;
				pe = active_event_memory_allocate(&ph_pool_test, EAT_SIG);
                pe->philoNum = m;
                active_queue_event_broadcast((STATE_EVENT *)pe);
                
				vc_port_printf("Pilo %d is eating\n", n);
            }
			
            return STM_RET_HANDLED;
        }


		 case EAT_SIG: {
            assert(0);
     		return STM_RET_HANDLED;
            break;
        }
        case TERMINATE_SIG: {
           
            return STM_RET_HANDLED;
        }
    }
	
     return STM_FATHER(hsm_top);
	 
}
