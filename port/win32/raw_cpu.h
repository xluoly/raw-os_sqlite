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


#ifndef RAW_CPU_H
#define RAW_CPU_H

#define  NEED_STATUS

typedef  unsigned int        CPU_SR;                    

                                                              
#ifdef   NEED_STATUS
#define  RAW_SR_ALLOC()           CPU_SR  cpu_sr = (CPU_SR)0
#else
#define  RAW_SR_ALLOC() 
#endif

extern void port_enter_critical(void );
extern void port_exit_critical(void);
													 											
#define  USER_CPU_INT_DISABLE()        port_enter_critical()
#define  USER_CPU_INT_ENABLE()         port_exit_critical()

#endif

