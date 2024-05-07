#include "PageLibPreprocessor.h"
#include "../shared/mylog.h"
#include "../shared/tinyxml2.h"
#include "math.h"
#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace tinyxml2;
using std::ifstream;
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
    Configuration *conf = Configuration::getInstance();
    auto dictPath = conf->getConfigMap()["DICT_PATH"].c_str();
    auto hmmPath = conf->getConfigMap()["HMM_PATH"].c_str();
    auto idfPath = conf->getConfigMap()["IDF_PATH"].c_str();
    auto stopWordsPath = conf->getConfigMap()["JIEBA_STOP_WORD_PATH"].c_str();
    return new simhash::Simhasher(dictPath, hmmPath, idfPath, stopWordsPath);
}

PageLibPreprocessor::PageLibPreprocessor(SplitTool *wordCutter)
    : _wordCutter(wordCutter), _psimhasher(createSimhasher()) {}

void PageLibPreprocessor::cutRedundantPage() {
    Configuration *conf = Configuration::getInstance();
    const string webPath = conf->getConfigMap()["WEB_LIB_RAW"];
    const string offsetPath = conf->getConfigMap()["OFFSET_LIB_RAW"];

    XMLDocument doc;

    // if (doc.ErrorID()) {
    //     std::cout << "doc error id" << doc.ErrorID() << "\n";
    //     Logerror("open webLib file failed");
    //     return;
    // }
    ifstream ifs(offsetPath);
    if (!ifs) {
        Logerror("can't open offsetLib file");
        return;
    }

    ifstream ifs2(webPath);
    if (!ifs2) {
        Logerror("can't open webLib file");
        return;
    }

    string line;
    int docId = 0;
    while (getline(ifs, line, '\n')) {
        size_t id, offset, length;
        std::istringstream iss(line);
        iss >> id >> offset >> length;
        _offsetLib.push_back({docId++, offset, length});
    }

    unordered_set<uint64_t, std::hash<uint64_t>, HashCmp> simHashSet;
    int idx = 0;
    size_t offset = 0;
    int parseCnt = 0;
    std::cout << "offset size:" << _offsetLib.size() << "\n";
    while (idx < _offsetLib.size()) {
        int docNum = 100;
        size_t docSize = 0;
        // 每100篇文章写入一次buf
        while (idx < _offsetLib.size() && ((docNum--) > 0)) {
            docSize += std::get<2>(_offsetLib[idx++]);
        }
        char buf[docSize + 1];
        buf[docSize] = 0;
        ifs2.seekg(offset);
        ifs2.read(buf, docSize);
        offset += docSize;

        // 解析
        XMLDocument doc;
        doc.Parse(buf);
        if (doc.ErrorID()) {
            Loginfo("parse webLib failed.");
            std::cout << doc.ErrorStr() << "\n";
        }
        XMLElement *root = doc.FirstChildElement("doc");

        while (root) {
            if (root->FirstChildElement("title") == nullptr) {
                root = root->NextSiblingElement("doc");
                continue;
            }
            if (root->FirstChildElement("title")->GetText() == nullptr) {
                root = root->NextSiblingElement("doc");
                continue;
            }
            if (root->FirstChildElement("link") == nullptr) {
                root = root->NextSiblingElement("doc");
                continue;
            }
            if (root->FirstChildElement("link")->GetText() == nullptr) {
                root = root->NextSiblingElement("doc");
                continue;
            }
            if (root->FirstChildElement("desc") == nullptr) {
                root = root->NextSiblingElement("doc");
                continue;
            }
            if (root->FirstChildElement("desc")->GetText() == nullptr) {
                root = root->NextSiblingElement("doc");
                continue;
            }
            string title = root->FirstChildElement("title")->GetText();
            string link = root->FirstChildElement("link")->GetText();
            string desc = root->FirstChildElement("desc")->GetText();

            for (auto &c : title) {
                if (c == '<' || c == '>') {
                    c = '\"';
                }
            }

            for (auto &c : link) {
                if (c == '<' || c == '>') {
                    c = '\"';
                }
            }

            for (auto &c : desc) {
                if (c == '<' || c == '>') {
                    c = '\"';
                }
            }

            // int pos = 0;
            // while ((pos = title.find("<", pos) != string::npos)) {
            //     title.replace(pos, 1, "\"");
            //     pos++;
            // }
            // pos = 0;
            // while ((pos = title.find(">", pos) != string::npos)) {
            //     title.replace(pos, 1, "\"");
            //     pos++;
            // }

            // pos = 0;
            // while ((pos = link.find("<", pos) != string::npos)) {
            //     link.replace(pos, 1, "\"");
            //     pos++;
            // }
            // pos = 0;
            // while ((pos = link.find(">", pos) != string::npos)) {
            //     link.replace(pos, 1, "\"");
            //     pos++;
            // }

            // pos = 0;
            // while ((pos = desc.find("<", pos) != string::npos)) {
            //     desc.replace(pos, 1, "\"");
            //     pos++;
            // }
            // pos = 0;
            // while ((pos = desc.find(">", pos) != string::npos)) {
            //     desc.replace(pos, 1, "\"");
            //     pos++;
            // }
            uint64_t u64 = 0;
            vector<pair<string, double>> res;

            _psimhasher->extract(desc, res, topN);
            _psimhasher->make(desc, topN, u64);
            if (simHashSet.count(u64)) {
                root = root->NextSiblingElement("doc");
                continue;
            }
            simHashSet.insert(u64);
            _pageList.emplace_back(title, link, desc);
            root = root->NextSiblingElement("doc");
        }
    }

    // while ((pos = title.find("<", pos) != string::npos)) {
    //     title.replace(pos, 1, "\"");
    //     pos++;
    // }
    // pos = 0;
    // while ((pos = title.find(">", pos) != string::npos)) {
    //     title.replace(pos, 1, "\"");
    //     pos++;
    // }

    // pos = 0;
    // while ((pos = link.find("<", pos) != string::npos)) {
    //     link.replace(pos, 1, "\"");
    //     pos++;
    // }
    // pos = 0;
    // while ((pos = link.find(">", pos) != string::npos)) {
    //     link.replace(pos, 1, "\"");
    //     pos++;
    // }

    // pos = 0;
    // while ((pos = desc.find("<", pos) != string::npos)) {
    //     desc.replace(pos, 1, "\"");
    //     pos++;
    // }
    // pos = 0;
    // while ((pos = desc.find(">", pos) != string::npos)) {
    //     desc.replace(pos, 1, "\"");
    //     pos++;
    // }
}

