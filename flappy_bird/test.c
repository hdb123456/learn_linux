#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define drop_MS 500 // 下坠时间间隔
#define move_MS 1000 // 障碍块移动时间间隔

//elements
#define bird "bird"
#define SPACE_KEY ' '

#define obstacles "@"

//障碍块宽度
#define obstacles_width 5


#define intervals 10//障碍块间隔

//障碍块初始x坐标
#define obstacles_x 30

//障碍块数量
#define obstacles_mount 4

//障碍块最小y坐标
#define obstacles_ymin 5

//障碍块最大y坐标
#define obstacles_ymax 30

//通道宽度
#define channel_width 5

typedef struct Position {
    int x;
    int y;
}Position;
typedef struct High {
    int high;
    struct High *next;
}High,*High_List;
// INIT_STATE
Position bird_position = {10, 10};
Position obstacles_position = { obstacles_x, 0};
High_List high_list = NULL;
volatile sig_atomic_t drop_flag = 0;
volatile sig_atomic_t obstacles_kill_flag =0;

int count=0;

//初始化curses环境
int init_curses(void);
//退出curses程序
void end_curses(void);
//处理对象显示
static void Add(char target[]);
// 动作实现
static void bird_drop(void);
static void bird_rise(void);

static void obstacles_display(void);

static void random_controllor(void);
static void sum(void);

//信号触发函数
static void drop(int sig);
//定时器
static int set_timer(int ms,void (*handler)(int));

//static void kill_flag(int sig);


int main(int argc, char *argv[])
{
    if(init_curses() == ERR){
        end_curses();
        fprintf(stderr,"Faild to initialize curses.\n");
        return 1;
    }
    
/*bird drop part:start*/
    set_timer(drop_MS,drop);
 //   set_timer(move_MS, kill_flag);
    srand(time(NULL));
    obstacles_display();
    /*obstacles part:start*/
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

static void obstacles_display(void){
    High_List h; 

    while(count<=obstacles_mount)
    {//障碍物数量

        random_controllor();
        sum();
        if(count==1)
        {
            h=high_list;
        }
        else if(count>1)
        {
            h=h->next; 
        }
        for(int j=0;j<obstacles_width;j++)//障碍物宽度
        {  
            int y=0;
            obstacles_position.x=obstacles_x+(count-1)*(intervals+obstacles_width)+j;
            while(y<obstacles_ymax)//障碍块高度
            {
                if(y>=h->high&&y<(h->high+channel_width))
                {
                    obstacles_position.y=y+channel_width;
                    y+=channel_width;
                }
                obstacles_position.y=y;
                // if(obstacles_kill_flag){
                //     Add(" ");
                // }
                // else
                Add(obstacles);
                obstacles_position.y++;
                y++; 
            }
        } 
       // obstacles_kill_flag=0;
    }
     
}




//################################################################################################################//
// BOTTOM LAWYAR
static void Add(char target[])
{
    if(strcmp(target, obstacles) == 0||target==" ") {
        move(obstacles_position.y, obstacles_position.x);
    }
    else if(strcmp(target,bird)==0||target=="    "){
        move(bird_position.y, bird_position.x);
    }
    addstr(target);
    refresh();
}

static int set_timer(int ms,void (*handler)(int))
{
    int TV_SEC=ms/1000;
    int TV_USEC=(ms%1000)*1000;

    struct sigaction sa = {.sa_handler = handler, .sa_flags = 0};
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

    return 0;
}
/*统计链表中的数量，并存入count */
static void sum(void){
    High_List h;
    h=high_list;
    while(h!=NULL)
    {
        count++;
        h=h->next;
    }
}
/*生成一个随机高度并放入链表中,采用尾插法*/
static void random_controllor(void)
{  // int flag=0;
  //  while(flag<5){
        int randomNumber=rand()%(obstacles_ymax-obstacles_ymin+1)+obstacles_ymin;
        High_List h;
        h= (High*)malloc(sizeof(High));
        h->high=randomNumber;
        h->next=NULL;
        
        if (high_list == NULL) {// 将新节点添加到链表的末尾
            high_list = h;// 如果链表为空，将新节点作为头节点
        } 
        else // 否则，遍历链表直到找到末尾节点
        { 
            High_List current = high_list;
            while (current->next != NULL) 
            {
                current = current->next;
            }
            current->next = h;// 将新节点添加到末尾节点的后面
        }    
}

// int rows, cols;
// getmaxyx(stdscr, rows, cols);
