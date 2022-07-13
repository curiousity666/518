/*
 * @Author: Channing
 * @Date: 2022-06-28 11:24:18
 * @LastEditTime: 2022-06-30 10:18:23
 * @FilePath: /SeachEngine/module2_mypart/src/WebPage.cc
 * @Description: 
 * Copyright (c) 2022 by Channing 995825312@qq.com, All Rights Reserved. 
 */
#include "../include/WebPage.h"


WebPage::WebPage(size_t docid,const string &title,const string &url,string &&content)
: _docId(docid)
, _docTitle(title)
, _docUrl(url)
, _docContent(content)
{

}

void WebPage::Calculate(){
    _simhash.extract(_docContent,_weights,TOPK_NUMBER);
    _simhash.make(_docContent,TOPK_NUMBER,_simhashValue);
}

uint64_t WebPage::getSimhashValue()const{
    return _simhashValue;
}
