#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

 
int main(int argc, char **argv)
{
    int server_sockfd,client_sockfd;
    socklen_t client_len = 0;
 
    struct sockaddr_in server_addr;	//������������׽ӿ����ݽṹserver_addr 
    struct sockaddr_in client_addr; //����ͻ����׽ӿ����ݽṹclient_addr 
 
    // �������˿�ʼ���� socket ������
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    // ���÷��������յ����ӵ�ַ�ͼ����Ķ˿�
    server_addr.sin_family = AF_INET;                   // ָ�������׽���Ϊ��Ϊ TCP/IP ��ַ�� 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // ��������IP��ַ������
    server_addr.sin_port = htons(6666);                 // �󶨵� 6666 �˿�
 
    // ��ΪTCP/IP��ַ��
    bind(server_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
 
    // �����׽��ֶ��У������׽��֣�ͬʱ������������������Ϊ5 
    listen(server_sockfd, 5);
 
    // �����ӽ���ֹͣ���˳��ź�
    signal(SIGCHLD, SIG_IGN);
 
    while (1)
    {
        char ch = '\0';
        client_len = sizeof(client_addr);
        printf("Server waiting\n");
 
        // ���� accept ����һ����������
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
 
        if (fork() == 0)
        {
            // �ӽ����У���ȡ�ͻ��˷���������Ϣ��������Ϣ���ٷ��͸��ͻ���
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
            // �������У��ر��׽���
            close(client_sockfd);
        }
    }
    
    return 0; 
}

