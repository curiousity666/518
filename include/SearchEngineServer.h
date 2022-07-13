/*
 * @Author: Channing
 ⊙)…e
 * @Date: 2022-07-01 10:18:28
 * @LastEditTime: 2022-07-01 14:49:13
 * @FilePath: /wangdao/SearchEngine.1.1/include/SearchEngineServer.h
 * @Description: 
 * Copyright (c) 2022 by Channing 995825312@qq.com, All Rights Reserved. 
 */
 #ifndef SEARCHENGINESERVER_H
#define SEARCHENGINESERVER_H

#include "WebPageSearcher.h"
#include "KeyRecommander.hpp"
#include "TcpServer.h"
#include "ThreadPool.h"
#include <iostream>
#include <string>

using namespace std;

class SearchEngineServer
{
public:
    SearchEngineServer(const string &ip,
                       unsigned short port,
                       size_t threadNum = 5,
                       size_t queSize = 10)
    :_pool(threadNum,queSize)
    ,_server(ip,port)
    {}

    void start()
    {
        _pool.start();//计算线程全部启动（计算线程在这里就是线程池里的线程）
        using namespace std::placeholders;
        _server.setAllCallback(bind(&SearchEngineServer::onConnection,this,_1),
                               bind(&SearchEngineServer::onMessage,this,_1),
                               bind(&SearchEngineServer::onClose,this,_1));
        _server.start();//IO线程启动
    }

    #if 0
    void stop()
    {
        _pool.stop();
        _server.stop();
    }
    #endif

private:
    void onConnection(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has connected!" << endl;
    }
    
    void onMessage(const TcpConnectionPtr &con)
    {
        //回显

        #if 0
        //json的格式：
        json j = {
            {"mode", 1},
            {"keyWord", "I am a student!"}
        };
        #endif

        json data = con->recvJson();
        int mode = data["mode"];
        string keyWord = data["keyWord"];
        cout << keyWord << endl;
        if(1 == mode)
        {
            KeyRecommander keyRec(keyWord,con);
            _pool.addTask(std::bind(&KeyRecommander::process,keyRec));
        }
        else if(2 == mode)
        {
            WebPageSearcher webPage(keyWord,con);
            _pool.addTask(std::bind(&WebPageSearcher::process,webPage));
        }


        // MyTask task(msg, con);
        // /* gThreadPool->addTask(std::bind(&MyTask::process, &task)); */
        // _pool.addTask(std::bind(&MyTask::process, task));
    }
    
    void onClose(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has closed!" << endl;
    }

private:
    TcpServer _server;
    ThreadPool _pool;
    // KeyRecommander _key;
    // WebPageSearcher _web;
};


#endif
