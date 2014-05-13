#Raw-OS_Sqlite

---
Raw-OS_Sqlite is a [Sqlite3](http://www.sqlite.org/) porting on [Raw-OS](http://www.raw-os.org/) Real Time Operation System. This project implements a very sample VFS named "RAW" that uses [FatFs](http://elm-chan.org/fsw/ff/00index_e.html) API such as f_open(), f_close(), f_read(), f_write(), f_seek(), f_truncate(), f_sync(), f_unlink(), and so forth.

##Screenshots

![sqlite_shell](https://github.com/xluoly/raw-os_sqlite/raw/master/images/sqlite_shell.png)

---
##How to build

###MinGW

    $ cd port/win32/mingw
    $ make

###MSVC2005
Open file port\win32\msvc2005\raw-vc.sln with Microsoft Visual Studio 2005, and run *build solution*.

###MSVC2010
Open file port\win32\msvc2010\raw-vc.sln with Microsoft Visual Studio 2010, and run *build solution*.

##How to run

Open DOS command prompt, and CD to excuable output directory.

    > sqlite 0:database.db 

For more infomation, please refer to [this](http://www.sqlite.org/cli.html).
Enjoy it. 
