#ifndef __GROUP__CHAT__
#define __GROUP__CHAT__
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <map>
#include <vector>
#include "Userinfo.hpp"
#include "myredis.hpp"
#include "readwrite.hpp"
#include "loginmessage.hpp"
#include "Userinfo.hpp"
#include "loginafter.hpp"
#include "message.hpp"
#include "group.hpp"
class groupchat
{
private:
    User people;
    int socket;
    string joingroup;      //索引,我加入的
    string my_group;       //索引,我创建的
    string my_leadergroup; //索引，我管理的
public:
    groupchat(User p, int m_socket)
    {
        people = p;
        socket = m_socket;
        joingroup = "join" + p.getUID();
        my_group = "my_" + p.getUID();
        my_leadergroup = "leader" + p.getUID();
    }
    string getjoin()
    {
        return joingroup;
    }
    string getmygroup()
    {
        return my_group;
    }
    string getleader()
    {
        return my_leadergroup;
    }

public:
    void creategroupc();                        //群的创建(客户端)
    void creategroups();                        //群的创建(服务器)
    void showaddc(vector<group> &mygroup);      //展示我加入的群聊(客户端)
    void showmycreatc(vector<group> &mycreate); //我创建的群聊(客户端)
    void sjowmyctlc(vector<group> &myleader);   //展示我管理的群聊(客户端)
    void setadminc();                           //设置群管理(客户端)
    void setadmins();                           //设置群管理(服务器)
    void Flushc(vector<group> &mygroup, vector<group> &myleader,
                vector<group> &mycreate); //刷新(客户端)
    void Flushs();                        //刷新(服务器)
    void cltmygroups();                   //管理我的群
    void cltmygroupc();                   //管理我的群
    void addgroups();                     //加入群聊(服务器)
    void addgroupc();                     //加入群聊(客户端)
};

void groupchat::creategroupc() //群的创建(客户端)
{
    sendMsg(socket, CREATEGROUP);
    string name;
    cout << "请输入你要创建的群聊昵称：";
    cin >> name;
    group temp(name, people.getUID());
    string json = temp.tojson();
    sendMsg(socket, json);
    cout << "创建成功，该群的群号为:" << temp.getuid() << endl;
    cout << "输入任意字符返回" << endl;
    cin >> name;
    system("clear");
    return;
}
void groupchat::creategroups() //群的创建(服务器)
{
    Redis r;
    r.connect();
    string json;
    recvMsg(socket, json); //接收创建的这个群的资料
    group temp;
    temp.jsonprase(json);
    r.hsetValue("groupinfo", temp.getuid(), json);
    r.saddvalue(joingroup, temp.getuid());
    r.saddvalue(my_group, temp.getuid());
    r.saddvalue(my_leadergroup, temp.getuid());
    r.saddvalue(temp.getmember(), people.getUID());
    r.saddvalue(temp.getadm(), people.getUID());
}

void groupchat::showaddc(vector<group> &mygroup)
{
    if (mygroup.size() == 0)
    {
        cout << "你未加入任何群聊" << endl;
        cout << "输入任意字符退出" << endl;
        string pp;
        cin >> pp;
        system("clear");
        return;
    }
    cout << people.getname() << "加入的群聊" << endl;
    cout << "----------------------------" << endl;
    for (int i = 0; i < mygroup.size(); i++)
    {
        cout << i + 1 << " . " << mygroup[i].getname() << " (" << mygroup[i].getuid() << ")" << endl;
    }
    cout << "----------------------------" << endl;
    cout << "输入任意字符退出" << endl;
    string pp;
    cin >> pp;
    system("clear");
    return;
}

void groupchat::showmycreatc(vector<group> &mycreate) //我创建的群聊(客户端)
{
    if (mycreate.size() == 0)
    {
        cout << "你未加入任何群聊" << endl;
        cout << "输入任意字符退出" << endl;
        string pp;
        cin >> pp;
        system("clear");
        return;
    }
    cout << people.getname() << "创建的群聊" << endl;
    cout << "----------------------------" << endl;
    for (int i = 0; i < mycreate.size(); i++)
    {
        cout << i + 1 << " . " << mycreate[i].getname() << " (" << mycreate[i].getuid() << ")" << endl;
    }
    cout << "----------------------------" << endl;
    cout << "输入任意字符退出" << endl;
    string pp;
    cin >> pp;
    system("clear");
    return;
}
void groupchat::sjowmyctlc(vector<group> &myleader) //展示我管理的群聊(客户端)
{
    if (myleader.size() == 0)
    {
        cout << "你未加入任何群聊" << endl;
        cout << "输入任意字符退出" << endl;
        string pp;
        cin >> pp;
        system("clear");
        return;
    }
    cout << people.getname() << "管理的群聊" << endl;
    cout << "----------------------------" << endl;
    for (int i = 0; i < myleader.size(); i++)
    {
        cout << i + 1 << " . " << myleader[i].getname() << " (" << myleader[i].getuid() << ")" << endl;
    }
    cout << "----------------------------" << endl;
    cout << "输入任意字符退出" << endl;
    string pp;
    cin >> pp;
    system("clear");
    return;
}

