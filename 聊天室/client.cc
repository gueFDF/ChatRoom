#include "Sock.hpp"
#include "login.hpp"
int main()
{
    int socket = Sock::Socket();
    Sock::Connect(socket, "127.0.0.1", 9997);
    int p;
    int ret;
    User people; //存放该用户的所有信息
    string temp;
    do
    {
        menulogin();
        cin >> p;
        switch (p)
        {
        case 1:
            temp = "1";
            sendMsg(socket, temp);
            ret=logincli(socket, people);
            if(!ret)
            break;
            messagemenu();
            break;
        case 2:
            regclier(socket);
            break;
        case 3:
            temp = "3";
            sendMsg(socket, temp);
            break;
        case 0:
            cout<<"退出成功"<<endl;
        default:
            cout<<"选择错误，请重新选择"<<endl;
            break;
        }
    } while (p);
}