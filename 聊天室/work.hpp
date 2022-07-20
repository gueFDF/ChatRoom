#ifndef __WORK__
#define __WORK__
#include"Sock.hpp"
#include"login.hpp"
#include "message.hpp"
void* worker1(void*arg)
{
    message mss;
    string json;
    int fd=*((int*)arg);
    while(1)
    {
        recvMsg(fd,json);
        if(json==ABOURT) //退出私聊
        {
            pthread_exit(NULL);//退出线程
            return NULL;
        }
        mss.josnparse(json);
        cout<<mss.getname()<<" : "<<mss.getinfo()<<endl;
        
    }
   

}
#endif