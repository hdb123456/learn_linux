#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main(){
    pid_t pid;
    pid=fork();//创建子进程
    if(pid==0){
        pid_t pid1=fork();//创建孙子进程
        if(pid1==0){
            pid_t pid2=fork();//创建重孙进程
            if(pid2==0){
                pid_t pid3=fork();//创建重重孙进程
            }
        }
    }
    return 0;
}
