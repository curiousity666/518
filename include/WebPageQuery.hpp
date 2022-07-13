//#include "../include/WordSegmentation.h"
#ifndef _WEBPAGEQUERY_HPP_
#define _WEBPAGEQUERY_HPP_
#include"WebPage.hpp"

#include<math.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <bitset>
#include <utility>
#include<set>
#include <fstream>
#include <iostream>
#include<queue>

using std::priority_queue;
using std::ifstream;
using std::ostream;
using std::set;
using std::pair;
using std::bitset;
using std::multimap;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

class WebPageQuery
{
public:
    WebPageQuery();
    bool doQuery(const string &str);//执行查询，返回结果,传一个长字符串，然后切割，
    void loadLibray();//加载库文件
    //vector<double>getQueryWorldWeight(vector<string>&queryWords);//计算查询词的权重值

    string getJson(vector<int>&docIdVec,const vector<string>&queryWords);
    string returnNoAnswer();
    WebPage getWebPage(int docid);//根据文档id生成对应的文档，在查询时用
    double getSimilarity(const vector<double> &lhs, const vector<double> &rhs);//计算相似度
    double getMold(const vector<double> &vec);
    //测试
    void display();
    set<WebPage> getResultPage();//返回网页结果
    vector<string> PagetoString();//把结果网页打包为字符串
private:
    //WordSegmentation _jieba;
    set<WebPage> _Resultpage;//查询结果网页库，使用set来进行相似度排序，在webPage中重载<符号，把相似度高的放在前面
    ifstream _ifsPage;//使用文件流存放网页库，
    unordered_map<int,pair<int,int>> _offsetLib;//偏移库
    unordered_map<string,set<pair<int,double>>> _invertIndexTable;//倒排索引表

};

#endif