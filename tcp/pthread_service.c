#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include<pthread.h>

struct SockInfo
{
    struct sockaddr_in addr;
    int fd;
};
struct SockInfo infos[512];

void* work(void* arg){

    struct SockInfo* pinfo=(struct SockInfo*)arg;
    char ip[32];
    printf("客户端ip%s,端口%d\n",
    inet_ntop(AF_INET,&pinfo->addr.sin_addr,ip,sizeof(ip)),
    ntohs(pinfo->addr.sin_port));
    
    while (1)
    {
        
        char buff[1024];
        int len=recv(pinfo->fd,buff,sizeof(buff),0);
        printf("clinet%d\n",len);
        if(len>0)
        {
            printf("clinet%s\n",buff);
            send(pinfo->fd,buff,len,0);
        }
        else if(len==0)
        {
            printf(" \n");
            break;
        }
        else
        {
            perror("recv");
            break;
        }
    }
    close(pinfo->fd);
    pinfo->fd=-1;
    return NULL;
}

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
    saddr.sin_family=AF_INET;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_port=htons(10000);//端口网络大端转换电脑小端
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

    int max=sizeof(infos)/sizeof(infos[0]);
    for(int i=0;i<max;++i)
    {

        memset(&infos[i],0,sizeof(infos[i]));
        infos[i].fd=-1;
    }
    int addrlen=sizeof(struct sockaddr_in);
     while (1)
     {
        /* code */
        struct SockInfo* pinfo;
        
        for (size_t i = 0; i < max; i++)
        {
            if(infos[i].fd==-1)
            {
                 
                pinfo=&infos[i];
                break;
            }
        }
        int cfd=accept(fd,(struct sockaddr*)&pinfo->addr,&addrlen);
        pinfo->fd=cfd;
        
        if(cfd==-1)
        {
            perror("accept");
            break;
        }
        pthread_t tid;
        pthread_create(&tid,NULL,work,pinfo);
        pthread_detach(tid);
        close(cfd);
    }
     
   


    close(fd);

    


    return 0;
    
}