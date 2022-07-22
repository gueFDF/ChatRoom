#include <iostream>
#include "hiredis/hiredis.h"
using namespace std;
int main()
{
    /*string name ;
     redisContext* r;
     redisReply* p;
     r=redisConnect("127.0.0.1",6379);
     p=(redisReply*)redisCommand(r,"lrange mylist 0 0");
     cout<<p->element[0]->str<<endl;*/
    cout << "\033[1m\033[30m"
         << "楚梦凡" << endl;
    cout<<"\033[1m\033[33m"<<"楚梦凡"<<"\033[0m"<<endl;
    //cin>>name;
    cout << "\033[1m\033[31m\033[11m"
         << "楚梦凡" << endl;
    cout << "\033[1m\033[32m\033[12m"
         << "楚梦凡" << endl;
    cout << "\033[1m\033[33m\033[13m"
         << "楚梦凡" << endl;
    cout << "\033[1m\033[34m\033[14m"
         << "楚梦凡" << endl;
    cout << "\033[1m\033[35m\033[15m"
         << "楚梦凡" << endl;
    cout << "\033[1m\033[36m\033[16m"
         << "楚梦凡" << endl;
         char a='*';
          cout<<a<<endl;
         cout<<"\033[1m\033[32m\033[12m"<<a<<endl;
}