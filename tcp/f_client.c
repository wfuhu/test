#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<arpa/inet.h>

int main()
{
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_port=htons(9999);
    inet_pton(AF_INET,"10.0.2.15",&saddr.sin_addr.s_addr);
    int ret=connect(fd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret==-1)
    {
        perror("connect");
        return -1;
    }

    while(1)
    {
        char buff[1024]="ni hao";

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