#ifndef __WORK__
#define __WORK__
#include "Sock.hpp"
#include "login.hpp"
#include "message.hpp"
void *worker1(void *arg)
{
    pair<string, int> p = *(pair<string, int> *)arg;
    message mss;
    string json;
    int fd = p.second;
    string UIDfrend = p.first;
    while (1)
    {
        recvMsg(fd, json);
        if (json == ABOURT) //退出私聊
        {
            pthread_exit(NULL); //退出线程
            return NULL;
        }
        mss.josnparse(json);
        if (mss.getUIDfrom() == UIDfrend)
            cout << mss.getname() << " : " << mss.getinfo() << endl;
        else
            cout << "\033[1m\033[31m"
                 << "           "
                 << "收到一条来自" << mss.getname() << "的一条消息"
                 << "\033[0m" << endl;
    }
}

void *worker2(void *arg)
{
    User people = *(User *)arg;
    int socket = Sock::Socket();
    Sock::Connect(socket, "127.0.0.1", 9998);
    while (1)
    {
        sendMsg(socket, THREAD);
        sendMsg(socket, people.getUID());
        string buf;
        recvMsg(socket, buf);
        if (buf == ISHAVEFRENDADD)
        {
            cout << "收到一条好友添加申请" << endl;
            //从容器中去掉
        }
        recvMsg(socket, buf);
        sleep(1);//每隔一秒检查一次
    }
}
#endif