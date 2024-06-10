#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define DROP_MS 500 // 下坠时间间隔
#define MOVE_MS 100 // 障碍块移动时间间隔

// Elements
#define BIRD "bird"
#define SPACE_KEY ' '

// 障碍块相关参数
#define OBSTACLES "@"
#define OBSTACLES_WIDTH 5
#define INTERVIEWS 20 // 障碍块间隔
#define OBSTACLES_X 100 // 障碍块初始 x 坐标
#define OBSTACLES_MOUNT 3        // 障碍块数量
#define OBSTACLES_Y_MIN 15 // 障碍块最小 y 坐标
#define OBSTACLES_Y_MAX 40 // 障碍块最大 y 坐标
#define CHANNEL_WIDTH 5 // 通道宽度

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct OBSTACLE {
    int x;
    int y;
    struct OBSTACLE *next;
} OBSTACLE, *OBS;

// INIT_STATE
Position bird_position = {10, 10};
OBS obs_list = NULL;


volatile sig_atomic_t move_flag = 0;
volatile sig_atomic_t drop_flag = 0;

 int high=0;
 int length=0;

// 初始化 curses 环境
int init_curses(void);

// 退出 curses 程序
void end_curses(void);

// 处理对象显示
static void Add(char target[]);
// 定时器
static int set_timer(int ms, void (*handler)(int));
// 生成随机高度并放入链表中
static void random_controller(void);
//存储高度
static void generate_obstacles_heights(void);

// 显示障碍物
static void obstacles_display(OBS extern_p,int key);
static void obstacles_move(void);
static void Move(int sig);

static void bird_rise(void) ;
static void bird_drop(void);
static void drop(int sig);





// 统计障碍物数量
// static int sum_obstacles(void);


int main(int argc, char *argv[])
{
    if (init_curses() == ERR) {
        end_curses();
        fprintf(stderr,"Faild to initialize curses.\n");
        return 1;
    }
    set_timer(MOVE_MS, Move);
    set_timer(DROP_MS, drop);
  
    srand(time(NULL));
 //   generate_obstacles_heights();//产生5个随机数，并存入链表
    // random_controller(); 
    // obstacles_display(obs_list,1); 

    // obstacles_display(obs_list,0); 
 
   
//    obstacles_display(1);
//    obstacles_move();
    while (1) 
    {
        if (drop_flag == 1) {
            bird_drop();
            drop_flag = 0;
        }

        bird_rise();
        for(int i=0;i<2;i++){
            random_controller(); 
        }
        obstacles_display(obs_list,0); 
        obs_list->x-=5;
        obstacles_display(obs_list,1);


        // while(obs_list!=NULL){
        //     obs_list=obs_list->next;
        // }

        
        // if(move_flag == 1)
        // {
        //     obstacles_move();
        //     move_flag = 0;
        // }
    //   obstacles_move(); 
    }
    end_curses();
    return 0;
}

// 初始化 curses 环境
int init_curses(void) {
    if ((initscr() == NULL) || (curs_set(0) == ERR) || (noecho() == ERR) || (keypad(stdscr, TRUE) == ERR) || (start_color() == ERR)) {
        return ERR;
    }
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    return OK;
}

// 退出 curses 程序
void end_curses(void) {
    endwin();
}
// 鸟上升
static void bird_rise(void) {
    switch(getch()){
        case SPACE_KEY: {
            attron(COLOR_PAIR(1));
            Add("    ");
            bird_position.y -= 1;
            Add(BIRD);
            attroff(COLOR_PAIR(1));
            break;
        }   
        default:
            break;
    }
}

// 鸟下落
static void bird_drop(void) { 
    attron(COLOR_PAIR(1));
    Add("    ");
    bird_position.y += 1;
    Add(BIRD);
    attroff(COLOR_PAIR(1));
}

// 信号触发函数
static void drop(int sig) {
    drop_flag = 1;
}
// 定时器
static int set_timer(int ms, void (*handler)(int)) {
    int TV_SEC = ms / 1000;
    int TV_USEC = (ms % 1000) * 1000;

    struct sigaction sa = {.sa_handler = handler, .sa_flags = 0};
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        end_curses();
        fprintf(stderr, "Failed to set signal handler.\n");
        return 1;
    }
    struct itimerval timer = {
            .it_value = {.tv_sec = TV_SEC, .tv_usec = TV_USEC},
            .it_interval = {.tv_sec = TV_SEC, .tv_usec = TV_USEC}
        };
    setitimer(ITIMER_REAL, &timer, NULL);

    return 0;
}

// 让新生成的障碍物具有高度，并显示，宽度与移动相结合在移动里做调整
static void obstacles_display(OBS extern_p,int key) {
   OBS in_p =extern_p ;//换为in_p->x=extern_p->x;也行
    high=extern_p->y; 
    length=extern_p->x;
    for (int i = 0; i < OBSTACLES_WIDTH; i++) 
    {   
        for(int j=0 ;j<OBSTACLES_Y_MAX; j++)
        {
            if (j>= high && j < (high + CHANNEL_WIDTH)) {
                in_p->y = j + CHANNEL_WIDTH;
                j += CHANNEL_WIDTH;
            }
            in_p->y = j;

            // attron(COLOR_PAIR(2));
            // if (key == 1) {
            //     move(in_p->y, in_p->x);
            //     addstr(OBSTACLES);
            //     refresh();
            // } else if (key == 0) {
            //     move(in_p->y, in_p->x);
            //     addstr(" ");
            //     refresh();
            // }
          
            attroff(COLOR_PAIR(2));
            in_p->y++;
       }  
       in_p->x++;
    } 
   extern_p->y = high; 
   extern_p->x = length;
}

static void obstacles_move(void){
    OBS p = NULL; 
    p = obs_list;
    random_controller();        
}
static void Move(int sig){
    move_flag=1;
}


static void generate_obstacles_heights(void){
    //开始时产生5个随机数
    for (int i = 0; i < OBSTACLES_MOUNT; i++) {
        random_controller();
    }
    //当一个障碍块消失的时候增加一个障碍块
    //障碍块消失
    // if(obs_list->x=0)
    // {
    //     OBS current=obs_list;
    //     obs_list=current->next;
    //     free(current);
    //     //空格填充？
    //     random_controller();
    // }
    //当调用时，维持高度只是移动
    

}

// 生成随机高度,并放入链表中；初始位置确定
static void random_controller(void) {
    int randomNumber = rand() % (OBSTACLES_Y_MAX-CHANNEL_WIDTH- OBSTACLES_Y_MIN) + OBSTACLES_Y_MIN;
    OBS p = (OBSTACLE*)malloc(sizeof(OBSTACLE));
    if (p == NULL) {
        end_curses();
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(1);
    }   
    p->x = OBSTACLES_X ;
    p->y = randomNumber;
    p->next = NULL;

    if (obs_list == NULL)
        {
        obs_list = p;
        }
    else 
    {
        OBS current = obs_list;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = p;
    }   
}

// 统计障碍物数量
// static int sum_obstacles(void) {
//     int count = 0;
//     OBS p = obs_list;
//     while (p != NULL) {
//         count++;
//         p = p->next;
//     }
//     return count;
// }
// 定义 Add 函数
static void Add(char target[]) {
    if (strcmp(target, OBSTACLES) == 0 || target == " ") {
        move(obs_list->y, obs_list->x);
    } else if (strcmp(target, BIRD) == 0 || target == "    ") {
        move(bird_position.y, bird_position.x);
    }
    addstr(target);
    refresh();
}

