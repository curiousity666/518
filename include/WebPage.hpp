// #include "SingleSimhasher.hpp"
// #include "handleString.hpp"
// #include "log4cpp.hpp"
//#include "../include/WordSegmentation.h"
#ifndef _WEBPAGE_HPP_
#define _WEBPAGE_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <bitset>
using std::bitset;
using std::multimap;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::map;
using std::string;

class WebPage
{
    //friend bool operator==(const WebPage &lhs,const WebPage & rhs);
    //friend ;//可以用这个来进行排序
public:
    WebPage(int docid,string &docTitle, string &docUrl,string &content)
    :_docId(docid)
    ,_docTitle(docTitle)
    ,_docUrl(docUrl)
    ,_docContent(content)
    ,_docSummary(Mysubstr(_docContent,100)+"...")//获取内容的前三十个字符
    {

    }
    //无参构造函数
    WebPage()
    :_docId(-1)
    {

    }
    int getDocId()const
    {
        return _docId;
    }

    string getTitle()const
    {
        return _docTitle;
    }
    string getUrl() const
    {
        return _docUrl;
    }
    void setSimilarity(const double &value)
    {
        _similarity=value;
    }
    //根据关键字，设置摘要
    string getSummary()const
    {
        return _docSummary;
    }
    
    string getContent()const
    {
        return _docContent;
    }

    double getSimilarity()const
    {
        return _similarity;
    }


    static size_t nBytesCode(const char ch)
    {
        if (ch & (1 << 7))
        {
            int nBytes = 1;
            for (int idx = 0; idx != 6; ++idx)
            {
                if (ch & (1 << (6 - idx)))
                {
                    ++nBytes;
                }
                else
                    break;
            }
            return nBytes;
        }
        return 1;
    }

    //从中英文字符串中拷贝前n个字， 如果字符串没有那么长，返回整个字符串
    static string Mysubstr(const string &copy_from, size_t len)
    {
        size_t curpos = 0;
        for (size_t curlen = 0; curlen != len; ++curpos)
        {
            if (curpos == copy_from.size() - 1)
                return copy_from;
            int nBytes = nBytesCode(copy_from[curpos]);
            curpos += (nBytes - 1);
            ++curlen;
        }
        return copy_from.substr(0, curpos);
    }
    bool operator<(const WebPage &rhs) const
    {
        if(_docId==rhs._docId)
        {
            return false;
        }
        if(_similarity>rhs._similarity)
        {
            return true;
        }
        if(_similarity==rhs._similarity)
        {
            if(_docId==rhs._docId)
            {
                printf("--\n");
                return false;
            }
            return true;
        }else
        {
            return false;
        }
    }
    // map<string,int> &getWordsMap()const
    // {
    //     return _wordsMap;
    // }

    //void processDoc(const string &doc,)

private:
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    string _docSummary;//文档摘要
    double _similarity;//与关键字的相似度，
    map<string,int> _wordsMap;//保存每篇文档的所有词语和词频

};

// bool operator==(const WebPage &lhs,const WebPage & rhs)
// {
//     printf("==\n");
//     return lhs._docId==rhs._docId;
// }

// bool operator<(const WebPage &lhs,const WebPage &rhs)
// {

//     if(lhs._docId==rhs._docId)
//     {
//         return false;
//     }
//     if(lhs._similarity>rhs._similarity)
//     {
//         return true;
//     }

    
//     if(lhs._similarity==rhs._similarity)
//     {
//         if(lhs._docId==rhs._docId)
//         {
//             printf("--\n");
//             return false;
//         }
//         return true;
//     }else
//     {
//         return false;
//     }
    
    
// }

#endif
