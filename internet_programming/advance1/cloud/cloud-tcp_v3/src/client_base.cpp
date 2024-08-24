#include <iostream>

bool login_input(void)
{
    bool flag = 0;
    char input = std::cin.get();
    if(input == '\n')
    {
        flag = 1;
    }
    return flag;
}