void PageLibPreprocessor::bulidInvertIndexMap() {
    Configuration *conf = Configuration::getInstance();
    size_t pageNum = _pageList.size();
    auto stopWords = conf->getStopWordSet();

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

    // 第二次遍历
    for (int docId = 0; docId < pageNum; docId++) {
        for (auto const &word : wordCounts[docId]) {
            auto frequency = termFrequency[{word, docId}];
            double idf = ::log2(pageNum / (docFrequecy[word] + 1));
            (*_invertIndexLib)[word][docId] = frequency * idf;
        }
    }

    // 第三次遍历
    for (int docId = 0; docId < pageNum; docId++) {
        double totalWeight = 0;
        for (auto &word : wordCounts[docId]) {
            totalWeight += ::pow((*_invertIndexLib)[word][docId], 2);
        }
        totalWeight = ::sqrt(totalWeight);
        for (auto &word : wordCounts[docId]) {
            (*_invertIndexLib)[word][docId] /= totalWeight;
        }
    }
}
void PageLibPreprocessor::storeOnDisk() {
    Configuration *conf = Configuration::getInstance();
    auto webPath = conf->getConfigMap()["WEB_LIB"];
    auto offsetPath = conf->getConfigMap()["OFFSET_LIB"];
    auto invertPath = conf->getConfigMap()["INVERT_IDX_LIB"];

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
    for (auto const &index : *_invertIndexLib) {
        ofs3 << index.first << " ";
        for (auto const &innerIndex : index.second) {
            ofs3 << innerIndex.first << " " << innerIndex.second << " ";
        }
        ofs3 << "\n";
    }
    ofs3.close();
}
