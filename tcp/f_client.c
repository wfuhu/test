#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<arpa/inet.h>

int main()
{
        // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);   // 大端端口
    inet_pton(AF_INET, "192.168.75.128", &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("connect");
        exit(0);
    }
        char buff[1024]="ni hao";
    while(1)
    {
        

        write(fd, buff, strlen(buff)+1);
        memset(buff,0,sizeof(buff));//接收数据
        int len=recv(fd,buff,sizeof(buff),0);
        if(len>0)
        {
            printf("service%s\n",buff);
        }
        else if(len==0)
        {
            printf("duankai");
            break;
        }
        else
        {
            perror("recv");
            break;
        }
    }

    close(fd);
    return 0;


}