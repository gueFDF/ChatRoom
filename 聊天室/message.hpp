#ifndef __MESSAGE_HANDLER_H__
#define __MESSAGE_HANDLER_H__

#include <iostream>
#include <string>
#include <cstdlib>
#include <json/json.h>
#include <ctime>
#include <cstdio>
using namespace std;
using namespace Json;
struct message
{
private:
    string name;        //发送者昵称
    string UIDfrom;     //原地址
    string UIDto;       //目标地址
    string information; //消息内容
    string my_time;     //时间
    string groupname;   //群聊昵称(若不是群聊，此字段设置为1)
public:
    message(string name1, string UIDfrom1, string UIDto1 = "", string p = "1") : name(name1), UIDfrom(UIDfrom1), UIDto(UIDto1), groupname(p){};
    message(){};

    string getname()
    {
        return name;
    }
    void setgroupname(string gpname)
    {
        groupname = gpname;
    }
    string getgroupname()
    {
        return groupname;
    }
    void setUIDto1(string gpname)
    {
        UIDto = gpname;
    }
    string getinfo()
    {
        return information;
    }
    string gettime()
    {
        return my_time;
    }
    string getUIDfrom()
    {
        return UIDfrom;
    }
    string getUIDto()
    {
        return UIDto;
    }
    string getmtime()
    {
        time_t rawtime;
        struct tm *ptm;
        time(&rawtime);
        ptm = localtime(&rawtime);
        char data[60];
        sprintf(data, "%d-%02d-%02d-%02d:%02d:%02d", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
        move(string(data));
        return data;
    }
    void setinformation(string p)
    {
        information = p;
    }
    string tojson()
    {
        my_time = getmtime();
        Value root;
        root.append(name);
        root.append(UIDfrom);
        root.append(UIDto);
        root.append(information);
        root.append(my_time);
        root.append(groupname);
        FastWriter f;
        string json = f.write(root);
        return json;
    }
    void josnparse(string json)
    {
        Value root;
        Reader r;
        r.parse(json, root);
        int i = 0;
        name = root[i++].asString();
        UIDfrom = root[i++].asString();
        UIDto = root[i++].asString();
        information = root[i++].asString();
        my_time = root[i++].asString();
        groupname = root[i++].asString();
    }
};
#endif