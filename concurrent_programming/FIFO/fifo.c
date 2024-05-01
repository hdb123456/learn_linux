#include "stdio.h"


int main(int argv ,char **argc)
{
    //检测是否存在管道(未实现)并mkfifo创建管道
   if( fifo=mkfifo("./Ffifo","777")==0)
   {
        perror("fifo file create success\n");
   }
    
    

  
}
