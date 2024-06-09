#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DROP_MS 500 // 下坠时间间隔
#define MOVE_MS 1000 // 障碍块移动时间间隔

// Elements
#define BIRD "bird"
#define SPACE_KEY ' '
#define OBSTACLES "@"

// 障碍块相关参数
#define OBSTACLES_WIDTH 5
#define INTERVIEWS 10 // 障碍块间隔
#define OBSTACLES_X 30 // 障碍块初始 x 坐标
#define OBSTACLES_MOUNT 4 // 障碍块数量
#define OBSTACLES_Y_MIN 5 // 障碍块最小 y 坐标
#define OBSTACLES_Y_MAX 30 // 障碍块最大 y 坐标
#define CHANNEL_WIDTH 5 // 通道宽度

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct High {
    int high;
    struct High *next;
} High, *High_List;

// INIT_STATE
Position bird_position = {10, 10};
Position obstacles_position = {OBSTACLES_X, 0};
High_List high_list = NULL;
volatile sig_atomic_t drop_flag = 0;
int count=0;

// 初始化 curses 环境
int init_curses(void);

// 退出 curses 程序
void end_curses(void);

// 处理对象显示
static void Add(char target[]);

// 鸟下落
static void bird_drop(void);

// 鸟上升
static void bird_rise(void);

// 信号触发函数
static void drop(int sig);

// 定时器
static int set_timer(int ms, void (*handler)(int));

// 显示障碍物
static void obstacles_display(void);

// 生成随机高度并放入链表中
static void random_controller(void);

// 统计障碍物数量
static void sum_obstacles(void);

int main(int argc, char *argv[])
{
    if (init_curses() == ERR) {
        end_curses();
        fprintf(stderr,"Faild to initialize curses.\n");
        return 1;
    }
    
    set_timer(DROP_MS, drop);
    srand(time(NULL));
    obstacles_display();

    while (1) {
        if (drop_flag == 1) {
            bird_drop();
            drop_flag = 0;
        }
        bird_rise();
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

// 显示障碍物
static void obstacles_display(void) {
    High_List h;
    
    while (count <= OBSTACLES_MOUNT) {
        random_controller();
        h = high_list;
        h = h->next;
        sum_obstacles();
        for (int i = 0; i < OBSTACLES_WIDTH; i++) {
            int y = 0;
            obstacles_position.x = OBSTACLES_X + (count - 1) * (INTERVIEWS + OBSTACLES_WIDTH) + i;
            while (y < OBSTACLES_Y_MAX) {
                if (y >= h->high && y < (h->high + CHANNEL_WIDTH)) {
                    obstacles_position.y = y + CHANNEL_WIDTH;
                    y += CHANNEL_WIDTH;
                }
                obstacles_position.y = y;
                Add(OBSTACLES);
                obstacles_position.y++;
                y++; 
            }
        }
         
    }
}

// 生成随机高度并放入链表中
static void random_controller(void) {
    int randomNumber = rand() % (OBSTACLES_Y_MAX - OBSTACLES_Y_MIN + 1) + OBSTACLES_Y_MIN;
    High_List h = (High*)malloc(sizeof(High));
    if (h == NULL) {
        end_curses();
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(1);
    }
    h->high = randomNumber;
    h->next = NULL;
    if (high_list == NULL) {
        high_list = h;
    } else {
        High_List current = high_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = h;
    }    
}

// 统计障碍物数量
static void sum_obstacles(void) {
    High_List h = high_list;
    count = 0;
    while (h != NULL) {
        count++;
        h = h->next;
    }
}
// 定义 Add 函数
static void Add(char target[]) {
    if (strcmp(target, OBSTACLES) == 0 || target == " ") {
        move(obstacles_position.y, obstacles_position.x);
    } else if (strcmp(target, BIRD) == 0 || target == "    ") {
        move(bird_position.y, bird_position.x);
    }
    addstr(target);
    refresh();
}
