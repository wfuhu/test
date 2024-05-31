#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


int main()
{
    int fd=socket(AF_INET,SOCK_STREAM,0);//监听
    if(fd==-1)
    {
        perror("socket");
        return -1;
    }
    //绑定本地ip
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));//】】】置0
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(9999);//端口网络大端转换电脑小端
    saddr.sin_addr.s_addr=INADDR_ANY;//实际为0 0=0.0.0.0  本地网卡地址

    int saddr_size=sizeof(saddr);
    int ret=bind(fd,(struct sockaddr*)&saddr,sizeof(saddr));//绑定ip地址
    if (ret==-1)
    {
        /* code */
        perror("bind");
        return -1;
    }
    if(listen(fd,128)==-1)//设置监听最大数
    {
        perror("listen");
        return -1;
    }

     struct sockaddr_in caddr;
     int caddr_size=sizeof(caddr);
    int cfd=accept(fd,(struct sockaddr*)&caddr,&caddr_size);
    if(cfd==-1)
    {
        perror("accept");
        return -1;
    }

    char ip[32];
    printf("客户端ip%s,端口%d\n",inet_ntop(AF_INET,&caddr.sin_addr.s_addr,ip,sizeof(ip)),ntohs(caddr.sin_port));
    
    while (1)
    {
        char buff[1024];
        int len=recv(cfd,buff,sizeof(buff),0);
        if(len>0)
        {
            printf("clinet%s\n",buff);
            send(cfd,buff,len,0);
        }
        else if(len==0)
        {
            printf("00");
            break;
        }
        else
        {
            perror("recv");
            break;
        }
    }
    

    close(fd);
    close(cfd);

    


    return 0;
    
}