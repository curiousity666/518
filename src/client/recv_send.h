#ifndef RECVSEND_H
#define RECVSEND_H

#include "head.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>

using nlohmann::json;
using namespace std;



typedef struct train_s{
    size_t length;
    char buf[65535];
}train_t;

int readn(char *buf,int len,int _fd)
{
    int left = len;//left是leave的过去式，表示余下的，未使用的
    char *pstr = buf;
    int ret = 0;
    while (left > 0)
    {
        ret = read(_fd,pstr,left);
        if(ret == -1 && errno == EINTR)//????
        {
            continue;
        }
        else if(ret == -1)
        {
            perror("read error -1");
            return len - ret;//异常读取，返回已读取字符的数量
        }
        else if(ret == 0)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }

    return len - left;//正常运行到处，left=0
}

int recvMsg1(int _fd,char *buf)
{
    train_t t = {0};
    read(_fd,&t.length,sizeof(size_t));
    readn(t.buf,t.length,_fd);
    strcpy(buf,t.buf);
    return t.length;
}

json recvMsg2(int _fd)
{
    char buff[65535] = {0};
    recvMsg1(_fd,buff);
    return json::parse(buff);
}


int sendMsg1(int fd,const char *buf)
{
    train_t t = {0};
    t.length = strlen(buf);
    if(t.length > 65535)
    {
        perror("数据长度超过65535,发送失败");
        return -1;
    }
    strcpy(t.buf,buf);
    send(fd,&t,t.length+sizeof(size_t),MSG_NOSIGNAL);
    return t.length; 
}


void sendMsg2(int fd,const json &msg)
{
    string buf = msg.dump();
    sendMsg1(fd,buf.c_str());
}


#endif