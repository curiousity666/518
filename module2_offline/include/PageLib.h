/*
 * @Author: Channing
 * @Date: 2022-06-26 13:39:11
 * @LastEditTime: 2022-06-28 17:08:44
 * @FilePath: /wangdao/SeachEngine/module2_mypart/PageLib.h
 * @Description: 
 * Copyright (c) 2022 by Channing 995825312@qq.com, All Rights Reserved. 
 */
#pragma once
#include <map>
#include <unordered_map>

// #include "DirScanner.h"
// #include "FileProcessor.h"
#include "WebPage.h"

using std::pair;
using std::map;
using std::unordered_map;
using std::unordered_set;


class Equal{
public:
    bool operator()(const uint64_t &lhs,const uint64_t &rhs) const{
        return Simhasher::isEqual(lhs,rhs,hammingDistance);
    }
private:
    unsigned short hammingDistance = 3;
};

class PageLib
{
public:
    // PageLib(Configuration &conf,DirScanner &dirScanner,FileProcessor&fileProcessor);
    PageLib();
    ~PageLib() {}

private:
    void traverse(const string &dirname);//获取某目录下的所有文件
    void create();
    void buildInvertIndexTable();//生成倒排索引
    void store();//1.保存网页库
    void storeOnDisk();//2.保存偏移库和3.倒排索引库
    
private:
    vector<string> _files;//保存rss路径
    unordered_set<uint64_t,std::hash<uint64_t>,Equal> simhashValue;
    vector<WebPage> _webpages;//存放网页对象
    vector<string> _pages;//网页库容器对象
    map<int,pair<int,int>> _offsetLib;//网页偏移库对象
    unordered_map<string,vector<pair<int,double>>> _invertIndexTable;//倒排索引对象
};

