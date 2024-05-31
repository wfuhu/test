#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h> //取消接受文件cfd阻塞
#include <error.h>
#include <cerrno>
#include <pthread.h>

typedef struct socketinfo
{
    int fd;
    int epfd;
} Socketinfo;

void *accpetConn(void *arg)
{

    Socketinfo *info = (Socketinfo *)arg;
    int cfd = accept(info->fd, NULL, NULL);

    // 设置非阻塞
    int flag = fcntl(cfd, F_GETFL);

    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = cfd;
    epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);
    free(info);
    return NULL;
}

void *communication(void *arg)
{
    Socketinfo *info = (Socketinfo *)arg;
    char buff[1024];
    //char temp[1024] 创建字符窜存储接收完整字符窜
    //memset(temp,sizeof(temp))
    while (1)
    {
        int len = recv(info->fd, buff, sizeof(buff), 0);
        
        if (len == -1)
        {
            if (errno == EAGAIN)
            {
                send(info->fd,buff,strlen(buff)+1,0);
                break;
            }
            else
            {
                perror("");
                exit(1);
            }
        }
        else if (len == 0)
        {
            epoll_ctl(info->epfd, EPOLL_CTL_DEL, info->fd, NULL);
            printf("worry");
            close(info->fd);
            break;
        }
        //strncat(temp+strlen(temp),buff,len);
        printf("%s",buff);
        write(STDOUT_FILENO, buff, len);
    }
    free(info);
    return NULL;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0); // 监听
    if (fd == -1)
    {
        perror("socket");
        return -1;
    }
    // 绑定本地ip
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr)); // 】】】置0
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(10000);       // 端口网络大端转换电脑小端
    saddr.sin_addr.s_addr = INADDR_ANY; // 实际为0 0=0.0.0.0  本地网卡地址

    int saddr_size = sizeof(saddr);
    int ret = bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)); // 绑定ip地址
    if (ret == -1)
    {
        /* code */
        perror("bind");
        return -1;
    }
    if (listen(fd, 128) == -1) // 设置监听最大数
    {
        perror("listen");
        return -1;
    }
    // epoll 实例
    int epfd = epoll_create(1);
    if (epfd == -1)
    {
        perror("epfd_create");
        return -1;
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(evs[0]);
    while (1)
    {
        int num = epoll_wait(epfd, evs, size, -1);
        pthread_t tid;
        for (int i = 0; i < num; ++i)
        {
            Socketinfo* info=(Socketinfo*)malloc(sizeof(Socketinfo));
           
            int fd1 = evs[i].data.fd;
            info->fd=fd1;
            info->epfd=epfd;
            if (fd1 == fd)
            {
                pthread_create(&tid,NULL,accpetConn,info);
                pthread_detach(tid);
            }
            else
            {
                pthread_create(&tid,NULL,communication,info);
                pthread_detach(tid);
            }
        }
    }
    close(fd);

    return 0;
}