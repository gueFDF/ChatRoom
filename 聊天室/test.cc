#include<iostream>
#include"hiredis/hiredis.h"
using namespace std;
int main()
{
    redisContext* r;
    redisReply* p;
    r=redisConnect("127.0.0.1",6379);
    p=(redisReply*)redisCommand(r,"lrange mylist 0 0");
    cout<<p->element[0]->str<<endl;

}