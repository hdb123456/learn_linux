#include <ncurses.h>
#include <stdio.h>
int main() {
    // 初始化Ncurses
    initscr();
    
    // 检查终端是否支持颜色
    if (!has_colors()) {
        endwin();
        printf("你的终端不支持颜色！");
        return 1;
    }

    // 初始化颜色
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLUE);

    // 设置颜色属性
    attron(COLOR_PAIR(1));

    // 打印字符
    mvaddch(10, 10, '@');

    // 刷新屏幕
    refresh();

    // 等待用户输入任意键结束
    getch();

    // 结束Ncurses
    endwin();

    return 0;
}
