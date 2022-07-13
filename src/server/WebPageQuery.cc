#include"../../include/WebPageQuery.hpp"
#include"../../include/simhash/Simhasher.hpp"
// #include<head.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <utility>
#include<map>
#include<sstream>
#include<set>
#include<memory>


using std::cout;
using std::endl;
using std::cerr;
using std::vector;
using std::string;
using std::ifstream;
using std::ostream;
using std::map;
using std::set;
using std::pair;
using std::ofstream;
using std::cin;
using std::istringstream;
using std::shared_ptr;
using namespace simhash;


//初始化网页库文件流
WebPageQuery::WebPageQuery( )
:_ifsPage("../../data/ripepageFile.dat")
{
    loadLibray();
   
}


void WebPageQuery::loadLibray()
{
    //加载偏移库
    string filename="../../data/offset.dat";
    ifstream ifs(filename);
    if(!ifs.good()){
        cerr<<">> ifstream open file error!\n";
        return;
    }
    string line;
    string word;
    while(getline(ifs,line)){
        int j=0;//用于该单词是否是该行第一个单词
        istringstream iss(line);//把一行字符串，分解为一个个字符串
        int wordOne;
        while(iss>>word){
        //!使用map下标，插入map,且map不会插入重复的值
            if(j==0){
                _offsetLib[atoi(word.c_str())];
                wordOne=atoi(word.c_str());//保存每行第一个单词
            }
            auto &wordTwo=_offsetLib[wordOne];//下标访问
            if(j==1){
                wordTwo.first=atoi(word.c_str());//wordTwo是引用，所以会改变map中value值
            }
            if(j==2)
            {
                wordTwo.second=atoi(word.c_str());//wordTwo是引用，所以会改变map中value值
            }
            ++j;
        }
    }

    //测试
    // for(auto it=_offsetLib.begin();it!=_offsetLib.end();++it)
    // {
    //     cout<<it->first<<"  "
    //         <<it->second.first<<" "
    //         <<it->second.second<<endl;
    // }
    
    //cout<<_offsetLib[1].first<<"  "<<_offsetLib[1].second<<endl;
    //cout<<_offsetLib[4315].first<<"  "<<_offsetLib[4315].second<<endl;
    //测试
    ifs.close();

    //加载倒排索引表
    filename="../../data/invertIndex.dat";
    ifstream ifs1(filename);
    if(!ifs1.good()){
        cerr<<">> ifstream open file error!\n";
        return;
    }
    string line1;//每一行
    string word1;//每个词
    pair<int,double> invert;//保存文档id和权值
    while(getline(ifs1,line1)){
        int j=0;//用于该单词是否是该行第一个单词
        istringstream iss(line1);//把一行字符串，分解为一个个字符串
        string wordOne;
        while(iss>>word1){
        //!使用map下标，插入map,且map不会插入重复的值
            if(j==0){
                _invertIndexTable[word1];
                wordOne=word1;//保存词语
            }
            auto &wordTwo=_invertIndexTable[wordOne];//下标访问,因为是引用，所以改变值，也会改变容器中的值

            
            //表示是文档id
            if(j%2==1&&j!=0)
            {
                invert.first=atoi(word1.c_str());
                //printf("invert docid=%d\n",invert.first);
            }
            else if(j%2==0&&j!=0)
            {
                //invert.first=docid;
                invert.second=atof(word1.c_str());
                wordTwo.insert(invert);
                //printf("wordTwo docid=%d\n",invert.first);
            }
            ++j;
        }
    }

    //对_invertIndexTable进行遍历
    // for(auto it=_invertIndexTable.begin();it!=_invertIndexTable.end();++it)
    // {
    //     //first是词语
    //     cout<<it->first<<"  ";
    //     //second是文档id和权重
    //     for(auto it1=it->second.begin();it1!=it->second.end();++it1)
    //     {
    //         //first 是文档id，second是权重
    //         cout<<"  "<<it1->first<<"  "<<it1->second;
    //     }
    //     cout<<endl;
    // }


    
   

   //加载网页库,需要写一个函数，把网页库中的标签给去掉


}

//把网页库中的title，content给取出来
string findItem(string text,string flag)
{
    
    size_t beg = text.find("<" + flag + ">", 0);
    size_t end = text.find("</" + flag + ">", beg);
    
    if (beg == text.size() || end == text.size())
    {
       // cout<<"findItem2"<<endl;
        //logError("webpage of id:%u donot contain flag:%s", webpageId, flag);
        return string();
    }
    
    beg += flag.size() + 2;
  
    return text.substr(beg, end - beg);
}


WebPage WebPageQuery::getWebPage(int docid)
{
    //   for(auto it=_offsetLib.begin();it!=_offsetLib.end();++it)
    // {
    //     cout<<it->first<<"  "
    //         <<it->second.first<<" "
    //         <<it->second.second<<endl;
    // }
    //cout<<"getweb"<<endl;
    //在偏移库中查找
    auto it=_offsetLib.find(docid);
    //cout<<it->first<<endl;
    if(it==_offsetLib.end())
    {
        cout<<"null webPage"<<endl;
        //返回一个空类，等下再写
        return WebPage();
    }
    //跳转到指定该文档开始id
    _ifsPage.seekg(it->second.first);
   size_t length=it->second.second;
   string text;
   text.resize(length);
    //cout<<"pointer1"<<endl;
   _ifsPage.read((char *)text.c_str(),length);

   //cout<<text<<endl; 

    
    //int docid1=atoi(findItem(text,"docid").c_str()) ;
    string title=findItem(text,"title");
    string url=findItem(text,"link");
    string content=findItem(text,"content");
    
    WebPage ret(docid,title,url,content);
    
    return ret;
}

