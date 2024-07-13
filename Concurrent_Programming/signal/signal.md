# 信号的状态
1. 递达状态：Delivery:忽略、执行默认动作、捕获
2. 未决状态：Pending:
# 信号集：
pending 未决信号集
mask 信号屏蔽字
# FUNCTIONS
```C++
sigset_t set;//自定义信号集
sigemptyset(sigset_t *set);//清空
sigfillset(sigset_t *set);//全部置1
sigaddset(sigset_t *set, int signo);//添加信号到集合中
sigdelset(sigset_t *set, int signo);//从集合中删除一个信号
sigismember(sigset_t *set, int signo);//判断信号是否在集合中
```
# sigprocmask
```C++
sigprocmask(int how, const sigset_t *set, sigset_t *oldset);//设置信号屏蔽字
```
- SIG_BLOCK:阻塞信号
- SIG_UNBLOCK:解除阻塞信号
- SIG_SETMASK:设置信号屏蔽字

***均不能阻塞SIGKILL和SIGSTOP***
# pause
```C++
pause();//阻塞进程，直到被信号中断
返回值：-1
errno：EINTR
```
函数行为：
1. 信号的默认处理动作是：终止进程，则进程终止，pause有机会返回
2. 信号的默认处理动作是：忽略，进程继续挂起，不返回
3. 信号的默认处理动作是：捕获，调用完信号处理函数之后，返回-1
4. 信号的默认处理动作是：屏蔽，pause不会唤醒
