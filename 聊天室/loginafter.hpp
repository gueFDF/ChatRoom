#ifndef __LOGINAFTER__
#define __LOGINAFTER__
#define LOGIN "1"       //登录
#define REGSI "2"       //注册
#define TOMSG "3"       //发消息
#define ADDFREND "4"    //加好友
#define SEARFREND "5"   //查询好友信息
#define SHOWFREND "6"   //展示好友列表
#define LOGOUT "7"      //退出账号
#define ABOURT "8"      //退出私聊
#define READMESSAGE "9" //客户端去消息缓冲区读消息
#define HISTORY "10"    //查看历史消息
#define THREAD "11"  //客户端子线程
#define ISHAVEFRENDADD "12"  //客户端不断检查是否有好友添加
#include "login.hpp"
#include "work.hpp"
class logafter
{
private:
    User people;
    int socket;

public:
    int getsocket()
    {
        return socket;
    }
    User getuser()
    {
        return people;
    }

public:
    logafter(int fd) : socket(fd){};
    logafter(int fd, User &p) : people(p), socket(fd){};
    void addfrendc();                                        //添加好友(客户端)
    void addfrends();                                        //添加好友(服务器)
    void sendmsgc(vector<pair<string, User>> &myfrends);     //发送消息(客户端)
    void sendmsgs();                                         //发送消息(服务器)
    void searchfredc(vector<pair<string, User>> &myfrends);  //查询好友信息(客户端)
    void searchfreds();                                      //查询好友信息(服务器)
    void showfrendc(vector<pair<string, User>> &myfrends);   //展示好友列表(客户端)
    void showfrends();                                       //展示好友列表(服务器)
    void findhistoryc(vector<pair<string, User>> &myfrends); //查看历史消息记录(客户端)
    void findhistorys();                                     //查看历史消息记录(服务器)
    void exitaccountc();                                     //退出账号(客户端)
    void exitaccounts();                                     //退出账号(服务器)
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
        // sleep(1);
        // system("clear");
        User ret;
        recvMsg(socket, temp);
        ret.jsonparse(temp);
       // cout << "你已成功添------" << ret.getname() << "-------为好友" << endl;
       // cout << "他的账号为:  " << ret.getUID() << endl;
       cout<<"你已成功发出好友申请，等待"<<ret.getname()<<"的同意"<<endl;
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
    r.saddvalue("addfrend",UID);//加入消息缓冲区
    string p=UID+"addfrend";
    r.saddvalue(p,people.getUID());
    //r.saddvalue(people.getfrend(), UID);
    sendMsg(socket, json);
}

