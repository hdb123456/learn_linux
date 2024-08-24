# debug.gdb

# 设置断点
break main
#设置命令行参数
set args 0 8888
# 运行程序
run

#监视变量
watch ret
watch max_fd
break 43
continue
watch rfd


watch rfds
watch temp_rfds
#继续运行
#continue

# 打印变量值
#print 

# 单步执行
#step

# 继续运行
#continue

# 查看汇编代码
#disassemble

# 打印调用栈
#backtrace

# 退出gdb
#quit
