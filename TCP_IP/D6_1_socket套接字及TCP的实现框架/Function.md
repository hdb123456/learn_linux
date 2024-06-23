<!-- TOC -->
- [1. 输入/输出（通用）](#1-输入输出通用)    
    - [1.1. getaddrinfo():](#11-getaddrinfo)    
    - [1.2. setsockopt():](#12-setsockopt)    
    - [1.3. getsockopt():](#13-getsockopt)    
    - [1.4. shutdown():](#14-shutdown)
- [2. 客户端](#2-客户端)    
    - [2.1. socket():](#21-socket)    
    - [2.2. connect():](#22-connect)    
    - [2.3. send() / sendto():](#23-send--sendto)    
    - [2.4. recv() / recvfrom():](#24-recv--recvfrom)    
    - [2.5. close():](#25-close)
- [3. 服务器端](#3-服务器端)    
    - [3.1. socket():](#31-socket)    
    - [3.2. bind():](#32-bind)    
    - [3.3. listen():](#33-listen)    
    - [3.4. accept():](#34-accept)    
    - [3.5. send() / sendto():](#35-send--sendto)    
    - [3.6. recv() / recvfrom():](#36-recv--recvfrom)    
    - [3.7. close():](#37-close)
<!-- /TOC -->
# 1. 输入/输出（通用）
## 1.1. getaddrinfo(): 
    解析主机名和服务名到地址结构数组，用于获取网络地址信息。
## 1.2. setsockopt(): 
    设置套接字的选项，比如超时、重用地址等。
## 1.3. getsockopt(): 
    获取套接字的当前设置选项。
## 1.4. shutdown(): 
    控制套接字的读、写权限，可以是关闭读、关闭写或同时关闭。
# 2. 客户端
## 2.1. socket(): 
```cpp
int socket(int domain,int type,int protocol);
```
- domain: 指定通信的域，即通信发生的网络范围。常见的值有：
    - AF_INET: 用于IPv4互联网协议通信。
    - AF_INET6: 用于IPv6互联网协议通信。
    - AF_UNIX: 用于同一台机器上的进程间通信。
- type: 指定套接字类型，决定了通信的特性。主要类型包括：

    SOCK_STREAM: 提供面向连接的、可靠的、基于字节流的服务，如TCP协议。
    SOCK_DGRAM: 提供无连接的、不可靠的、基于数据报的服务，如UDP协议。
    SOCK_RAW: 提供原始套接字，直接访问底层协议。
protocol: 指定使用的特定协议。一般情况下，当type已经确定了协议类型（如TCP或UDP），可以传入0，让系统自动选择合适的协议。

返回值
成功时，返回新创建的套接字描述符（一个非负整数），后续网络操作都将通过这个描述符进行。
失败时，返回-1，并且可以通过errno变量获取具体的错误代码。
示例代码
c
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
在这段示例代码中：

AF_INET 表示使用IPv4协议族。
SOCK_STREAM 表示创建的是一个面向连接的、可靠的套接字，适用于TCP通信。
第三个参数为0，意味着让系统自动选择与SOCK_STREAM对应的默认TCP协议。
创建套接字后，通常接下来会执行bind()来绑定本地地址、listen()（如果是服务器）来监听连接请求、connect()（如果是客户端）来发起连接请求，或是进行其他相应的网络配置操作。
## 2.2. connect(): 
    尝试连接到服务器。
## 2.3. send() / sendto(): 
    发送数据到服务器。
## 2.4. recv() / recvfrom(): 
    接收来自服务器的数据。
## 2.5. close(): 
    关闭客户端套接字。
# 3. 服务器端
## 3.1. socket(): 
    创建服务器端套接字。
## 3.2. bind(): 
    将服务器端套接字绑定到特定地址和端口。
## 3.3. listen(): 
    使服务器端套接字开始监听连接请求。
## 3.4. accept(): 
    接受一个客户端的连接请求，并为该连接创建新的套接字。
## 3.5. send() / sendto(): 
    发送数据给已连接的客户端。
## 3.6. recv() / recvfrom(): 
    从已连接的客户端接收数据。
## 3.7. close(): 
    关闭服务器端或与客户端通信的套接字。

- 请注意，尽管send()/sendto()和recv()/recvfrom()被分类在客户端和服务器端，这些函数实则在两端均会被使用，依据数据交互的方向。类似地，shutdown()、setsockopt()、getsockopt()等函数在客户端与服务器端程序设计中均扮演着重要角色，用以实现更高级的控制和配置功能。