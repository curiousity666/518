#include "DictProducer.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::ofstream;

DictProducer::DictProducer(const string& ENdir
                            ,const string& CNdir
                            ,const Configuration *config
                            ,SplitTool *splitTool)
: _spliTool(splitTool)
,_config(config)
{
    //通过目录得到目录内所有文件的名字。
    chdir("..");

    //测试地址
#if 0
    char path1[128];
	if(!getcwd(path1,128)){
		cout<<"Get path fail!"<<endl;
	}
	cout<<"path:"<<path1<<endl;
#endif
    //测试代码--------------------------------------------

    DIR *dir;
    if ((dir = opendir(CNdir.c_str())) == nullptr)
    {
        throw std::runtime_error("directory " + CNdir + " does not exist");
    }
    dirent *dp;
    for (dp = readdir(dir); dp != nullptr; dp = readdir(dir))
    {
        const std::string img_file_name = dp->d_name;

        // 使用readdir读取目录中的所有文件
        // 默认情况下，包括了'.'和'..'目录文件
        if (img_file_name == "." || img_file_name == "..")
        {
            continue;
        }
        _CNfiles.push_back(CNdir + "/" + img_file_name);
    }
    closedir(dir);
    chdir("src/");
    //构建中文词典和索引
    bulidCnDict();

    // 测试，输出当前地址
    char path[128];
	if(!getcwd(path,128)){
		cout<<"Get path fail!"<<endl;
	}
	cout<<"path:"<<path<<endl;
    //
    chdir("..");
    DIR *dir1;
    if ((dir1 = opendir(ENdir.c_str())) == nullptr)
    {
        throw std::runtime_error("directory1 " + ENdir + " does not exist");
    }
    dirent *dp1;
    for (dp1 = readdir(dir1); dp1 != nullptr; dp1 = readdir(dir1))
    {
        const std::string img_file_name = dp1->d_name;

        // 使用readdir读取目录中的所有文件
        // 默认情况下，包括了'.'和'..'目录文件
        if (img_file_name == "." || img_file_name == "..")
        {
            continue;
        }
        _ENfiles.push_back(ENdir + "/" + img_file_name);
    }
    closedir(dir1);
    chdir("src/");
    loadStopWords();
    bulidEnDict();
}

void DictProducer::loadStopWords()
{
    ifstream ifs(_config->_StopWordsPath);
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string word;
        while (iss >> word)
        {
            _stopWord.insert(word);
        }
    }
}

void DictProducer::bulidEnDict()
{
    chdir("..");
    for (auto tem : _ENfiles)
    {
        ifstream ifs(tem);
        if (!ifs)
        {
            cerr << "ifstream open " << tem << endl;
            return;
        }
        string line;
        while (getline(ifs, line))
        {
            //处理非法字符转为空格,大写字母转为小写
            for (auto &ch : line)
            {
                if (::ispunct(ch) || !::isalpha(ch))
                {
                    ch = ' ';
                }
                else if (::isupper(ch))
                {
                    ch = ::tolower(ch);
                }
            }
            istringstream iss(line);
            string word;
            while (iss >> word)
            {
                auto iter = _stopWord.find(word);
                if (iter == _stopWord.end())
                {
                    ++_ENdict[word];
                }
            }
        }
        //创建英文索引
        ifs.close();
    }
    
    size_t lineNumber = _CnFrequenc.size();
    for (auto &elem : _ENdict)
    {
        ++lineNumber;
        for (auto &alphElem : elem.first)
        {
            _ENindex[alphElem].insert(lineNumber);
        }
    }
    auto i = _ENdict.begin();
    cout << (++i)->first << endl;
    cout << (++i)->first << endl;
    chdir("src/");
}

void DictProducer::bulidCnDict()
{
    //建立好一个分词后的词典，名为words
    chdir("..");
    //建立_CNfrequency
    for (auto tem : _CNfiles)
    {
        ifstream ifs1(tem);
        chdir("src/");
        string s;
        vector<string> words;
        while (getline(ifs1, s))
        {
            // cout << s << endl;
            vector<string> curWord = _spliTool->cut(s);
            for (auto wd_ : curWord)
            {
                // cout << wd_ << endl;
                int &frequence_ = _CnFrequenc[wd_]; // 单词and频率
                ++frequence_;
            }
        }
        ifs1.close();
    }
    //建立_CNindex
    int line_ = 0;
    for (auto it_ = _CnFrequenc.begin(); it_ != _CnFrequenc.end(); ++it_)
    {
        ++line_;
        char c_curWord[4] = {0};
        char c_word[1024] = {0};
        strcpy(c_word, it_->first.c_str());
        for (int idx = 0; idx < it_->first.size(); idx += 3)
        {
            strncpy(c_curWord, it_->first.c_str(), 3);
            // cout << c_curWord <<endl;
            string curWord = c_curWord;
            auto &Line = _CNindex[curWord];
            Line.insert(line_);
        }
    }
    // cout << _CNindex << endl;
}

void DictProducer::showDict() const
{
    // int i =0; 记录文件个数
    for (auto name : _CNfiles)
    {
        cout << name << endl;
        // i++;
    }
    // cout << i << endl;
}

void DictProducer::showFiles() const
{
    // for (auto it = _CnFrequenc.begin(); it != _CnFrequenc.end(); ++it)
    // {
    //     cout << it->first << " " << it->second << endl;
    // }
    for (auto it = _CNindex.begin(); it != _CNindex.end(); ++it)
    {
        cout << it->first << " " << it->second << endl;
    }
}

void DictProducer::store()
{
    ofstream ofs("../data/Dict.dat");
    if (!ofs)
    {
        cerr << ">> ofstream open file 1 error!\n";
        return;
    }
    //先植入中文库，再植入英文库。
    for(auto &elem : _CnFrequenc)
    {
        ofs << elem.first << " " << elem.second << endl;
    }

    for (auto &elem : _ENdict)
    {
        ofs << elem.first << " " << elem.second << endl;
    }
    ofs.close();

    ofstream ofs1("../data/Index.dat");
    if (!ofs1)
    {
        cerr << ">> ofstream open file error!\n";
        return;
    }

    for(auto &elem:_CNindex)
    {
        ofs1 << elem.first << " ";
        for (auto &intElem : elem.second)
        {
            ofs1 << intElem << " ";
        }
        ofs1 << endl;
    }

    for (auto &elem : _ENindex)
    {
        ofs1 << elem.first << " ";
        for (auto &intElem : elem.second)
        {
            ofs1 << intElem << " ";
        }
        ofs1 << endl;
    }
    ofs1.close();
}


int main()
{
    vector<string> addr = {"../dict0/jieba.dict.utf8",
                           "../dict0/hmm_model.utf8",
                           "../dict0/user.dict.utf8",
                           "../dict0/idf.utf8",
                           "../dict0/stop_words.utf8"};
    Configuration config(addr, "../dict0/stop_words.utf8");
    SplitTool *splitTool = new SplitToolCppJieba(config);
    DictProducer Dic("data/eng","data/art", &config,splitTool); 
    //参数1：英文分词地址，2中文分词地址，参数3：配置文件 参数4：分词工具 
    Dic.store();
    // dic.showFiles();
    // dic.showDict();
    return 0;
}