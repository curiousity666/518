/*
 * @Author: Channing
 * @Date: 2022-06-26 13:39:17
 * @LastEditTime: 2022-06-30 10:24:26
 * @FilePath: /SeachEngine/module2_mypart/src/PageLib.cc
 * @Description: 
 * Copyright (c) 2022 by Channing 995825312@qq.com, All Rights Reserved. 
 */
#include "../include/PageLib.h"
#include "../include/tinyxml2.h"

#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <regex>
#include <vector>
#include <dirent.h>
#include <unistd.h>

using namespace tinyxml2;
using std::cout;
using std::endl;
using std::cerr;
using std::vector;
using std::regex;
using std::regex_replace;
using std::ofstream;
using std::ifstream;
using std::pair;


PageLib::PageLib(){
    traverse("../data/yuliao");
    cout << _files;
    create();
    buildInvertIndexTable();
    store();
    storeOnDisk();
}


//遍历得到rss文件
void PageLib::traverse(const string &dirname){
    DIR* dirp = opendir(dirname.c_str());
    if(dirp == nullptr){
        perror("opendir");
        return;
    }

    struct dirent* p_dirent;
    while((p_dirent = readdir(dirp)) != NULL){
        if(p_dirent->d_name[0] == '.'){
            continue;;
        }
        string name = p_dirent->d_name;
        //如果存绝对路径
        // string path = getcwd(nullptr,0);
        // path = path + "/" + name;

        _files.push_back(name);
    }
    closedir(dirp);
}

//生成网页webpage和倒排索引
void PageLib::create(){
    size_t docid = 1;
    chdir("../data/yuliao");
    for(auto &file : _files){
        cout << file << endl;
        XMLDocument doc;
        doc.LoadFile(file.c_str());
        if(doc.ErrorID()){
            cerr << "load file failed!" << endl;
        }
        XMLElement* item = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
        while(item){
            string title,link,description,content;
            if(item->FirstChildElement("title")){
                title = item->FirstChildElement("title")->GetText();
            }
            if(item->FirstChildElement("link")){
                link = item->FirstChildElement("link")->GetText();
            }
            if(item->FirstChildElement("content")){
                content = item->FirstChildElement("content")->GetText();
            }
            else if(item->FirstChildElement("description")){
                description = item->FirstChildElement("description")->GetText();
            }
            regex reg("<[^>]+>");
            description = regex_replace(description,reg,"");
            content = regex_replace(content,reg,"");
            if(content == ""){
                content = description;
            }
            WebPage webpage(docid,title,link,std::move(content));
            webpage.Calculate();
            auto result = simhashValue.insert(webpage.getSimhashValue());
            if(result.second){//判断是否重复,插入成功就没重复
                _webpages.push_back(webpage);
            }
            item = item->NextSiblingElement();
            ++docid;
        }
    }
}

void PageLib::buildInvertIndexTable(){
    for(auto &webpage : _webpages){
        for(auto &weight : webpage._weights){
            _invertIndexTable[weight.first].emplace_back(webpage._docId,weight.second);
        }
    }
}

void PageLib::store(){
    chdir("..");
    ofstream ofs("ripepageFile.dat");
    if(!ofs){
        cerr << "ofstream open file failed!" << endl;
    }
    for(auto &item : _webpages){
        ofstream::pos_type beg = ofs.tellp();
        // cout << "I am here" << ofs.tellp() << endl; 
        string text = "<doc>\n\t<docid>" + std::to_string(item._docId) + "</docid>\n"
        + "\t<title>" + item._docTitle + "</title>\n"
        + "\t<link>" + item._docUrl + "</link>\n"
        + "\t<content>" + item._docContent + "</content>\n"+"</doc>\n";
        size_t length = text.size();
        ofs << text;
        // cout << "I am here" << ofs.tellp() << endl; 
        _offsetLib[item._docId] = {beg,length};
    }
}


void PageLib::storeOnDisk(){
    //chdir("..");
    ofstream offsetFile("offset.dat"),invertIndexFile("invertIndex.dat");
    if(!(offsetFile && invertIndexFile)){
        cerr << "ofstream open file failed" << endl;
    }
    for(auto &item : _offsetLib){
        offsetFile << item.first << "\t" << item.second.first << " " << item.second.second << endl;
    }

    for(auto &item : _invertIndexTable){
        invertIndexFile << item.first << "\t\t";
        for(auto &elem : item.second){
            invertIndexFile << elem.first << " " << elem.second << "\t";
        }
        invertIndexFile << endl;
    }
}