#ifndef KEYRECOMMANDER_HPP
#define KEYRECOMMANDER_HPP

#include"Recommander.hpp"
#include "Dictionary.hpp"
#include "TcpServer.h"
#include "nlohmann/json.hpp"
#include<iostream>
#include<string>
#include<vector>

using nlohmann::json;
using namespace std;

class KeyRecommander
{
public:
    KeyRecommander(const string &queryWord,const TcpConnectionPtr &con)
    :_queryWord(queryWord)
    ,_con(con)
    {
    }

    //该函数在线程池中执行的
    void process()
    {
        //...
        Dictionary *p=Dictionary::getDictSigleInstance();
        p->init("../../data/dict.dat","../../data/dictIndex.dat");
        Recommander keyRec(_queryWord);
        vector<string> vec=keyRec.query();
        for(auto i=vec.begin();i!=vec.end();i++)
        {
            _ReKeys+=*i;
            _ReKeys+=" ";
        }
        //...

        json keys_2_send = keyWords2Json();
        _con->sendInLoop(keys_2_send);
    }

private:
    json keyWords2Json()
    {
        json j = {
            {"result",""}
        };
        j["result"] = _ReKeys;
        return j;
    }

private:
    string _queryWord;//查询关键词
    string _ReKeys;//返回关键词
    TcpConnectionPtr _con;
};

#endif