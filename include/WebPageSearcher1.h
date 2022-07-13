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
        //初始化
        WebPageQuery wp;
        //set<WebPage> Resultpage=wp.getResultPage();
        bool flag=wp.doQuery(_msg);
        // bool flag=wp.doQuery("精神支柱");
        if(!flag)
        {
            // cout<<"没有结果"<<endl;
            sendNull();
            return;
        }
        
        //获得每个网页的信息
        vector<string> _res=wp.PagetoString();  


        //下面代码目的是实现分页发送效果
        //代码逻辑：多次发送，每次发送10个网页信息
        size_t len = _res.size();
        cout << "len："<< len << endl;
        int cnt = (len+9) / 10;//发送次数，向上取整
        cout << "cnt:" << endl;

        //优化，直接发送_res
        if(1 == cnt)
        {
            sendWebPage1();
        }
        else
        {
            int begin = 0;//每次发送的起始位置
            while(cnt-- > 0)
            {
                splitWebPage(len,begin);
                begin += 10;

                //发送
                sendWebPage();
            }
            //多次发送结束时，发送一个字符串null
            sendNull();
        }
    }


private:
    void sendWebPage()
    {
        json msg_2_send(_subRes);
        _con->sendInLoop(msg_2_send);
    }
    void sendWebPage1()
    {
        json msg_2_send(_res);
        _con->sendInLoop(msg_2_send);
    }
    void sendNull()
    {
        vector<string> tmp{"null"};
        json msg_2_send(tmp);
        _con->sendInLoop(msg_2_send);
    }

    vector<string> subVector(vector<string>& Arrs, int begin, int end)
    {   
        // begin <= end;
        //if(end > Arrs.size()) return;
        vector<string> result;
        result.assign(Arrs.begin() + begin, Arrs.begin() + end);
        return result;
    }
    void splitWebPage(size_t len,int begin)
    {
        int left = begin;
        int right = left + 10;
        if(right <= len)
        {
            _subRes = subVector(_res,left,right);
        }
        else
        {
            _subRes = subVector(_res,left,len);
        }
    }


    

private:
    string _msg;
    vector<string> _res;
    vector<string> _subRes;
    TcpConnectionPtr _con;
};

#endif