#include <vector>
#include <string>
using std::string;
using std::vector;

class Configuration
{
public:
    Configuration(const vector<string> &addr,string StopWords)
    :_JiebaAddr(addr)
    ,_StopWordsPath(StopWords)
    {
    }
public:
    string _StopWordsPath;
    vector<string> _JiebaAddr;
};


