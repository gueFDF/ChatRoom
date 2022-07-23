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
        int ret = sendMsg(socket, people.getUID());
        string buf;
        recvMsg(socket, buf); //
        if (buf == ISHAVEFRENDADD)
        {
            cout << "收到一条好友添加申请" << endl;
        }
        else
        {
           // cout << "nofrend" << endl;
        }

        recvMsg(socket, buf);
        if (buf != "NO")
        {
            cout << "收到一条来自" << buf << "的消息" << endl;
        }
        else
        {
          //  cout << "nochat" << endl;
        }
        //读被删消息
        recvMsg(socket,buf);
        int len=stoi(buf);
        for(int i=0;i<len;i++)
        {
            recvMsg(socket,buf);
            cout<<"你已被"<<buf<<"删除,你们将不再是好友"<<endl;
        }
        
        sleep(1); //每隔一秒检查一次
    }
}
#endif