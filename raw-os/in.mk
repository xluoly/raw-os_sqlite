INC += $(ROOT)/raw-os/include
INC += $(ROOT)/raw-os/extension/include

SRC += raw-os/extension/rf/active_memory.c
SRC += raw-os/extension/rf/active_object.c
SRC += raw-os/extension/rf/active_queue_broadcast.c
SRC += raw-os/extension/rf/active_time_event.c
SRC += raw-os/extension/protothread/clock_port.c
SRC += raw-os/extension/protothread/etimer.c
SRC += raw-os/extension/lib/fifo.c
SRC += raw-os/extension/lib/fifo_lock.c
SRC += raw-os/extension/mm/heap_1.c
SRC += raw-os/extension/mm/heap_2.c
SRC += raw-os/extension/mm/heap_3.c
SRC += raw-os/extension/mm/heap_4.c
SRC += raw-os/extension/posix/mqueue.c
SRC += raw-os/extension/protothread/process.c
SRC += raw-os/extension/posix/pthread.c
SRC += raw-os/extension/posix/pthread_cond.c
SRC += raw-os/extension/posix/pthread_mutex.c
SRC += raw-os/raw_block.c
SRC += raw-os/extension/mm/slab/raw_buddy.c
SRC += raw-os/raw_byte.c
SRC += raw-os/extension/driver_interface/raw_driver_interface.c
SRC += raw-os/raw_event.c
SRC += raw-os/extension/mm/slab/raw_frame.c
SRC += raw-os/raw_idle.c
SRC += raw-os/raw_idle_event.c
SRC += raw-os/extension/mm/raw_malloc.c
SRC += raw-os/extension/lib/raw_mlock.c
SRC += raw-os/raw_mqueue.c
SRC += raw-os/raw_mutex.c
SRC += raw-os/raw_obj.c
SRC += raw-os/extension/mm/raw_page.c
SRC += raw-os/raw_pend.c
SRC += raw-os/raw_queue.c
SRC += raw-os/raw_queue_buffer.c
SRC += raw-os/raw_queue_size.c
SRC += raw-os/raw_sched.c
SRC += raw-os/raw_sem.c
SRC += raw-os/extension/mm/slab/raw_slab.c
SRC += raw-os/raw_stat.c
SRC += raw-os/raw_stm.c
SRC += raw-os/raw_system.c
SRC += raw-os/raw_task.c
SRC += raw-os/raw_task_0.c
SRC += raw-os/raw_task_queue_size.c
SRC += raw-os/raw_task_sem.c
SRC += raw-os/raw_tick.c
SRC += raw-os/raw_timer.c
SRC += raw-os/extension/mm/raw_tlsf.c
SRC += raw-os/extension/lib/raw_work_queue.c
SRC += raw-os/extension/lib/rb_tree.c
SRC += raw-os/extension/shell/rsh.c
SRC += raw-os/extension/posix/semphore.c
SRC += raw-os/extension/lib/simple_printf.c
SRC += raw-os/extension/protothread/stimer.c
SRC += raw-os/extension/lib/string.c
SRC += raw-os/extension/posix/time.c
SRC += raw-os/extension/protothread/timer.c

