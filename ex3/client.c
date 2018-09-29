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
 
    // �ͻ�����ʼ���� socket ������ 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    // ����Ҫ���ӵķ���������Ϣ
    address.sin_family = AF_INET;                       // ָ�������׽���Ϊ��Ϊ TCP/IP ��ַ�� 
    address.sin_addr.s_addr = inet_addr("127.0.0.1");   // ��������ַ
    address.sin_port = htons(6666);                     // �������������Ķ˿�

 
    // ���ӵ������� 
    if (connect(sockfd, (struct sockaddr *) &address, (socklen_t) sizeof(address)) == -1)
    {
        perror("ops: client\n");
        exit(1);
    }
 	printf("input ch:\n");
 	scanf("%c",&ch);
    // �������ݸ�������
    write(sockfd, &ch, 1);
 
    // �ӷ�������ȡ����
    read(sockfd, &ch, 1);
 
    printf("char from server = %c\n", ch);
    close(sockfd);
    exit(0);
}

