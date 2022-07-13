#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <functional>

using nlohmann::json;
using std::shared_ptr;
using std::function;

class EventLoop;

class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
public:
    TcpConnection(int fd, EventLoop *loop);
    ~TcpConnection();

    //接收json数据
    json recvJson();
    //接收一行字符串
    string recvLine();
    //发送字符串
    void sendMsg(const string &msg);
    //发送json数据
    void sendJson(const json &msg);

    void sendInLoop(const json &msg);

    //输出连接信息
    string toString();

    bool isClosed() const;

    //注册三个事件的回调函数
    void setConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);

    //三个事件回调函数的执行
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    EventLoop *_loop;
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;


};

#endif