bool WebPageQuery::doQuery(const string &str)
{
    
    Simhasher simhasher("../../dict/jieba.dict.utf8", "../../dict/hmm_model.utf8", "../../dict/idf.utf8", "../../dict/stop_words.utf8");
   
    size_t topN = 5;
    vector<pair<string ,double> > res;
    //res关键词序列，其中存储了权重,topN个数
    simhasher.extract(str, res, topN);
    
    //权重
    vector<double> Wordweight;
    
    //关键词
    vector<string> keywords;
    
    
    
    for(auto &elem:res)
    {
       
        keywords.push_back(elem.first);
        
        Wordweight.push_back(elem.second);
        
    }
    
    
    
    //写两个容器，然后一个每次都置为空去接受新的网页，然后另一个就每次都与新的网页进行取交集
    // set<pair<int,double>> newset;
    // set<pair<int,double>> andset;
    map<int,double> DocWeight; 
    for(auto &elem:keywords)
    {
        //cout<<elem<<endl;
        auto it=_invertIndexTable.find(elem);
        
        if(it==_invertIndexTable.end())
        {
            
            //没有该关键字返回错误信息
            //cout<<"no result"<<endl;
            return false;
        }
        cout<<"word:"<<it->first<<endl;
        //文档id,和权重
        auto doc=it->second;
        vector<double> weight;
        set<int> DocId;//存放文档id
        for(auto e:doc)
        {
            
            //把网页id和权重加到set中
            DocWeight[e.first]=e.second;
            
            
            //存放关键词的权重
        }
    
    }

    for(auto &e:DocWeight)
    {
        vector<double> weight;
        weight.push_back(e.second);
            WebPage webpage=getWebPage(e.first);
            if(webpage.getDocId()==-1)
            {
                break;
            }
            double simlar=getSimilarity(Wordweight,weight);
            webpage.setSimilarity(simlar);
            
            // cout<<" id:"<<webpage.getDocId()<<"    title:"<<webpage.getTitle()<<"  "<<endl;
            pair<set<WebPage>::iterator, bool > ret=_Resultpage.insert(webpage);
            // if(ret.second)
            // {
            //     //cout << "该元素插入成功" <<  endl;
            // }
            // else
            // {
            //     cout << "插入失败,该元素存在set中" << endl;
            // }
    }





    // while (_Resultpage.size()!=0)
    // {
    //     cout<<_Resultpage.top().getDocId()<<"    title:"<<_Resultpage.top().getTitle()<<"  "
    //         <<"\n\nsummary:"<<_Resultpage.top().getSummary()<<endl;

    //     _Resultpage.pop();    
    //     //<<endl;
    //     /* code */
    // }
    
    
    
    //首先取倒排索引库里，去查找关键词对应的文档id
    return true;
}

double WebPageQuery::getSimilarity(const vector<double> &lhs, const vector<double> &rhs)
{
    int n = lhs.size();
    //assert(n == rhs.size());
    double tmp = 0.0; //内积
    for (int i = 0; i < n; ++i)
        tmp += lhs[i] * rhs[i];
    return tmp / (getMold(lhs) * getMold(rhs));

}
double WebPageQuery::getMold(const vector<double> &vec)
{ //求向量的模长
    int n = vec.size();
    double sum = 0.0;
    for (int i = 0; i < n; ++i)
        sum += vec[i] * vec[i];
    return sqrt(sum);
}

set<WebPage> WebPageQuery::getResultPage()
{
    return _Resultpage;
}
//"id:"+to_string(e.getDocId())+
vector<string> WebPageQuery::PagetoString()
{
    vector<string> vStr;
    
    int i=0;
    for(auto e:_Resultpage)
    {
       string str;
       i+=("id "+to_string(e.getDocId())+"title "+e.getTitle()+"url "+e.getUrl()+"summary "+e.getSummary()).size();
       str.reserve(i+50);
       str+=("id "+to_string(e.getDocId())+" title "+e.getTitle()+" url "+e.getUrl()+" summary "+e.getSummary());
       vStr.push_back(str);
    }
   //先预留位置
    // for(auto e:_Resultpage)
    // {
    //     str+=("id "+to_string(e.getDocId())+" title "+e.getTitle()+" url "+e.getUrl()+" summary "+e.getSummary());
    // }
    return vStr;
}

// int main()
// {
//     //初始化
//     WebPageQuery wp("租房保障性");
//     //set<WebPage> Resultpage=wp.getResultPage();

 
    
//     //获得每个网页的信息
//     vector<string> vStr=wp.PagetoString();
//     for(auto e:vStr)
//     {
//         cout<<e<<endl;
//     }
        
//     return 1;
// }