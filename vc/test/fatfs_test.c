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


/* 	2014-2  Created by jorya_txj
  *	xxxxxx   please added here
  */






#include <raw_api.h>
#include <ff.h>
#include "sqlite3.h"




#define  TEST_TASK_STK_SIZE 2048


static RAW_TASK_OBJ  fat_task_obj;
static PORT_STACK    test_task_fat[TEST_TASK_STK_SIZE];



FATFS fss;
static RAW_U32 buffer[10];
static RAW_U32 buffer2[10];

static FIL file;


static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;

	for(i=0; i<argc; i++){
		
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	
	return 0;
}


/*
** Execute an SQL statement.
*/
void db_execute(sqlite3 *db, const char *zFile, const char *zFormat, ...){
  char *zSql;
  int rc;
  char *zErrMsg = 0;
  va_list ap;
  va_start(ap, zFormat);
  zSql = sqlite3_vmprintf(zFormat, ap);
  va_end(ap);
  
  printf("EXEC %s: %s\n", zFile, zSql);
  
  do{
    rc = sqlite3_exec(db, zSql, 0, 0, &zErrMsg);
  }while( rc==SQLITE_BUSY );
  printf("DONE %s: %s\n", zFile, zSql);
  if( zErrMsg ){
    fprintf(stdout,"%s: command failed: %s - %s\n", zFile, zSql, zErrMsg);
    free(zErrMsg);
    sqlite3_free(zSql);
    exit(1);
  }
  sqlite3_free(zSql);
}

static void task_fat(void * pParam)
{

	RAW_U32 bw;
	
	FRESULT res;

	sqlite3 *db;
	int rc;
	char *zErrMsg = 0;

	int t = atoi("txj");
	
	raw_memset(buffer, 0xa1, 40);

	res = f_mount(&fss, "0:", 0);	
	vc_port_printf("f_mount res is  %d\r\n", res);	

	res = f_mkfs("0:", 0, 0); 
	vc_port_printf("f_mkfs res is  %d\r\n", res);

	res = f_mount(&fss, "0:", 1);	
	vc_port_printf("f_mount res is  %d\r\n", res);	
	
	res = f_mkdir("PICTURE1");

	vc_port_printf("f_mkdir res is  %d\r\n", res);	

	
	res = f_open(&file, "AA.TXT", FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
	vc_port_printf("the f_open return is %d\r\n",res);


	res = f_write(&file, buffer, 20, &bw);
	vc_port_printf("the f_write return is %d\r\n",res);
	vc_port_printf("the f_write bytes is %d\r\n",bw);

	f_lseek(&file, 0);
	
	res = f_read (&file, buffer2, 20, &bw);	
	vc_port_printf("the read bytes is %d\r\n",bw);
	
	res = f_close(&file);
	vc_port_printf("f_close return is %d\r\n", res);
/*
	rc = sqlite3_open("test222", &db);
	
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);

	}

	vc_port_printf("sqlite3_open is %d\r\n", res);

	// db_execute(db, "txj", "CREATE TABLE t%d(a,b,c);", 1);

	//rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
	
	sqlite3_close(db);
*/
	while (1);

}


extern RAW_U8 test_started_raw;

void fat_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	

	raw_task_create(&fat_task_obj, (RAW_U8  *)"task_fat", 0,
	                         30, 0,  test_task_fat, 
	                         TEST_TASK_STK_SIZE , task_fat, 1); 

	

}

