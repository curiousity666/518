#include <unordered_set>
#include <iostream>
#include "SplitTool.h"
using std::cout;
using std::endl;

using std::ifstream;
using std::unordered_set;

SplitToolCppJieba::SplitToolCppJieba(Configuration &config)
    : _dict_path(config._JiebaAddr[0]),
      _hmm_path(config._JiebaAddr[1]),
      _user_dict_path(config._JiebaAddr[2]),
      _idf_path(config._JiebaAddr[3]),
      _stop_word_path(config._JiebaAddr[4]),
      _jieba(_dict_path,
             _hmm_path,
             _user_dict_path,
             _idf_path,
             _stop_word_path)

{
    string stopLine;
    //加载停用词库
    ifstream ifs(_stop_word_path);
    assert(ifs);
    while (getline(ifs, stopLine))
    {
        // cout << stopLine << endl;
        _stopWords.insert(stopLine);
    }
    ifs.close();
}

vector<string> SplitToolCppJieba::cut(const string &sentence)
{
    vector<string> words;
    // cout << sentence << endl;
    _jieba.CutForSearch(sentence, words, true);
    for (auto it = words.begin(); it != words.end();)
    {
        // cout << *it << endl;
        if (_stopWords.find(*it) != _stopWords.end())
        {
            it = words.erase(it);
            continue;
        } else if(atoi(it->c_str()))
        {
            it = words.erase(it);
            continue;
        } else if(isalnum((*it)[0]))
        {
            it = words.erase(it);
            continue;
        }
        ++it;
    }
    return words;
}