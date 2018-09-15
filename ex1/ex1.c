#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
#define BUFLEN  50
int main(int argc, char **argv)
{
    size_t data_len = 0;
    int filedes[2];
    const char data[] = "This is a message from parent process\n";
    char buffer[BUFLEN];
    pid_t pid;
 
 
    if (pipe(filedes) == 0)
    {
        // 创建管道成功
        // 调用fork 创建子进程
        pid = fork();
        if (pid == -1)
        {
            fprintf(stderr, "fork failure\n");
            exit(EXIT_FAILURE);
        }
 
        if (pid == 0)
        {
            // 子进程
            // 读取数据
            data_len = read(filedes[0], buffer, BUFSIZ);
            printf("I'm child porcess,Read %ld bytes: %s\n", data_len, buffer);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // 父进程
            // 写数据
            data_len = write(filedes[1], data, strlen(data));
            printf("I'm parent process,Wrote %ld bytes: %s\n", data_len, data);
 
            // 休眠2秒, 主要是为了等待子进程结束, 这样做也只是纯粹为了输出好看而已
            // 父进程其实没有必要等待子进程结束
            sleep(2);
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        fprintf(stderr, "pipe failure\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
