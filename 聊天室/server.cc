#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <map>
#include "Sock.hpp"
#include "myredis.hpp"
#include "login.hpp"
#include "threadpool.hpp"
#define PORT 9998
#define LISTEN 1024
#define EPOLL 1024
int main()
{

    map<string, int> mmap;
    //创建一个线程池
    ThreadPool threadpool(10); //创建一个有十个线程的线程池
    int i = 0, cfd;
    int ret = 1; //接收返回值
    string buf;
    int sockfd = Sock::Socket();
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (void *)&opt, sizeof(int)); //端口复用
    Sock::Bind(sockfd, PORT);
    Sock::Listen(sockfd, LISTEN);
    int efd = epoll_create(EPOLL);
    struct epoll_event temp, ep[EPOLL];
    temp.data.fd = sockfd;
    temp.events = EPOLLIN;

    epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &temp);
    while (1)
    {
        ret = epoll_wait(efd, ep, EPOLL, -1);
        for (i = 0; i < ret; i++)
        {
            if (ep[i].data.fd == sockfd) //客户端请求链接
            {
                cfd = Sock::Accept(sockfd);
                sleep(3);
                temp.data.fd = cfd;
                temp.events = EPOLLIN;
                //设置非阻塞
                /*
                int flag;
                flag = fcntl(cfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(cfd, F_SETFL, flag); //设置非阻塞*/
                epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &temp);
                cout<<"对方已连接"<<endl;
            }
            else
            {

                recvMsg(ep[i].data.fd, buf);
                if (buf == LOGIN) //有人登录
                {
                    int arg[] = {ep[i].data.fd, efd};
                    Task task;
                    task.function = loginser;
                    task.arg = arg;
                    threadpool.addTask(task);
                }
                else if (buf == REGSI) //有人要注册账号
                {
                    int arg[] = {ep[i].data.fd, efd};
                    Task task;
                    task.function = regser;
                    task.arg = arg;
                    threadpool.addTask(task);
                }
                else if (buf == THREAD) //读取缓冲区消息
                {
                    int arg[] = {ep[i].data.fd, efd};
                    history(arg);
                }
            }
        }
    }
}
