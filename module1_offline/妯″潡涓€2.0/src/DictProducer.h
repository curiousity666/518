#include "SplitTool.h"
#include <map>
#include <set>

using std::map;
using std::pair;
using std::set;

class DictProducer
{
public:
    DictProducer(const string &ENdir
                , const string &CNdir
                , const Configuration *config
                , SplitTool *splitTool);
    void store();
    void showFiles() const;
    void showDict() const;
    // void getFiles();
    // void pushDict(const string&word);
private:
    void loadStopWords();
    void bulidEnDict();
    void bulidCnDict();

private:
    vector<string> _CNfiles;
    vector<string> _ENfiles;
    unordered_set<string> _stopWord;
    SplitTool *_spliTool;
    map<string, int> _CnFrequenc;   //单词and频率
    map<string, set<int>> _CNindex; //单词 and行号
    map<string, int> _ENdict;
    map<char, set<int>> _ENindex;
    const Configuration *_config;
};