#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MS 500 // 更新间隔(毫秒)
#define Max_X 100
#define Max_Y 100

//elements
#define bird "bird"
#define SPACE_KEY ' '
#define obstacles "@@@@@@@@@@@"
#define intervals 20

typedef struct {
    int x;
    int y;
}Position;

Position bird_position = {10, 10};
Position intervals_position = {100-strlen(obstacles), 0};
volatile sig_atomic_t drop_flag = 0;

//初始化curses环境
int init_curses(void);
//退出curses程序
void end_curses(void);
//处理对象显示
static void Add(char target[]);

// 动作实现
static void bird_drop(void);
static void bird_rise(void);
//信号触发函数
static void drop(int sig);
//定时器
static int timer(int ms);

int main(int argc, char *argv[])
{
    if(init_curses() == ERR){
        end_curses();
        fprintf(stderr,"Faild to initialize curses.\n");
        return 1;
    }
/*bird drop part:start*/
    timer(MS);
    while(1){
        if(drop_flag==1)
        {
            bird_drop();
            drop_flag=0;
        }
/*bird drop part:end*/
/*bird rise part:start*/
        bird_rise();
/*bird rise part:end*/
    }
    endwin();
    return 0;
}
//###############################################################################################################//
//Strat or End
int init_curses(void){
    if ((initscr() == NULL) || (curs_set(0) ==ERR) || (noecho() == ERR) || (keypad(stdscr, TRUE) == ERR) || (start_color() == ERR)) {
        return ERR;
    }
     init_pair(1, COLOR_RED, COLOR_BLACK);
    return OK;
}
void end_curses(void){
    endwin();
}
//################################################################################################################//
// bird rise or drop
static void bird_rise(void)
{
    switch(getch()){
        case ' ':{
            attron(COLOR_PAIR(1));
            Add("    ");
            bird_position.y-=1;
            Add(bird);
            attroff(COLOR_PAIR(1));
            break;
        }   
        default:
            break;
    }
}
static void bird_drop(void)
{ 
    attron(COLOR_PAIR(1));
    Add("    ");
    bird_position.y+=1;
    Add(bird);
    attroff(COLOR_PAIR(1));
}
static void drop(int sig){
    drop_flag = 1;
}

//################################################################################################################//
// BOTTOM LAWYAR
static void Add(char target[])
{
    move(bird_position.y, bird_position.x);
    addstr(target);
    refresh();
}
static int timer(int ms)
{
    int TV_SEC=ms/1000;
    int TV_USEC=(ms%1000)*1000;
    struct sigaction sa = {.sa_handler = drop, .sa_flags = 0};
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        end_curses();
        fprintf(stderr, "Failed to set signal handler.\n");
        return 1;
    }
    struct itimerval timer = {
            .it_value = {.tv_sec=TV_SEC,.tv_usec=TV_USEC},
            .it_interval = {.tv_sec=TV_SEC,.tv_usec=TV_USEC}
        };
    setitimer(ITIMER_REAL, &timer, NULL);
}

// int rows, cols;
// getmaxyx(stdscr, rows, cols);
