## 终端 1: 调试 semaphore_w
在第一个终端中，启动 GDB 并 attach 到 semaphore_w 进程：
```bash
gdb semaphore_w
```
在 GDB 提示符下，设置断点：
```gdb
(gdb) break main
(gdb) break while(1)
```
然后运行程序：
```gdb
(gdb) run
```
程序将在 main 函数或 while(1) 循环的开始处停止。您可以检查和修改变量，单步执行，或者继续运行：
```gdb
(gdb) print variable_name  # 查看变量值
(gdb) next  # 单步执行（不进入函数）
(gdb) step  # 单步执行（进入函数）
(gdb) continue  # 继续运行直到下一个断点
```
当 semaphore_w 等待用户输入时，您可以切换到终端 2，运行 semaphore_r。
## 终端 2: 调试 semaphore_r
在第二个终端中，启动 GDB 并 attach 到 semaphore_r 进程：
```bash
gdb semaphore_r
```
在 GDB 提示符下，设置断点：
```gdb
(gdb) break main
(gdb) break while(1)
```
然后运行程序：
```gdb
(gdb) run
```
程序将在 main 函数或 while(1) 循环的开始处停止。您可以检查和修改变量，单步执行，或者继续运行：
```gdb
(gdb) print variable_name  # 查看变量值
(gdb) next  # 单步执行（不进入函数）
(gdb) step  # 单步执行（进入函数）
(gdb) continue  # 继续运行直到下一个断点
```
现在，您可以在两个终端中分别观察两个程序的执行情况。当 semaphore_w 写入共享内存后，semaphore_r 应该能够读取到写入的数据。您可以在 semaphore_r 的 GDB 中使用 print 命令来检查共享内存的内容。
## 跨越进程边界的调试
由于 semaphore_r 和 semaphore_w 访问相同的共享内存和信号量，您可能需要在两个进程中交叉检查状态。例如，当 semaphore_w 更新了共享内存并释放了 sem_w 信号量时，semaphore_r 应该能够获取 sem_r 信号量并读取共享内存。
您可以使用以下命令来观察信号量的状态：
```gdb
(gdb) print sem_r->_sem  # 查看信号量 sem_r 的状态
(gdb) print sem_w->_sem  # 查看信号量 sem_w 的状态
```
请确保在两个进程中正确地处理了信号量，并且在写入共享内存后正确地释放了相应的信号量。
## 注意事项
确保在两个进程中使用相同的共享内存键和信号量名称。
在调试时，注意不要让一个程序无限期地持有信号量，这可能会导致另一个程序无法继续执行。
如果您在调试过程中更改了代码，请记得重新编译并重新启动 GDB。
通过这种方式，您可以在两个进程中分别进行调试，并观察它们如何通过共享内存和信号量进行交互。希望这能帮助您解决问题。如果您有更多问题，请随时提出。
```
您可以将这段 Markdown 代码保存到一个文件中，文件扩展名为 `.md`，例如 `debugging_semaphore_w_r.md`。这样，您就有了一个格式化的记录，可以在支持 Markdown 的编辑器中查看。
