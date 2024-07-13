/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    : main.c
  * @brief   : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 MingZe.
  * All rights reserved.</center></h2>
  *
  * This is a simple program demonstrating signal handling in C.
  * It sets up a signal handler to catch SIGINT signals (typically generated by pressing Ctrl+C).
  * After blocking the SIGINT signal temporarily, it sleeps for 5 seconds and then unblocks the signal.
  * Finally, it enters an infinite loop to keep the program running.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle(int si) {
    printf("get sig:%d\n", si);
}
/*
struct sigaction {
    void (*sa_handler)(int);
    void (*sa_sigaction)(int,siginfo_t*,void)
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
}*/


int main() {
    struct sigaction act;
    act.sa_handler = handle;    // 设置信号处理函数为 handle
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);  // 清空信号屏蔽集

    sigaction(SIGINT, &act, NULL); // 捕获 SIGINT 信号，并指定处理函数为 handle

    /* 初始化信号屏蔽集 */   
    //sigset_t mask;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);   // 将 SIGINT 信号添加到信号屏蔽集

    sigprocmask(SIG_BLOCK, &act.sa_mask, NULL);  // 阻塞 SIGINT 信号

    sleep(5);   // 睡眠 5 秒

    sigprocmask(SIG_UNBLOCK, &act.sa_mask, NULL);  // 解除对 SIGINT 信号的阻塞
  //  sigsuspend(&act.sa_mask);
  
    // 进入一个无限循环
    while(1)
    {
      sleep  ;
    }

    return 0; 
}