void logafter::sendmsgc(vector<pair<string, User>> &myfrends) //发送消息(客户端) 私人聊天
{
    int size = myfrends.size();
    // sleep(1);
    // system("clear");
    cout << "-------------------------------" << endl;
    for (int i = 1; i <= size; i++)
    {
        cout << i << "  .  " << myfrends[i - 1].second.getname() << " " << myfrends[i - 1].second.getUID() << endl;
    }
    cout << "-------------------------------" << endl;
    sendMsg(socket, TOMSG);
    cout << "请选择你要进行聊天的好友" << endl;
    int i;
    cin >> i;
    if (i <= 0 || i > myfrends.size())
    {
        cout << "输入错误,输入任意字符返回" << endl;
        string p;
        cin >> p;
        system("clear");
        return;
    }
    system("clear");
    cout << "                和" << myfrends[i - 1].second.getname() << "的聊天" << endl;
    cout << endl;
    string m;
    string json;
    //打印部分历史聊天记录
    message temp;
    //先发送历史聊天记录索引
    json = people.getUID() + myfrends[i - 1].second.getUID();
    sendMsg(socket, json);
    //接收历史聊天记录要打印的个数
    int ret = recvMsg(socket, json);
    if (ret == 0)
        return;
    int len = stoi(json);
    //打印历史消息记录
    for (int j = 0; j < len; j++)
    {
        recvMsg(socket, json);
        temp.josnparse(json);
        cout << "         " << temp.gettime() << endl;
        cout << temp.getname() << " : " << temp.getinfo() << endl;
    }
    cout << "        "
         << "\033[1m\033[33m以上为历史消息记录\033[0m" << endl;
    message mes(people.getname(), people.getUID(), myfrends[i - 1].second.getUID());
    pair<string, int> w(myfrends[i - 1].second.getUID(), socket);
    pthread_t tid;
    pthread_create(&tid, NULL, worker1, (void *)&w);
    while (1)
    {
        cout << people.getname() << " : ";
        cin >> m;
        if (m == "quit") //退出聊天
        {
            sendMsg(socket, ABOURT);
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
    redisReply **arr;
    Redis r; //打开数据库
    r.connect();
    int ret = 0;
    //打印四条历史聊天记录
    //先接收索引
    recvMsg(socket, json);
    int len = r.llen(json);
    //发送消息个数
    if (len <= 4)
    {
        sendMsg(socket, to_string(len));
    }
    else
    {
        len = 4;
        sendMsg(socket, to_string(len));
    }
    //发送历史消息记录
    if (len != 0)
        arr = r.lrange(json, "0", to_string(len - 1)); //从数据库中获取
    for (int i = len - 1; i >= 0; i--)
    {
        sendMsg(socket, arr[i]->str);
    }
    while (1)
    {
        ret = recvMsg(socket, json);
        if (json == ABOURT || ret == 0) //自己退出私聊
        {
            sendMsg(socket, ABOURT);
            return;
        }
        mms.josnparse(json);
        string UID = mms.getUIDto();
        if (!r.hashexists("islog", UID)) //如果对方不在线
        {
            // printf("不在线\n");
            string pp = mms.getUIDfrom() + mms.getUIDto(); //历史消息记录存放索引
            string pp2 = mms.getUIDto() + mms.getUIDfrom();
            r.lpush(pp, json);
            r.lpush(pp2, json);
        }
        else //如果对方在线，直接进行消息发送
        {
            //先获取套接字
            string ffd = r.gethash("islog", UID);
            int fd = stoi(ffd);
            sendMsg(fd, json);
            string pp = mms.getUIDfrom() + mms.getUIDto(); //历史消息记录存放索引
            string pp2 = mms.getUIDto() + mms.getUIDfrom();
            r.lpush(pp, json);
            r.lpush(pp2, json);
        }
    }
}

void logafter::showfrends() //查询好友信息(服务器)
{
    //链接数据库
    Redis r;
    r.connect();
    //接受要查询的好友个数
    string temp;
    int ret=recvMsg(socket, temp);
    if(ret==0)
    return ;
    int len = stoi(temp);
    for (int i = 0; i < len; i++) //循环查询好友在线状况
    {
        recvMsg(socket, temp); //接收好友账号
        //后面对是否在线进行查询
         ret = r.hashexists("islog", temp);
        if (ret == 0) //不在线
            sendMsg(socket, "0");
        else //在线
            sendMsg(socket, "1");
    }
}
void logafter::showfrendc(vector<pair<string, User>> &myfrends) //展示好友列表(客户端)
{
    system("clear");
    string temp;
    sendMsg(socket, SHOWFREND); //发送信号
    cout << "         " << people.getname() << "的好友列表" << endl;
    cout << "-------------------------" << endl;
    int len = myfrends.size();
    sendMsg(socket, to_string(len)); //发送好友数量
    for (int i = 0; i < len; i++)
    {
        sendMsg(socket, myfrends[i].second.getUID()); //发送好友账号
        //接受是否在线的信号
        recvMsg(socket, temp);
        if (temp == "0") //不在线
            cout << i + 1 << " " << myfrends[i].second.getname() << "("
                 << "\033[1m\033[30m"
                 << "*"
                 << "\033[0m"
                 << ")" << endl;
        else //在线
            cout << i + 1 << " " << myfrends[i].second.getname() << "("
                 << "\033[1m\033[32m"
                 << "*"
                 << "\033[0m"
                 << ")" << endl;
    }
    cout << "-------------------------" << endl;
    return;
}

void logafter::searchfredc(vector<pair<string, User>> &myfrends) //查询好友信息(客户端)
{
    system("clear");
    showfrendc(myfrends);
   // sendMsg(socket, SHOWFREND);
    cout << "请选择要查看信息的好友：" << endl;
    int i;
    cin >> i;
    if (i > myfrends.size() || i <= 0)
    {
        cout << "输入错误,输入任意字符返回" << endl;
        string p;
        cin >> p;
        system("clear");
        return;
    }
    i--;
    cout << "昵称 :   " << myfrends[i].second.getname() << endl;
    cout << "账号 :   " << myfrends[i].second.getUID() << endl;
    cout << "账号创建时间 : " << myfrends[i].second.getmytime() << endl;
    cout << endl;
    cout << "输入任意字符退出" << endl;
    string p;
    cin >> p;
    return;
}

void logafter::findhistoryc(vector<pair<string, User>> &myfrends) //查看历史消息记录(客户端)
{
    string temp;
    system("clear");
    sendMsg(socket, HISTORY); //发送信号
    int len = myfrends.size();
    cout << "          好友列表              " << endl;
    cout << "-------------------------------" << endl;
    for (int i = 0; i < len; i++)
    {
        cout << i + 1 << " : " << myfrends[i].second.getname() << endl;
    }
    cout << "-------------------------------" << endl;
    cout << "请选择你要查看的好友序号：";
    int i = 0;
    cin >> i;
    i--;
    //发送你要查看对象的UID
    sendMsg(socket, myfrends[i].second.getUID());
    int ret=recvMsg(socket, temp); //接受数量
    if(ret==0)
    return;
    len = stoi(temp);
    message mess;
    if (len == 0)
        cout << "你与该好友暂未聊天" << endl;
    else //循环打印
    {
        for (i = 0; i < len; i++)
        {
            recvMsg(socket, temp); //接受消息
            mess.josnparse(temp);
            cout << "     "
                 << "\033[1m\033[30m" << mess.gettime() << "\033[0m" << endl;
            cout << mess.getname() << " : " << mess.getinfo() << endl;
            cout << endl;
        }
    }
    cout << "输入任意字符退出" << endl;
    cin >> temp;
    return;
}
void logafter::findhistorys() //查看历史消息记录(服务器)
{
    Redis r;
    r.connect();
    string temp;
    recvMsg(socket, temp);         //接收要查询的UID
    temp = temp + people.getUID(); //索引
    cout << temp << endl;
    int len = r.llen(temp);          //查询聊天记录路的数量
    sendMsg(socket, to_string(len)); //发给客户端
    redisReply **arr = r.lrange(temp);
    //循环发送
    for (int i = len - 1; i >= 0; i--)
    {
        sendMsg(socket, arr[i]->str); //发送聊天记录
    }
}
#endif