void groupchat::Flushc(vector<group> &mygroup, vector<group> &myleader, vector<group> &mycreate)
{
    mygroup.clear();
    myleader.clear();
    mycreate.clear();
    string temp;
    sendMsg(socket, FLUSH);

    //我加入的
    recvMsg(socket, temp);
    int len = stoi(temp);
    for (int i = 0; i < len; i++)
    {
        group p;
        recvMsg(socket, temp);
        p.jsonprase(temp);
        mygroup.push_back(p);
    }

    //我管理的群聊
    recvMsg(socket, temp);
    len = stoi(temp);
    for (int i = 0; i < len; i++)
    {
        group p;
        recvMsg(socket, temp);
        p.jsonprase(temp);
        myleader.push_back(p);
    }
    //我创建的群聊
    recvMsg(socket, temp);
    len = stoi(temp);
    for (int i = 0; i < len; i++)
    {
        group p;
        recvMsg(socket, temp);
        p.jsonprase(temp);
        mycreate.push_back(p);
    }
}
void groupchat::Flushs()
{
    Redis r;
    r.connect();
    //我加入的
    int len = r.scard(joingroup);
    sendMsg(socket, to_string(len));
    if (len != 0)
    {
        redisReply **arr = r.smembers(joingroup);
        for (int i = 0; i < len; i++)
        {
            string json = r.gethash("groupinfo", arr[i]->str);
            sendMsg(socket, json);
        }
    }

    //我管理的群聊
    len = r.scard(my_leadergroup);
    sendMsg(socket, to_string(len));
    if (len != 0)
    {
        redisReply **arr = r.smembers(my_leadergroup);
        for (int i = 0; i < len; i++)
        {
            string json = r.gethash("groupinfo", arr[i]->str);
            sendMsg(socket, json);
        }
    }
    //我创建的群聊
    len = r.scard(my_group);
    sendMsg(socket, to_string(len));
    if (len != 0)
    {
        redisReply **arr = r.smembers(my_group);
        for (int i = 0; i < len; i++)
        {
            string json = r.gethash("groupinfo", arr[i]->str);
            sendMsg(socket, json);
        }
    }
}

void groupchat::cltmygroups() //管理我的群
{
}
void groupchat::cltmygroupc() //管理我的群
{
}
void groupchat::addgroups() //加入群聊(服务器)
{
    Redis r;
    r.connect();
    string temp;
    recvMsg(socket, temp); //接收所要加的群id
    if (!r.hashexists("groupinfo", temp))
    {
        sendMsg(socket, "-1"); // 账号不存在
        return;
    }
    if (r.sismember(joingroup, temp))
    {
        sendMsg(socket, "-2"); // 已加入
        return;
    }
    sendMsg(socket, "1");
    //存入数据库等待群管理
    string json = r.gethash("groupinfo", temp);
    group p;
    p.jsonprase(json);
    r.saddvalue("ifadd" + p.getuid(), people.getUID());
    //等待管理员或群主同意
}
void groupchat::addgroupc() //加入群聊(客户端)
{
    sendMsg(socket, ADDGROUP);
    system("clear");
    cout << "输入你要加入的群ID: ";
    string temp;
    cin >> temp;
    sendMsg(socket, temp); //发送所要加的群id
    recvMsg(socket, temp); //接受结果响印
    if (temp == "-1")
    {
        cout << "该群不存在" << endl;
    }
    else if (temp == "-2")
    {
        cout << "你已是该群成员" << endl;
    }
    else
    {
        cout << "请求已成功发出,等待同意" << endl;
    }
    cout << "输入任意字符退出" << endl;
    string pp;
    cin >> pp;
    system("clear");
    return;
}
#endif