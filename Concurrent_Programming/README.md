

exec函数族
守护进程
GDB 调试多进程程序

线程的创建和参数传递
线程的回收及内存演示
线程的取消和清理
互斥锁/读写锁的概念及使用、死锁的避免
条件变量的使用及注意事项
线程池及gdb调试多线程
有名管道和无名管道
共享内存（内存映射的使用
信号机制
信号机制
消息队列
信号灯


systemd是一个Linux系统的初始化系统和系统管理守护进程





创建,回收，销毁。
创建：pthread_create()
回收：pthread_join()
销毁：pthread_detach()
查看进程号 
ps -eLf | grep splash
用户           PID    PPID    PGID    SID    TTY          STAT    TIME COMMAND
root           1       0       1      0      1      6月09 ?       00:01:01 /sbin/init splash
xiaohe    166050  164301  166050      0      1      14:35 pts/1    00:00:00 grep --color=auto splash


2.线程





System V IPC（Inter-Process Communication，进程间通信）是一组在Unix和类Unix操作系统中提供的进程间通信机制。
这些机制在System V版本的操作系统中被引入，并因此得名。
System V这个名字来源于Unix操作系统的一个版本

System V IPC包括三种主要的通信机制：消息队列（Message Queues）、信号量（Semaphores）和共享内存（Shared Memory）。
