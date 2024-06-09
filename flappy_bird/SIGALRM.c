#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>

#define UPDATE_INTERVAL 1 // 更新间隔（秒）
#define START_POS_X 10
#define START_POS_Y 10
char string[]="Flappy bird";
#define STRING_LEN strlen(string)

typedef struct {
    int x;
    int y;
}Position;

Position pos = {START_POS_X, START_POS_Y};
volatile sig_atomic_t update_flag = 0;


//处理文本显示
static void text(void);
//定时器信号处理函数
static void display(int arg);

int main(int argc, char *argv[])
{
    initscr();
    curs_set(0);//隐藏光标

    //noecho();//不回显
    //keypad(stdscr, TRUE);//允许键盘输入

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    signal(SIGALRM,display);
    struct itimerval timer = {
        .it_value = {.tv_sec=UPDATE_INTERVAL,.tv_usec=0},
        .it_interval = {.tv_sec=UPDATE_INTERVAL,.tv_usec=0}
    };
    setitimer(ITIMER_REAL, &timer, NULL);
    
    while(1){
        if(update_flag){
            attron(COLOR_PAIR(1));
            move(pos.y, pos.x);
            text();
            pos.x+=STRING_LEN;
            erase();
            update_flag = 0;
            attroff(COLOR_PAIR(1));
        }
       // napms(10);
    }
    endwin();
    return 0;
}



static void display(int arg)
{   
    update_flag = 1;
}
static void text(void)
{
    addstr(string);
    refresh();
}