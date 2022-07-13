#include "recv_send.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

using nlohmann::json;
using namespace std;


int main(){
    int sock_fd = socket(AF_INET,SOCK_STREAM,0);  //第三个参数填0，会根据第二个参数自动获取是TCP，还是UDP
    ERROR_CHECK(sock_fd,-1,"socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);  //端口号是1024~65535，16位两个字节
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //将点分十进制转换为32位无符号整数

    //连接服务端地址//第一次握手，可以用tcpdump抓包验证
    int ret = connect(sock_fd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connnect");


    json json_2_send = {
        {"mode", 0},
        {"keyWord", ""}
    };
    json json_2_recv;
    string first;
    string second;
    string query;

    while(1)
    {
        // cin.sync();
        // 输入模式：
        // 数字 + 空格 + 要查询的关键词
        cout << "请输入（数字 + 空格 + 要查询的关键词）：" << endl;
        // getline(cin,query);
        // istringstream iss(query);
        cin>>first;
        cin>>second;
        cout << endl;
        int mode = atoi(first.c_str());

        // int mode = 2;
        // second = "租房";

        json_2_send["mode"] = mode;
        json_2_send["keyWord"] = second;


        sendMsg2(sock_fd,json_2_send);

        // sleep(1);

        if(1 == mode)
        {
            json_2_recv = recvMsg2(sock_fd);
            cout << "推荐的关键词：" << endl;
            cout << json_2_recv["result"] << endl << endl;;
        }
        else if(2 == mode)
        {
            while (1)
            {
                json_2_recv = recvMsg2(sock_fd);
                if("null" == json_2_recv["result"])
                {
                    cout << "查询结束" << endl;
                    break;
                }
                cout << json_2_recv["result"] << endl << endl;
            }

            cout << "是否继续查询？（y/n）" << endl;
            string tmp;
        Loop:
            cin >> tmp;
            if("y" == tmp)
            {
                continue;;
            } 
            else if("n" == tmp)
            {
                break;
            }
            else
            {
                cout << "非法字符，请重新输入" << endl;
                goto Loop;
            }
        }
    }

    close(sock_fd);
    return 0;
}