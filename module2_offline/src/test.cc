/*
 * @Author: Channing
 * @Date: 2022-06-26 14:55:06
 * @LastEditTime: 2022-06-28 17:09:27
 * @FilePath: /wangdao/SeachEngine/module2_mypart/test.cc
 * @Description: 
 * Copyright (c) 2022 by Channing 995825312@qq.com, All Rights Reserved. 
 */
#include <iostream>
// #include "DirScanner.h"
// #include "FileProcessor.h"
#include "../include/PageLib.h"

using namespace std;

const Simhasher &WebPage::_simhash = SingleSimhasher::getInstance();
const size_t WebPage::TOPK_NUMBER = 20;
// int PageNum = 1;

//1.test Dirscanner
// void test1(){
//     DirScanner scan;
//     scan.traverse(".");
//     vector<string> result;
//     result = scan.getFiles();
//     for(auto &it : result){
//         cout << it << endl;
//     }
//     cout << endl;
// }


// //2.test FileProcessor
// void test2(){
//     FileProcessor fileP;
//     fileP.process("history.xml");
// }

// void test3(){
//     DirScanner scan;
//     scan("./yuliao");//获取该目录下的所有文件
//     FileProcessor filep;
//     PageLib page(scan,filep);
//     page.create();
//     page.store();
// }



void test4(){
    PageLib page;
}
int main()
{
    test4();
    return 0;
}

