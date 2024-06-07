#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>

int  x=0,y=0;

void  display_cursor(void)
{
   addch('A');
}

void position_cursor(int arg)
{
    move(x,y);
    display();
    refresh(); 
}
int main(int argc, char *argv[])
{
    char ch;
    initscr();
    curs_set(0);//隐藏光标

    noecho();//不回显
    keypad(stdscr, TRUE);//允许键盘输入

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    signal(SIGALRM,handler);
    struct itimerval timer;
    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    ch  = getch();
    ch=(char)(inch());
    
    while(1);
    endwin();
}