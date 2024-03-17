#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
//awdawdawdawd

int main()
{
    int fd=socket(AF_INET6,SOCK_STREAM,0);//监听
    if(fd==-1)
    {
        perror("socket");
        return -1;
    }
    //绑定本地ip
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET6;
    saddr.sin_port=htons(9999);//端口网络大端转换电脑小端
    saddr.sin_addr.s_addr=INADDR_ANY;//实际为0 0=0.0.0.0  本地网卡地址
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

    //使用i/o复用,初始化据
    fd_set redset;
    fd_set temp;
    FD_ZERO(&redset);
    FD_SET(fd,&redset);
    struct timeval timeout;
    timeout.tv_sec=5;//5s
    timeout.tv_usec=5000;//纳秒
    
    int maxfd=fd;
    while (1)
    {
        temp=redset;
        int ret=select(maxfd+1,&temp,0,0,0);//判断是否监听的fd
        /* code */
        if(FD_ISSET(fd,&temp))
        {
            int cfd=accept(fd,NULL,NULL);
            FD_SET(cfd,&redset);
            maxfd=cfd>maxfd?cfd:maxfd;
        }
        for(int i=0;i<=maxfd;++i)
        {
            if(i!=fd&&FD_ISSET(i,&temp))
            {
                char buff[1024];
                int len=recv(i,buff,sizeof(buff),0);
                if(len==-1)
                {
                    perror("recv");
                    exit(1);
                }
                else if(len==0)
                {
                printf("");
                FD_CLR(i,&temp);
                close(i);
                break;
                }

                ret=send(i,buff,strlen(buff)+1,0);
                if(ret==-1)
                {
                    perror("send");
                    exit(1);
                }
            }
        }

    }
    

    return 0;
    
}