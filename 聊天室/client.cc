#include "Sock.hpp"
#include "login.hpp"
int main()
{
    int socket = Sock::Socket();
    Sock::Connect(socket, "127.0.0.1", 9999);
    int p;
    int ret;
    User people; //存放该用户的所有信息
    string temp;
    do
    {
         system("clear");
        menulogin();
        cin >> p;
        switch (p)
        {
        case 1:

            ret=logincli(socket, people);
            if(!ret)
            break;
            afterloginc(socket,people);
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