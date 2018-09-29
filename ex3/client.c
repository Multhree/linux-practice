#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
 
int main(int argc, char **argv)
{
    int sockfd = -1;
    struct sockaddr_in address;
    int result;
    char ch = 'A';
 
    // 客户程序开始建立 socket 描述符 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    // 设置要连接的服务器的信息
    address.sin_family = AF_INET;                       // 指定网络套接字为设为 TCP/IP 地址族 
    address.sin_addr.s_addr = inet_addr("127.0.0.1");   // 服务器地址
    address.sin_port = htons(6666);                     // 服务器所监听的端口

 
    // 连接到服务器 
    if (connect(sockfd, (struct sockaddr *) &address, (socklen_t) sizeof(address)) == -1)
    {
        perror("ops: client\n");
        exit(1);
    }
 	printf("input ch:\n");
 	scanf("%c",&ch);
    // 发送数据给服务器
    write(sockfd, &ch, 1);
 
    // 从服务器获取数据
    read(sockfd, &ch, 1);
 
    printf("char from server = %c\n", ch);
    close(sockfd);
    exit(0);
}

