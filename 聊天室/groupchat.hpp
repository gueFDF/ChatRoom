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
                vector<group> &mycreate);      //刷新(客户端)
    void Flushs();                             //刷新(服务器)
    void cltmygroups();                        //管理我的群
    void cltmygroupc(vector<group> &myleader); //管理我的群
    void addgroups();                          //加入群聊(服务器)
    void addgroupc();                          //加入群聊(客户端)
    void agreeadds(group &g);                  //同意申请(服务器)
    void agreeaddc();                          //拒绝申请
    void delgrouppeoples();                    //踢人(服务器)
    void delgrouppeoplec();                    //踢人(客户端)
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

void groupchat::addgroups() //加入群聊(服务器)
{
    Redis r;
    r.connect();
    string temp;
    recvMsg(socket, temp); //接收所要加的群id
    cout << temp << endl;
    cout << r.hashexists("groupinfo", temp) << endl;
    if (r.hashexists("groupinfo", temp))
    {
        // q群存在
        //  cout<<"sfdsdfsdfsd"<<endl;
        //  sendMsg(socket, "-1"); // 账号不存在
        //  return;
        if (!r.sismember(joingroup, temp))
        {
            // sendMsg(socket, "-2"); // 已加入
            // return;
            sendMsg(socket, "1");
            //存入数据库等待群管理
            string json = r.gethash("groupinfo", temp);
            group p;
            p.jsonprase(json);
            r.saddvalue("ifadd" + p.getuid(), people.getUID());
        }
        else
        {
            sendMsg(socket, "-2"); // 已加入
            // return;
        }
    }
    else
    {
        cout << "sfdsdfsdfsd" << endl;
        sendMsg(socket, "-1"); // 账号不存在
        // return;
    }
    // if (r.sismember(joingroup, temp))
    // {
    //     sendMsg(socket, "-2"); // 已加入
    //     return;
    // }
    // cout << "aaaaaaaaaaaaaaaaaaaaaaaa" << endl;
    // sendMsg(socket, "1");
    // //存入数据库等待群管理
    // string json = r.gethash("groupinfo", temp);
    // group p;
    // p.jsonprase(json);
    // r.saddvalue("ifadd" + p.getuid(), people.getUID());
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
        cout << temp << endl;
        cout << "请求已成功发出,等待同意" << endl;
    }
    cout << "输入任意字符退出" << endl;
    string pp;
    cin >> pp;
    system("clear");
    return;
}

void groupchat::cltmygroups() //管理我的群
{
    cout << "12313142342343254345" << endl;
    group p;
    string json;
    sendMsg(socket, json);
    cout << json << endl;
    cout << "12313142342343254345" << endl;
    p.jsonprase(json);
    string sel;
    int ret;
    do
    {
        ret = recvMsg(socket, sel);
        if (sel == DELGROUPPEOPLE)
        {
        }
        if (sel == AGREEADD)
        {
            agreeadds(p);
        }
    } while (sel != LOGOUT && ret != 0);
}
void groupchat::cltmygroupc(vector<group> &myleader) //管理我的群
{
    sendMsg(socket, CTLMYGROUP);
    system("clear");
    cout << people.getname() << "的聊天室" << endl;
    //打印自己的所管理的群
    cout << "---------------------------" << endl;
    for (int i = 0; i < myleader.size(); i++)
    {
        cout << i + 1 << " . " << myleader[i].getname() << endl;
    }
    cout << "---------------------------" << endl;
    cout << "请选择你要管理的群:";
    int i;
    cin >> i;
    i--;
    sendMsg(socket, myleader[i].tojson()); //解析成json串发送给服务器
    cout << myleader[i].tojson() << endl;
    // system("clear");

    int sel;
    do
    {
        cout << "      1.处理入群申请" << endl;
        cout << "      2.移出群里的用户" << endl;
        cout << "      0.返回上一页" << endl;
        cout << "请输入你的选择:";
        cin >> sel;
        switch (sel)
        {
        case 1:
            agreeaddc();
            break;
        case 2:
            break;
        case 0:
            sendMsg(socket, LOGOUT);
            break;
        }

    } while (sel);
}

void groupchat::agreeaddc()
{
    sendMsg(socket, AGREEADD);
    string buf;
    recvMsg(socket, buf);
    cout << buf << " 374" << endl;
    int len = stoi(buf);
    if (len == 0)
    {
        cout << "暂无入群申请" << endl;
    }
    else
    {
        for (int i = 0; i < len; i++)
        {
            cout << buf << " 3857" << endl;
            recvMsg(socket, buf);
            cout << buf << " 385" << endl;
            cout << "收到" << buf << "的入群申请" << endl;
            cout << "请做出选择[YES/NO]:";
        flag:
            cin >> buf;
            if (buf != "YES" && buf != "NO")
            {
                cout << "\033[1m\033[31m\033[11m警告输入错误,只能输入YES或NO\033[0m" << endl;
                cout << "请重新输入:" << endl;
                goto flag;
            }
            sendMsg(socket, buf);
            if (buf == "YES")
            {
                string json;
                group temp;
                cout << "添加成功" << endl;
            }
            else
            {
                cout << "添加失败" << endl;
            }
        }
        cout << "处理完毕，输入任意字符返回" << endl;
        cin >> buf;
        system("clear");
    }
}
void groupchat::agreeadds(group &g)
{
    cout << "dfsdfsdfs" << endl;
    Redis r;
    r.connect();
    int ret = r.scard("ifadd" + g.getuid());
    cout << g.getuid() << endl;
    cout << "ret:" << ret << endl;
    sendMsg(socket, to_string(ret));
    string buf;
    User temp;
    if (ret != 0)
    {
        redisReply **arr = r.smembers("ifadd" + g.getuid());
        for (int i = 0; i < ret; i++)
        {
            buf = r.gethash("peopleinfo", arr[i]->str);
            temp.jsonparse(buf);
            sendMsg(socket, temp.getname()); //发送名字
            cout << temp.getname() << endl;
            recvMsg(socket, buf); //接收对面的选择
            if (buf == "NO")
            {
                //将数据删除
                r.sremvalue("ifadd" + g.getuid(), temp.getUID());
            }
            else //同意
            {
                //添加数据
                r.saddvalue("my_" + temp.getUID(), temp.getUID());
                r.saddvalue(g.getmember(), temp.getUID());
                //将数据删除
                r.sremvalue("ifadd" + g.getuid(), temp.getUID());
            }
        }
    }
}
#endif