#ifndef __LOGIN_HANDLER_H__
#define __LOGIN_HANDLER_H__
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include "Userinfo.hpp"
#include "myredis.hpp"
#include "readwrite.hpp"
#include "loginmessage.hpp"
#include "Userinfo.hpp"
#include "loginafter.hpp"
using namespace std;


void afterloginc(int fd, User tem);
void afterlogins(int fd);
void menulogin()
{
    cout << "--------------------------------------" << endl;
    cout << "-------------聊天室--------------------" << endl;
    cout << "-------------0.退出--------------------" << endl;
    cout << "-------------1.登录--------------------" << endl;
    cout << "-------------2.注册--------------------" << endl;
}

void messagemenu()
{
    cout << "-----------------------------------------" << endl;
    cout << "----------------1.发消息------------------" << endl;
    cout << "----------------2.加好友------------------" << endl;
    cout << "----------------3.查询好友信息------------------" << endl;
    cout << "----------------4.查看好友列表-------------" << endl;
    cout << "----------------5.注销账号----------------" << endl;
}

void regclier(int socket) //注册账号(客户端)
{
    sendMsg(socket, REGSI);
    User peope;
    string p;
    string temp;
    cout << "请输入邮箱:";
    cin >> p;
    peope.settele(p);
    cout << "请输入昵称: ";
    cin >> p;
    peope.setname(p);
flag:
    cout << "请输入密码:";
    cin >> p;
    cout << "请再次输入密码:";
    cin >> temp;
    if (temp != p)
    {
        cout << "两次密码输入不一致" << endl;
        goto flag;
    }
    peope.setpassword(temp);
    temp = peope.tojson();
    sendMsg(socket, temp);
    recvMsg(socket, temp);
    cout << "账号注册成功" << endl;
    cout << "你的账号为:" << temp << endl;
    return;
}

void regser(void *arg) //注册账号(服务器)
{
    struct epoll_event temp;
    int *arr = (int *)arg;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN;
    epoll_ctl(efd, EPOLL_CTL_DEL, fd, &temp); //从树上去除该套接字

    string buf;
    recvMsg(fd, buf);
    User people;
    people.jsonparse(buf);
    string UID = people.getUID();
    Redis r;
    r.connect();
    r.hsetValue("peopleinfo", UID, buf);
    r.saddvalue("info", UID);
    sendMsg(fd, UID);
    epoll_ctl(efd, EPOLL_CTL_DEL, fd, &temp); //再添加回树上
}

int logincli(int fd, User &people) //登录(客户端)
{
    sendMsg(fd, LOGIN);
    string UID;
    string password;
    cout << "请输入你的账号: ";
    cin >> UID;
    cout << "请输入你的密码: ";
    cin >> password;
    loginmessage p(UID, password);
    string buf = p.tojson();
    cout << buf << endl;
    sendMsg(fd, buf);
    recvMsg(fd, buf);
    if (buf == "-2")
    {
        cout << "账号错误,请重新输入" << endl;
        return 0;
    }
    else
    {
        if (buf == "-1")
        {
            cout << "密码错误,请重新输入" << endl;
            return 0;
        }
        else
        {
            cout << "登录成功" << endl;
            recvMsg(fd, buf);
            people.jsonparse(buf);
        }
    }
    return 1;
}

void loginser(void *arg) //登录(服务器)
{
    struct epoll_event temp;
    int *arr = (int *)arg;
    int fd = arr[0];
    int efd = arr[1];
    temp.data.fd = fd;
    temp.events = EPOLLIN;
    epoll_ctl(efd, EPOLL_CTL_DEL, fd, &temp);
    User people;
    Redis r;
    r.connect();
    string buf;
    recvMsg(fd, buf);
    cout << buf << endl;
    loginmessage p;
    p.josnparse(buf);
    string UID = p.getUID();
    string password = p.getpassword();
    if (!r.sismember("info", UID)) //如果该账号不存在
    {
        sendMsg(fd, "-2");
        epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp);
        return;
    }
    else //账号存在，下面进行密码匹配
    {
        string json;
        json = r.gethash("peopleinfo", UID);
        people.jsonparse(json);
        if (people.getpassword() != password) //密码错误
        {
            sendMsg(fd, "-1");
            epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp);
            return;
        }
        else
        {
            sendMsg(fd, "1");
            //改变用户状态
            people.setislog(fd);
            sendMsg(fd, json);
        }
    }
    afterlogins(fd);
    // epoll_ctl(efd, EPOLL_CTL_ADD, fd, &temp);
}

//登录后操作(用户)
void afterloginc(int fd, User tem)
{
    string temp;
    logafter people(fd, tem);
    cout << "请输入你的选择:" << endl;
    int p;
    cin>>p;
    do
    {

        switch (p)
        {
        case 1:
            break;
        case 2:
            people.addfrendc();
            break;
        case 3:
            break;
        case 0:
            cout << "退出成功" << endl;
        default:
            cout << "输入错误" << endl;
        }
    } while (0);
}
void afterlogins(int fd)
{
    string temp;
    logafter tt(fd);
    do
    {
        recvMsg(fd, temp);
        if (temp == ADDFREND) //添加好友
        {
            tt.addfrends();
        }

    } while (/*temp != LOGOUT*/0);
}
#endif