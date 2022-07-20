#ifndef __LOGINAFTER__
#define __LOGINAFTER__
#define LOGIN "1"     //登录
#define REGSI "2"     //注册
#define TOMSG "3"     //发消息
#define ADDFREND "4"  //加好友
#define SEARFREND "5" //查询好友信息
#define SHOWFREND "6" //展示好友列表
#define LOGOUT "7"    //退出账号
#define ABOURT "8"   //退出私聊
#include "login.hpp"
#include "work.hpp"
class logafter
{
private:
    User people;
    int socket;

public:
    logafter(int fd) : socket(fd){};
    logafter(int fd, User &p) : people(p), socket(fd){};
    void addfrendc();                                    //添加好友(客户端)
    void addfrends();                                    //添加好友(服务器)
    void sendmsgc(vector<pair<string, User>> &myfrends); //发送消息(客户端)
    void sendmsgs();                                     //发送消息(服务器)
    void searchfredc();                                  //查询好友信息(客户端)
    void searchfreds();                                  //查询好友信息(服务器)
    void showfrendc();                                   //展示好友列表(客户端)
    void showfrends();                                   //展示好友列表(服务器)
    void exitaccountc();                                 //退出账号(客户端)
    void exitaccounts();                                 //退出账号(服务器)
};
void logafter::addfrendc()
{
    sendMsg(socket, ADDFREND);
    string UID;
    string temp;
    cout << "请输入你要添加好友的账号：";
    cin >> UID;
    sendMsg(socket, UID);  //发送账号
    recvMsg(socket, temp); //接受信号
    if (temp == "-1")      //用户不存在
    {
        cout << "该用户不存在，无法添加好友" << endl;
        return;
    }
    else if (temp == "-2")
    {
        cout << "该用户已是你的好友，无法多次添加";
        return;
    }
    else
    {
        sleep(1);
        system("clear");
        User ret;
        recvMsg(socket, temp);
        ret.jsonparse(temp);
        cout << "你已成功添------" << ret.getname() << "-------为好友" << endl;
        cout << "他的账号为:  " << ret.getUID() << endl;
    }
}
void logafter::addfrends()
{
    Redis r;
    r.connect();
    string UID;
    string json;
    recvMsg(socket, UID);
    if (!r.hashexists("peopleinfo", UID)) //该用户不存在
    {
        sendMsg(socket, "-1"); //给客户端信号(好友不存在)
        return;
    }
    json = r.gethash("peopleinfo", UID);
    // people.jsonparse(json);
    if (r.sismember(people.getfrend(), UID))
    {
        sendMsg(socket, "-2"); //给客户端信号(该用户已是好友无法添加)
        return;
    }
    sendMsg(socket, "1"); //给客户端信号(好友存在)
    //判断对方是否在线
    // if(!r.sismember(""))
    r.saddvalue(people.getfrend(), UID);
    sendMsg(socket, json);
}

void logafter::sendmsgc(vector<pair<string, User>> &myfrends) //发送消息(客户端) 私人聊天
{
    int size = myfrends.size();
    sleep(1);
    system("clear");
    cout << "-------------------------------" << endl;
    for (int i = 1; i <= size; i++)
    {
        cout << i << "  .  " << myfrends[i - 1].second.getname() << endl;
    }
    cout << "-------------------------------" << endl;
    sendMsg(socket, TOMSG);
    cout << "请选择你要进行聊天的好友" << endl;
    int i;
    cin >> i;
    usleep(500000);
    system("clear");
    cout << "              " << myfrends[i - 1].second.getname() << endl;
    cout << endl;
    string m;
    string json;
    message mes(people.getname(), people.getUID(), myfrends[i-1].first);
    pthread_t tid;

    pthread_create(&tid, NULL, worker1, (void *)&socket);
    while (1)
    {
        cout<<people.getname()<<" : ";
        cin >> m;
        if (m == "quit") //退出聊天
        {
            sendMsg(socket,ABOURT);
            break;
        }
        mes.setinformation(m);
        json = mes.tojson();
        sendMsg(socket, json);
    }
}
void logafter::sendmsgs() //发送消息(服务器) 私人聊天
{
    message mms;
    string json;
    recvMsg(socket, json);
    if(json==ABOURT)  //对方退出私聊
    {
        sendMsg(socket,ABOURT);
        return;
    }
    mms.josnparse(json);
    string UID=mms.getUIDto();
    Redis r;//打开数据库
    r.connect();
    if(!r.hashexists("islog",UID))  //如果对方不在线
    {
        printf("不在线\n");
    }
    else //如果对方在线，直接进行消息发送
    {
            //先获取套接字
            string ffd=r.gethash("islog",UID);
            int fd=stoi(ffd);
            sendMsg(fd,json);
            string pp=mms.getUIDfrom()+mms.getUIDto();  //历史消息记录存放索引
            r.lpush(pp,json);

    } 
}
#endif