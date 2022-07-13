/*
 * @Author: Channing
 * @Date: 2022-06-28 11:24:26
 * @LastEditTime: 2022-06-28 17:08:38
 * @FilePath: /wangdao/SeachEngine/module2_mypart/WebPage.h
 * @Description: 
 * Copyright (c) 2022 by Channing 995825312@qq.com, All Rights Reserved. 
 */

#pragma once
// #include "SplitTool.h"
#include "SingleSimhasher.h"
#include <string>
#include <set>
#include <map>
#include <vector>

using namespace std;


class WebPage
{
    friend class PageLib;
public:
    WebPage(size_t docid,const string &title,const string &url,string &&content);
    ~WebPage() {}
    
    void Calculate();//计算单词权重,计算simhash值
    uint64_t getSimhashValue() const;//

private:
    size_t _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    string _docSummary;
    vector<pair<string,double>> _weights;
    uint64_t _simhashValue;
private:
    const static Simhasher &_simhash;
    const static size_t TOPK_NUMBER;
};

