#include "KeyRecommander.h"
#include <fstream>
#include <iterator>
#include <sstream>

using std::ifstream;
using std::istream_iterator;
using std::istringstream;

KeyRecommander *KeyRecommander::_pInstance = nullptr;

// 1. 求取一个字符占据的字节数
size_t nBytesCode(const char ch);

// 2. 求取一个字符串的字符长度
std::size_t length(const std::string &str);

// 3. 中英文通用的最小编辑距离算法
int editDistance(const std::string &lhs, const std::string &rhs);

// 4.切分字符
vector<string> cutChar(const string &str);

KeyRecommander *KeyRecommander::getInstance() {
    if (_pInstance == nullptr) {
        _pInstance = new KeyRecommander();
    }
    return _pInstance;
}

void KeyRecommander::destory() {
    if (_pInstance != nullptr) {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

void KeyRecommander::dictInit() {
    KeyRecommander *pInstance = KeyRecommander::getInstance();
    Configuration *conf = Configuration::getInstance();
    auto confMap = conf->getConfigMap();

    auto enDictPath = confMap["EN_DICT"];
    auto chDictPath = confMap["CH_DICT"];
    auto chIdxPath = confMap["CH_IDX"];
    auto enIdxPath = confMap["EN_IDX"];

    ifstream ifs(enDictPath);
    if (!ifs) {
        Logerror("open enDict failed.");
        return;
    }
    string line;
    while (getline(ifs, line, '\n')) {
        istringstream iss(line);
        string word, freq;
        iss >> word >> freq;
        (pInstance->_enDict).emplace_back(word, stoi(freq));
    }
    std::cout << "en dict size: " << pInstance->_enDict.size() << "\n";
    ifs.close();

    ifs.open(chDictPath);
    if (!ifs) {
        Logerror("open chDict failed.");
        return;
    }
    while (getline(ifs, line, '\n')) {
        istringstream iss(line);
        string word, freq;
        iss >> word >> freq;
        (pInstance->_cnDict).emplace_back(word, stoi(freq));
    }
    std::cout << "cn dict size: " << pInstance->_cnDict.size() << "\n";
    ifs.close();

    ifs.open(chIdxPath);
    if (!ifs) {
        Logerror("open chIdx failed.");
        return;
    }

    string chars;
    while (getline(ifs, line, '\n')) {
        istringstream iss(line);
        iss >> chars;
        pInstance->_cnIdx[chars].insert(std::istream_iterator<size_t>(iss),
                                        std::istream_iterator<size_t>());
    }
    std::cout << "cn idx size: " << pInstance->_cnIdx.size() << "\n";
    ifs.close();

    ifs.open(enIdxPath);
    if (!ifs) {
        Logerror("open enIdx failed.");
    }
    while (getline(ifs, line, '\n')) {
        istringstream iss(line);
        iss >> chars;
        pInstance->_enIdx[chars].insert(std::istream_iterator<size_t>(iss),
                                        std::istream_iterator<size_t>());
    }

    std::cout << "en idx size: " << pInstance->_enIdx.size() << "\n";
    ifs.close();
}

string KeyRecommander::doQuery(const string &msg) {
#ifdef DEBUG
    std::cout << "begin query, msg :" << msg << "\n";
#endif
    KeyRecommander *pInstance = KeyRecommander::getInstance();
    vector<string> charas = cutChar(msg);
#ifdef DEBUG
    std::cout << "chara is :" << " ";
    for (auto chara : charas) {
        std::cout << chara << "\n";
    }
#endif
    set<size_t> chWords;
    set<size_t> enWords;
    for (const auto &chara : charas) {
        if (chara[0] & 0x80) {
            set<size_t> &temp = pInstance->_cnIdx[chara];
            chWords.insert(temp.begin(), temp.end());
        } else {
            set<size_t> &temp = pInstance->_enIdx[chara];
            enWords.insert(temp.begin(), temp.end());
        }
    }
    for (const auto wordIdx : chWords) {
        string word = pInstance->_cnDict[wordIdx].first;
        int dist = editDistance(msg, word);
        int freq = pInstance->_cnDict[wordIdx].second;
        pInstance->_pq.emplace(word, freq, dist);
    }
    for (const auto wordIdx : enWords) {
        string word = pInstance->_enDict[wordIdx].first;
        int dist = editDistance(msg, word);
        int freq = pInstance->_cnDict[wordIdx].second;
        pInstance->_pq.emplace(word, freq, dist);
    }

    string result;
    int topN = pInstance->_topN;
    while (topN-- && !pInstance->_pq.empty()) {
        result += pInstance->_pq.top()._word + " ";
        pInstance->_pq.pop();
    }
    result += "\n";
    std::cout << "result = " << result;
    priority_queue<CandidateResult> empty;
    pInstance->_pq.swap(empty);
    return result;
}

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

int triple_min(const int &a, const int &b, const int &c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

vector<string> cutChar(const string &str) {
    size_t nbyte = nBytesCode(str[0]);
    size_t len = length(str);
    vector<string> result;
    for (int i = 0; i < len; i += nbyte) {
        result.emplace_back(str.substr(i, nbyte));
    }
    return result;
}

int editDistance(const std::string &lhs,
                 const std::string &rhs) { // 计算最小编辑距离-包括处理中英文
    size_t lhs_len = length(lhs);
    size_t rhs_len = length(rhs);
    int editDist[lhs_len + 1][rhs_len + 1];
    for (size_t idx = 0; idx <= lhs_len; ++idx) {
        editDist[idx][0] = idx;
    }

    for (size_t idx = 0; idx <= rhs_len; ++idx) {
        editDist[0][idx] = idx;
    }

    std::string sublhs, subrhs;
    for (std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len;
         ++dist_i, ++lhs_idx) {
        size_t nBytes = nBytesCode(lhs[lhs_idx]);
        sublhs = lhs.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes - 1);

        for (std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len;
             ++dist_j, ++rhs_idx) {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes - 1);
            if (sublhs == subrhs) {
                editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
            } else {
                editDist[dist_i][dist_j] =
                    triple_min(editDist[dist_i][dist_j - 1] + 1,
                               editDist[dist_i - 1][dist_j] + 1,
                               editDist[dist_i - 1][dist_j - 1] + 1);
            }
        }
    }
    return editDist[lhs_len][rhs_len];
}
