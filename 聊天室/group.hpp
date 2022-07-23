#ifndef __GROUP__
#define __GROUP__
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

class group
{
private:
    string g_name;
    string g_uid;
    string administrators; //管理员索引
    string UID;            //群主UID
    string groupmembers;   //群成员
public:
    group(string name = "", string uid = "")
    {
        g_name=name;
        UID = uid;
        time_t timestamp;
        time(&timestamp);
        g_uid = to_string(timestamp);
        g_uid[0] = '5';
        administrators = g_uid + "ct";
        groupmembers = g_uid + "members";
    }
    string getname()
    {
        return g_name;
    }
    string getuid()
    {
        return g_uid;
    }
    string getadm()
    {
        return administrators;
    }
    string getgleaderUID()
    {
        return UID;
    }
    string getmember()
    {
        return groupmembers;
    }

public:
    void viewownerc(); //查看群主
    void viewowners();
    void viewleaderc(); //查看群管理
    void viewleaders();
    void viewmemberc(); //查看群成员
    void viewmembers();
    void sendinfos(); //发消息
    void sendinfoc(); //发消息
public:
    string tojson()
    {
        Value root;
        root.append(g_name);
        root.append(g_uid);
        root.append(administrators);
        root.append(UID);
        root.append(groupmembers);
        FastWriter f;
        string josn=f.write(root);
        return josn;
    }
    void jsonprase(string json)
    {
        Value root;
        Reader r;
        r.parse(json,root);
        int i=0;
        g_name=root[i++].asString();
        g_uid=root[i++].asString();
        administrators=root[i++].asString();
        UID=root[i++].asString();
        groupmembers=root[i++].asString();
    }
};

#endif