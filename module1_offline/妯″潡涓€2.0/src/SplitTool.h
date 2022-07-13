#include <vector>
#include <string>
#include "Configuration.h"
#include <unordered_set>

#include "../lib/cppjieba2/Jieba.hpp"

using std::unordered_set;
using std::string;
using std::vector;
class SplitTool
{
public:
    virtual ~SplitTool()
    {}
    virtual vector<string> cut(const string& sentence) = 0;  
};

class SplitToolCppJieba
:public SplitTool
{
public:
    SplitToolCppJieba(Configuration &config);
    vector<string> cut(const string &sentence);
private:
    string _dict_path;
    string _hmm_path;
    string _user_dict_path;
    string _idf_path;
    string _stop_word_path;
    unordered_set<string> _stopWords;
    cppjieba::Jieba _jieba;
};

