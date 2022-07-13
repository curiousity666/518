#ifndef __WD_KEYRECOMMANDER_HPP__
#define __WD_KEYRECOMMANDER_HPP__
#include "Dictionary.hpp"
#include "MinEditDist.hpp"
#include<string>
#include<vector>
#include<map>
#include<queue>
using namespace std;

class QueryResult
{
    friend bool operator<(const QueryResult &lhs,const QueryResult &rhs);
public:
    QueryResult(string word,int dist,int frequeycy)
    :_candidateWord(word)
    ,_editDist(dist)
    ,_frequency(frequeycy)
    {}

    string _candidateWord;
    int _editDist;
    int _frequency;
};

bool operator<(const QueryResult &lhs,const QueryResult &rhs){
    if(lhs._editDist!=rhs._editDist){
        return lhs._editDist<rhs._editDist;
    }
    else if(lhs._frequency!=rhs._frequency){
        return lhs._frequency>rhs._frequency;
    }
    else{
        return lhs._candidateWord<rhs._candidateWord;
    }
}


class Recommander
{
public:
    Recommander(string queryWord)
    :_queryWord(queryWord)
    ,_pDict(Dictionary::getDictSigleInstance())
    ,_minDistCalculator()
    {}
    vector<string> query();
private:
    void splitWord(const string &word, vector<string> &ret);    //词语分解成单个字符
    //void query();
private:
    string _queryWord;
    //priority_queue<QueryResult> _resultQue;
    Dictionary* _pDict;    //一定要在某个地方先执行其init()函数
    MinEditDist _minDistCalculator; //计算最小编辑距离的类
    set<QueryResult> _result;
};

//词语分解成单个字符
void Recommander::splitWord(const string &word, vector<string> &ret)
{
    int num = word.size();
    int i = 0;
    while(i < num)
    {
        int size = 1;
        if(word[i] & 0x80)
        {
            char temp = word[i];
            temp <<= 1;
            do{
                temp <<= 1;
                ++size;
            }while(temp & 0x80);
        }
        string subWord;
        subWord = word.substr(i, size);
        ret.push_back(subWord);
        i += size;
    }
}

//查询
vector<string> Recommander::query(){
    //先获取词典和索引表
    map<string,set<int>> dictIndex=_pDict->getIndexTable(); 
    vector<pair<string,int>> dict=_pDict->getDict();

    vector<string> sigleWordVec;
    splitWord(_queryWord,sigleWordVec); //将待查询词语分解成单个字符，保存在sigleWordVec

    for(auto sigleWord:sigleWordVec){
        auto &Set=dictIndex[sigleWord];
        for(auto idx:Set){
            auto &pair=dict[idx];
            //_tmpResult.insert(pair.first);
            int minDist=_minDistCalculator.editDistance(_queryWord,pair.first);
            QueryResult ret(pair.first,minDist,pair.second);
            _result.insert(ret);
        }
    }

    vector<string> ret;
    int cnt=0;
    for(auto elem:_result){
        ret.push_back(elem._candidateWord);
        cnt++;
        if(cnt>=5){
            break;
        }
    }
    return ret;
}




#endif