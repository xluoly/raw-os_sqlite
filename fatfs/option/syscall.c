
#include <stdlib.h>		/* ANSI memory controls */

#include "../ff.h"

#include <raw_api.h>
#include <mm/heap_4_config.h>

#if _FS_REENTRANT

//Create a Synchronization Object


/* This function is called in f_mount function to create a new
/  synchronization object, such as semaphore and mutex. When a zero is
/  returned, the f_mount function fails with FR_INT_ERR.
*/

int ff_cre_syncobj (	/* TRUE:Function succeeded, FALSE:Could not create due to any error */
	BYTE vol,			/* Corresponding logical drive being processed */
	_SYNC_t *sobj		/* Pointer to return the created sync object */
)
{

	RAW_U16  ret = 0;

	*sobj = mem_4_malloc(sizeof(RAW_MUTEX));

	ret = raw_mutex_create(*sobj, (RAW_U8 *)"fatfs_lock", RAW_MUTEX_INHERIT_POLICY, 0);

	if (ret == RAW_SUCCESS) {

		ret = 1;
	}
	
	return ret;
}



/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to delete a synchronization
/  object that created with ff_cre_syncobj function. When a zero is
/  returned, the f_mount function fails with FR_INT_ERR.
*/

int ff_del_syncobj (	/* TRUE:Function succeeded, FALSE:Could not delete due to any error */
	_SYNC_t sobj		/* Sync object tied to the logical drive to be deleted */
)
{

	RAW_U16  ret = 0;


	ret = raw_mutex_delete(sobj);

	mem_4_free(sobj);

	if (ret == RAW_SUCCESS) {

		ret = 1;
	}
	
	return ret;
}



/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a zero is returned, the file function fails with FR_TIMEOUT.
*/

int ff_req_grant (	/* TRUE:Got a grant to access the volume, FALSE:Could not get a grant */
	_SYNC_t sobj	/* Sync object to wait */
)
{

	RAW_U16  ret = 0;


	ret = raw_mutex_get(sobj, RAW_WAIT_FOREVER);

	if (ret == RAW_SUCCESS) {

		ret = 1;
	}
	
	return ret;
}



/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant (
	_SYNC_t sobj	/* Sync object to be signaled */
)
{
	RAW_U16 ret;
	
	ret = raw_mutex_put(sobj);

	if (ret != RAW_SUCCESS) {

		RAW_ASSERT(0);
	}
}

#endif




#if _USE_LFN == 3	/* LFN with a working buffer on the heap */
/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/
/* If a NULL is returned, the file function fails with FR_NOT_ENOUGH_CORE.
*/

void* ff_memalloc (	/* Returns pointer to the allocated memory block */
	UINT size		/* Number of bytes to allocate */
)
{
	return mem_4_malloc(size);
}


/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/

void ff_memfree(
	void* mblock	/* Pointer to the memory block to free */
)
{
	mem_4_free(mblock);
}

#endif
