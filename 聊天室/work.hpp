#ifndef __WORK__
#define __WORK__
#include"Sock.hpp"
#include"login.hpp"
#include "message.hpp"
void* worker1(void*arg)
{
    pair<string,int>p=*(pair<string ,int>*)arg;
    message mss;
    string json;
    int fd=p.second;
    string UIDfrend=p.first;
    while(1)
    {
        recvMsg(fd,json);
        if(json==ABOURT) //退出私聊
        {
            pthread_exit(NULL);//退出线程
            return NULL;
        }
        mss.josnparse(json);
        if(mss.getUIDfrom()==UIDfrend)
        cout<<mss.getname()<<" : "<<mss.getinfo()<<endl;
        else
        cout<<"\033[1m\033[31m"<<"           "<<"收到一条来自"<<mss.getname()<<"的一条消息"<<"\033[0m"<<endl;
        
    }
   

}
#endif