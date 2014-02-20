/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2012        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/
#include "raw_api.h"
#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"
//#include "usbdisk.h"	/* Example: USB drive control */
#include <string.h>
#include <stdio.h>


static RAW_U8 *simulated_space;
static int init_flag;
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{

	if (init_flag == 0) {
		simulated_space = malloc(50 * 1024 *1024);
		raw_memset(simulated_space, 0, 50 * 1024 *1024);
		
		if (simulated_space == 0) {

			RAW_ASSERT(0);
			
		}

		init_flag = 1;
	}
	
	return 0;	
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{

	return 0;	
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)

{
	
	raw_memcpy(buff, simulated_space + (512 * sector), 512 * count);
	
	
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{

	raw_memcpy(simulated_space + (512 * sector), buff, 512 * count);
	
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;
	
		switch (ctrl) {
		case CTRL_SYNC:
			res = RES_OK;
			break;
	
		case GET_SECTOR_COUNT:
			*(DWORD*)buff = (102400); /*50M space*/
			res = RES_OK;
			break;
	
		case GET_SECTOR_SIZE:
			*(WORD*)buff = 512;
			res = RES_OK;
			break;
	
		default:
			break;
	
		}
	
		return res;
}

DWORD get_fattime (void)
{

 	return 0;
}


