#ifndef __WD_DICTIONARY_HPP__
#define __WD_DICTIONARY_HPP__

#include<string>
#include<vector>
#include<map>
#include<set>
#include<fstream>
#include<iostream>
#include<sstream>
#include<memory>
using namespace std;

class Dictionary
{
public:
    static Dictionary* getDictSigleInstance();
    void init(const string& dictPath,const string &dictIndexPath);
    vector<pair<string,int>>& getDict(){return *_dict;}  //获取词典
    map<string,set<int>>& getIndexTable(){return *_index;}  //获取词典索引
private:
    Dictionary(){}
    ~Dictionary(){}
    
private:
    class AutoRelease   //负责单例Dictionary类对象的释放
    {
    public:
        AutoRelease(){}

        ~AutoRelease(){
            if(_pDictSigleInstance!=nullptr){
                delete _pDictSigleInstance;
                _pDictSigleInstance==nullptr;
            }
        }
    };
private:
    shared_ptr<vector<pair<string,int>>>_dict;
    shared_ptr<map<string,set<int>>> _index;

    static Dictionary *_pDictSigleInstance;
    static AutoRelease _ar;
};

Dictionary* Dictionary::_pDictSigleInstance=nullptr;
Dictionary::AutoRelease Dictionary::_ar;

//获取词典单例对象
Dictionary* Dictionary::getDictSigleInstance(){
    if(_pDictSigleInstance==nullptr){
        _pDictSigleInstance=new Dictionary();
        //init("/home/joy/sousuo/server/datadict.data");
    }
    return _pDictSigleInstance;
}

//初始化词典和词典索引
void Dictionary::init(const string& dictPath,const string& dictIndexPath){

    _dict=make_shared<vector<pair<string,int>>>();
    _index=make_shared<map<string,set<int>>>();
    ifstream ifs4Dict(dictPath);
    if(!ifs4Dict){
        cout<<"open dictPath error"<<endl;
    }

    //获取词典
    string line;
    string word;
    int frequency;
    while(getline(ifs4Dict,line)){   //读取一整行
        istringstream ifs4Dict(line);    //将一行记录分解成pair<string,int>
        ifs4Dict>>word>>frequency;
        _dict->push_back(make_pair(word,frequency));
    }


    //获取（单个字，词语行号）的索引
    ifstream ifs2Index(dictIndexPath);
    if(!ifs2Index.is_open()){
        cout<<"open "<<dictIndexPath<<" error"<<endl;
        return;
    }
    int lineNo;
    while(getline(ifs2Index,line)){
        istringstream ifs2Index(line);

        ifs2Index>>word;
        while(ifs2Index>>lineNo){
            (*_index)[word].insert(lineNo);
        }
    }
}


#endif