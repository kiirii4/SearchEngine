#include "DictProducer.h"
#include "../shared/mylog.h"
#include <cctype>
#include <fstream>

using std::ifstream;
using std::ofstream;

size_t nBytesCode(const char ch) {
    if (ch & (1 << 7)) {
        int nBytes = 1;
        for (int idx = 0; idx != 6; ++idx) {
            if (ch & (1 << (6 - idx))) {
                ++nBytes;
            } else
                break;
        }
        return nBytes;
    }
    return 1;
}

std::size_t length(const std::string &str) {
    std::size_t ilen = 0;
    for (std::size_t idx = 0; idx != str.size(); ++idx) {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

DictProducer::DictProducer(SplitTool *tool, DirScanner *scanner)
    : _cuttor(tool), _scanner(scanner) {}

void DictProducer::buildEnDict() {
    auto config = Configuration::getInstance();
    _dict.clear();
    _index.clear();
    _scanner->clear();
    auto stopwords = config->getStopWordSet();
    auto engCorpus = config->getConfigMap()["EN_PATH"];
    _scanner->traverse(engCorpus);
    vector<string> files = _scanner->getFiles();

    std::cout << "en files\n";
    for (const auto &file : files) {
        std::cout << file << "\n";
    }

    map<string, int> dict;
    for (auto file : files) {
        std::cout << "readfile" << file << "\n";
        ifstream ifs(file);
        string line;
        while (getline(ifs, line)) {
            if (line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1, 1);
            }
            vector<string> words = _cuttor->cut(line);
            for (auto word : words) {
                if (std::isdigit(word[0]) || std::isspace(word[0]) ||
                    std::ispunct(word[0]) || stopwords.count(word)) {
                    continue;
                }
                dict[word]++;
            }
        }
    }
    _dict = vector<pair<string, int>>(dict.begin(), dict.end());
    createIndex();
    store("EN");
    return;
}

void DictProducer::buildCnDict() {
    auto config = Configuration::getInstance();
    _dict.clear();
    _index.clear();
    _scanner->clear();
    auto stopwords = config->getStopWordSet();
    auto chCorpus = config->getConfigMap()["CH_PATH"];
    _scanner->traverse(chCorpus);
    vector<string> files = _scanner->getFiles();

    std::cout << "ch files\n";
    for (const auto &file : files) {
        std::cout << file << "\n";
    }

    map<string, int> dict;
    for (auto file : files) {
        std::cout << "readfile" << file << "\n";
        ifstream ifs(file);
        string line;
        while (getline(ifs, line)) {
            if (line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1, 1);
            }
            vector<string> words = _cuttor->cut(line);
            for (auto word : words) {
                if (std::isgraph(word[0]) || std::isspace(word[0]) ||
                    stopwords.count(word)) {
                    continue;
                }
                dict[word]++;
            }
        }
    }
    _dict = vector<pair<string, int>>(dict.begin(), dict.end());
    createIndex();
    store("ZH");
    return;
}

void DictProducer::createIndex() {
    for (size_t j = 0; j < _dict.size(); j++) {
        string word = _dict[j].first;
        size_t len = length(word);
        size_t bytes = nBytesCode(word[0]);
        for (size_t i = 0; i < len; i += bytes) {
            _index[word.substr(i, bytes)].insert(j);
        }
    }
}

void DictProducer::store(const string &str) {
    auto confMap = Configuration::getInstance()->getConfigMap();
    string dictPath = (str == "EN") ? confMap["EN_DICT"] : confMap["CH_DICT"];
    string idxPath = (str == "EN") ? confMap["EN_IDX"] : confMap["CH_IDX"];

    ofstream ofs(dictPath);
    if (!ofs) {
        Logerror("open dictPath failed");
        return;
    }
    for (const auto &wordPair : _dict) {
        ofs << wordPair.first << " " << wordPair.second << "\n";
    }
    ofs.close();

    ofs.open(idxPath);
    if (!ofs) {
        Logerror("open dictIdx failed");
        return;
    }
    for (const auto &charaIdx : _index) {
        ofs << charaIdx.first << " ";
        for (const auto &idx : charaIdx.second) {
            ofs << idx << " ";
        }
        ofs << "\n";
    }
    ofs.close();
}
