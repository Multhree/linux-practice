#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

 
int main(int argc, char **argv)
{
    int server_sockfd,client_sockfd;
    socklen_t client_len = 0;
 
    struct sockaddr_in server_addr;	//定义服务器端套接口数据结构server_addr 
    struct sockaddr_in client_addr; //定义客户端套接口数据结构client_addr 
 
    // 服务器端开始建立 socket 描述符
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    // 设置服务器接收的连接地址和监听的端口
    server_addr.sin_family = AF_INET;                   // 指定网络套接字为设为 TCP/IP 地址族 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // 接受所有IP地址的连接
    server_addr.sin_port = htons(6666);                 // 绑定到 6666 端口
 
    // 设为TCP/IP地址族
    bind(server_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
 
    // 创建套接字队列，监听套接字，同时处理的最大连接请求数为5 
    listen(server_sockfd, 5);
 
    // 忽略子进程停止或退出信号
    signal(SIGCHLD, SIG_IGN);
 
    while (1)
    {
        char ch = '\0';
        client_len = sizeof(client_addr);
        printf("Server waiting\n");
 
        // 调用 accept 接受一个连接请求
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
 
        if (fork() == 0)
        {
            // 子进程中，读取客户端发过来的信息，处理信息，再发送给客户端
            read(client_sockfd, &ch, 1);
            printf("char from client  = %c\n",ch);
            sleep(1);
            ch++;
            write(client_sockfd, &ch, 1);
            close(client_sockfd);
            exit(0);
        }
        else
        {
            // 父进程中，关闭套接字
            close(client_sockfd);
        }
    }
    
    return 0; 
}

