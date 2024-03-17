#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include<pthread.h>

pthread_mutex_t mutex;

typedef struct FdInfo
{
    int fd;
    int *maxfd;
    fd_set* redset;
}FDInfo;

void* acceptConnect(void* arg)
{
    FDInfo* info=(struct FDInfo*)arg;
    int cfd=accept(info->fd,NULL,NULL);
    pthread_mutex_lock(&mutex);
    FD_SET(cfd,info->redset);
    info->maxfd=cfd>info->maxfd?cfd:info->maxfd;
    pthread_mutex_unlock(&mutex);
    free(info);
    return NULL;
}

void* comm_pthread(void* arg)
{
    FDInfo* info=(struct FDInfo*)arg;
     char buff[1024];
                int len=recv(info->fd,buff,sizeof(buff),0);
                if(len==-1)
                {
                    perror("recv");
                    free(info);
                }
                else if(len==0)
                {
                printf("");
                pthread_mutex_lock(&mutex);
                FD_CLR(info->fd,info->redset);
                pthread_mutex_unlock(&mutex);
                close(info->fd);
                free(info);
                return NULL;
                }

                int ret=send(info->fd,buff,strlen(buff)+1,0);
                if(ret==-1)
                {
                    perror("send");
                }
                free(info);
                return NULL;
}


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
    saddr.sin_family=AF_INET;
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
    
    pthread_mutex_init(&mutex,NULL);

    int maxfd=fd;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        temp=redset;
        pthread_mutex_unlock(&mutex);
        int ret=select(maxfd+1,&temp,0,0,0);//判断是否监听的fd
        /* code */
        if(FD_ISSET(fd,&temp))
        {
            pthread_t pid;
            FDInfo* info=(FDInfo*)malloc(sizeof(FDInfo));
            info->fd=fd;
            info->maxfd=&maxfd;
            info->redset=&redset;
            pthread_create(&pid,NULL,acceptConnect,info);
            pthread_destory(pid);

        }
        for(int i=0;i<=maxfd;++i)
        {
            if(i!=fd&&FD_ISSET(i,&temp))
            {

                pthread_t pid;
                FDInfo* info=(FDInfo*)malloc(sizeof(FDInfo));
                info->fd=i;
                info->redset=&redset;
                pthread_create(&pid,NULL,comm_pthread,info);
                pthread_destory(pid);    

               
            }
        }

    }

    close(fd);
    pthread_mutex_destroy(&mutex);

    return 0;
    
}