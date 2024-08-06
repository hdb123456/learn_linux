# 常见的网络接口：
- berkeley UNIX操作系统定义了一种API,称为BSD Socket API；
- 微软的Windows NT操作系统定义了WinSock API；
- AT&T的UNIX操作系统定义了SysV Socket API；

# 三元组：
IP地址+端口号+协议
    1.IP地址：标识网络设备
    2.端口号：标识进程
    3.协议：指定数据传输方式

# 套接字：
## 地址族结构体：
- SA_family：地址族：IPv4、IPv6、Unix域、网络域
    - AF_UNIX：Unix域地址族,使用本地域套接字地址结构，用于本地通信；
    - AF_INET：IPv4地址族；
    - AF_INET6：IPv6地址族；

- 通用地址结构体：
```cpp
struct sockaddr{
    sa_family_t sa_family;
    char sa_data[14];
};
```
- IPv4地址结构体:
```cpp
struct sockaddr_in{
    sa_family_t sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
};
struct in_addr{
    uint32_t s_addr;
};
```
- IPv6地址结构体：
```cpp
struct sockaddr_in6{
    sa_family_t sin6_family;
    uint16_t sin6_port;
    uint32_t sin6_flowinfo;
    struct in6_addr sin6_addr;
    uint32_t sin6_scope_id;
};
struct in6_addr{
    uint8_t s6_addr[16];
};
```
## 套接字类型：
- SOCK_STREAM：流套接字，TCP协议；
- SOCK_DGRAM：数据报套接字，UDP协议；
- SOCK_RAW：原始套接字，用户协议；
![alt text](image.png)
    - 套接字属于一种特殊的文件描述符
    - UNIX域套接字用于本地进程间通信

实现过程：
1.创建套接字：
int socket(int domain,int type,int protocol);
2.绑定套接字：
int bind(int sockfd,const struct sockaddr *addr,socklen_t addrlen);
3.监听套接字：
int listen(int sockfd,int backlog);
4.接受连接：
int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
5.发送数据：
ssize_t send(int sockfd,const void *buf,size_t len,int flags);
6.接收数据：
ssize_t recv(int sockfd,void *buf,size_t len,int flags);

