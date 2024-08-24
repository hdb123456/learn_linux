# 改变gdb对于信号量的处理方式
    这个命令告诉 GDB 在接收到 SIGINT 信号时，执行 handle 函数
    handle SIGINT nostop noprint pass
# 阻塞时 才能被信号量打断