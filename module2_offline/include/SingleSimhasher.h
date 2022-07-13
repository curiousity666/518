/*
 * @Author: Channing
 * @Date: 2022-06-28 16:34:04
 * @LastEditTime: 2022-06-30 10:30:29
 * @FilePath: /SeachEngine/module2_mypart/include/SingleSimhasher.h
 * @Description: 
 * Copyright (c) 2022 by Channing 995825312@qq.com, All Rights Reserved. 
 */
#pragma once
#include "../deps/simhash/Simhasher.hpp"
using namespace simhash;

class SingleSimhasher
{
public:
    static Simhasher &getInstance(){
        static SingleSimhasher instance;
        return instance._simhash;
    }
    
private:
    SingleSimhasher() 
    : _simhash("../dict/jieba.dict.utf8","../dict/hmm_model.utf8","../dict/idf.utf8","../dict/stop_words.utf8")
    {

    }
    ~SingleSimhasher() {}

private:
    Simhasher _simhash;
};

