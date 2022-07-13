#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__
#include <iostream>

//小火车协议
typedef struct train_s{
    size_t length;
    char buf[65535];
}train_t;

class SocketIO
{
public:
    explicit SocketIO(int fd);
    ~SocketIO();
    int readn(char *buf, int len);
    int readLine(char *buf, int len);
    int writen(const char *buf, int len);

    int recvMsg(char *buf);
    int sendMsg(const char *buf);

private:
    int _fd;

};

#endif
