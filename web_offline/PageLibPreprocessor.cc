#include "PageLibPreprocessor.h"
#include "math.h"
#include "mylog.h"
#include "tinyxml2.h"
#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace tinyxml2;
using std::ofstream;
using std::ostringstream;
using std::unordered_set;

const size_t topN = 10;
struct HashCmp {
    bool operator()(const uint64_t u1, const uint64_t u2) const {
        return simhash::Simhasher::isEqual(u1, u2, 3);
    }
};

struct pairCmp {
    bool operator()(const pair<string, int> &p1,
                    const pair<string, int> &p2) const {
        return p1.second > p2.second;
    }
};

static bool isChinese(const string &str) { return (str[0] & 0x80); }

simhash::Simhasher *PageLibPreprocessor::createSimhasher() {
    auto dictPath = _conf.getConfigMap()["DICT_PATH"].c_str();
    auto hmmPath = _conf.getConfigMap()["HMM_PATH"].c_str();
    auto idfPath = _conf.getConfigMap()["IDF_PATH"].c_str();
    auto stopWordsPath = _conf.getConfigMap()["JIEBA_STOP_WORD_PATH"].c_str();
    return new simhash::Simhasher(dictPath, hmmPath, idfPath, stopWordsPath);
}

PageLibPreprocessor::PageLibPreprocessor(Configuration &conf,
                                         SplitTool *wordCutter)
    : _conf(conf), _wordCutter(wordCutter), _psimhasher(createSimhasher()) {}

void PageLibPreprocessor::cutRedundantPage() {
    const string webPath = _conf.getConfigMap()["WEB_LIB_RAW"];

    XMLDocument doc;
    doc.LoadFile(webPath.c_str());
    // doc.LoadFile("/home/x/3_cpp/SearchEngine/data/lib/webRaw.dat");
    std::cout << webPath << "\n";

    if (doc.ErrorID()) {
        std::cout << "doc error id" << doc.ErrorID() << "\n";
        Logerror("open webLib file failed");
        return;
    }

    unordered_set<uint64_t, std::hash<uint64_t>, HashCmp> simHashSet;
    XMLElement *root = doc.FirstChildElement("doc");
    while (root) {
        string title, link, desc;
        title = root->FirstChildElement("title")->GetText();
        link = root->FirstChildElement("link")->GetText();
        desc = root->FirstChildElement("desc")->GetText();

        uint64_t u64 = 0;
        vector<pair<string, double>> res;

        _psimhasher->extract(desc, res, topN);
        _psimhasher->make(desc, topN, u64);
        if (simHashSet.count(u64)) {
            root = root->NextSiblingElement("doc");
            continue;
        }
        simHashSet.insert(u64);
        _pageList.emplace_back(title, link, desc, u64);
        root = root->NextSiblingElement("doc");
    }
    return;
}
void PageLibPreprocessor::bulidInvertIndexMap() {
    size_t pageNum = _pageList.size();
    auto stopWords = _conf.getStopWordSet();

    map<string, size_t> docFrequecy; // 记录每个单词在几个文档中出现过
    map<pair<string, int>, size_t, pairCmp>
        termFrequency;   // 记录某个单词在一篇文档中出现的次数
    set<string> wordSet; // 记录所有出现过的单词
    vector<set<string>> wordCounts;
    // 遍历一遍记录上述数据结构中的信息
    // 遍历一遍求出文档中每个单词的权重
    // 遍历一遍对权重进行归一化

    // 第一次遍历
    int pageId = 0;
    for (auto const &page : _pageList) {
        auto wordVec = _wordCutter->cut(page._docContent);
        set<string> localWordSet;
        for (const auto &word : wordVec) {
            if ((stopWords.find(word) == stopWords.end()) && isChinese(word)) {
                localWordSet.insert(word);
                termFrequency[{word, pageId}] += 1;
            }
        }
        // 为每个出现在文档中的单词文档计数加一
        for (const auto &word : localWordSet) {
            // 将文档中出现的单词加入到单词集合中
            wordSet.insert(word);
            docFrequecy[word] += 1;
        }
        wordCounts.emplace_back(localWordSet);
        pageId++;
    }

    std::cout << "sec\n";
    // 第二次遍历
    for (int docId = 0; docId < pageNum; docId++) {
        for (auto const &word : wordCounts[docId]) {
            auto frequency = termFrequency[{word, docId}];
            double idf = ::log2(pageNum / (docFrequecy[word] + 1));
            (_invertIndexLib[word])[docId] = frequency * idf;
        }
    }

    std::cout << "third\n";
    // 第三次遍历
    for (int docId = 0; docId < pageNum; docId++) {
        double totalWeight = 0;
        for (auto &word : wordCounts[docId]) {
            totalWeight += ::pow(_invertIndexLib[word][docId], 2);
        }
        totalWeight = ::sqrt(totalWeight);
        for (auto &word : wordCounts[docId]) {
            _invertIndexLib[word][docId] =
                _invertIndexLib[word][docId] / totalWeight;
        }
    }
}
void PageLibPreprocessor::storeOnDisk() {
    auto webPath = _conf.getConfigMap()["WEB_LIB"];
    auto offsetPath = _conf.getConfigMap()["OFFSET_LIB"];
    auto invertPath = _conf.getConfigMap()["INVERT_IDX_LIB"];

    ofstream ofs(webPath);
    if (!ofs) {
        Logerror("open webLib file failed");
        return;
    }

    ofstream ofs2(offsetPath);
    if (!ofs2) {
        Logerror("open offsetLib file failed");
        return;
    }

    int id = 0;
    long totalSize = 0;
    long pageSize = 0;
    // 写入网页库
    for (auto const &page : _pageList) {
        ostringstream oss;
        oss << "<doc>\n"
            << "<title>" << page._docTitle << "</title>\n"
            << "<link>" << page._docUrl << "</link>\n"
            << "<desc>" << page._docContent << "</desc>\n"
            << "</doc>\n";
        auto str = oss.str();
        pageSize = str.size();
        ofs << str;
        ofs2 << id << " " << totalSize << " " << pageSize << "\n";
        totalSize += pageSize;
        id++;
    }

    ofs.close();
    ofs2.close();

    // 写入倒排索引
    ofstream ofs3(invertPath);
    if (!ofs3) {
        Logerror("open invertLib file failed");
        return;
    }
    for (auto const &index : _invertIndexLib) {
        ofs3 << index.first << " ";
        for (auto const &innerIndex : index.second) {
            ofs3 << innerIndex.first << " " << innerIndex.second << " ";
        }
        ofs3 << "\n";
    }
    ofs3.close();
}
