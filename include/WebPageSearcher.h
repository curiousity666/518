#ifndef WEBPAGESEARCHER_H
#define WEBPAGESEARCHER_H

#include "TcpServer.h"
#include "WebPageQuery.hpp"
#include "nlohmann/json.hpp"
#include "simhash/Simhasher.hpp"


using nlohmann::json;

class WebPageSearcher
{
public:
    WebPageSearcher(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg)
    , _con(con)
    {
    }

    //该函数在线程池中执行的
    void process()
    {
        //...
        doQuery();
        //...

    }


private:
    json webPage2Json()
    {
        json j = {
            {"result",""}
        };
        j["result"] = _res;
        return j;
    }

    void sendWebPage()
    {
        json msg_2_send = webPage2Json();
        _con->sendInLoop(msg_2_send);
    }
    void sendNull()
    {
        json j = {
            {"result","null"}
        };
        _con->sendInLoop(j);
    }

    void doQuery()
    {
        //初始化
        WebPageQuery wp;
        //set<WebPage> Resultpage=wp.getResultPage();
        bool flag=wp.doQuery(_msg);
        // bool flag=wp.doQuery("精神支柱");
        if(!flag)
        {
            // cout<<"没有结果"<<endl;
            _res = "没有结果";
        }
        
        //获得每个网页的信息
        vector<string> vStr=wp.PagetoString();
        if(0 == vStr.size())
        {
            sendWebPage();
        }

        for(auto e:vStr)
        {
            // cout<<e<<endl;
            _res = e;
            //一篇篇发送
            sendWebPage();
        }
        cout << _res << endl;

        //发送结束时，发送个字符串null
        sendNull();
    }

private:
    string _msg;
    string _res;
    TcpConnectionPtr _con;
};

#endif