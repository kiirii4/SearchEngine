/*
 * Project SearchEngine
 */

#include "WebPageQuery.h"
#include "../shared/mylog.h"
#include "../shared/tinyxml2.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <sstream>
#include <wfrest/json.hpp>

using std::ifstream;
using namespace tinyxml2;
using Json = nlohmann::json;

WebPageQuery *WebPageQuery::_pInstance = nullptr;

struct IndexComp;

struct IndexWithCos {
  public:
    double _cos;
    int _index;
};

struct IndexComp {
    bool operator()(const IndexWithCos &lhs, const IndexWithCos &rhs) {
        return lhs._cos > rhs._cos;
    }
};

struct Cmp {
    bool operator()(const pair<int, double> &p1, const pair<int, double> &p2) {
        return p1.second > p2.second;
    }
};

static string getSummary(const string &str) {
    int nbytes = 0, wordCnt = 0, idx = 0;
    string summary;
    for (; idx < str.size() && (wordCnt < 20); idx += nbytes) {
        if (str[idx] & 0x80) {
            nbytes = 3;
            wordCnt++;
        } else {
            nbytes = 1;
            wordCnt++;
        }
    }
    summary = str.substr(0, idx);
    if (idx < str.size()) {
        summary += "...";
    }
    return summary;
}

WebPageQuery *WebPageQuery::getInstance() {
    if (_pInstance == nullptr) {
        Configuration *config(new Configuration());
        _pInstance = new WebPageQuery();
    }
    return _pInstance;
}

WebPageQuery::WebPageQuery() { ::atexit(&WebPageQuery::destory); }
void WebPageQuery::destory() {
    if (_pInstance != nullptr) {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

void WebPageQuery::dictInit(Configuration *config) {
    WebPageQuery *pInstance = WebPageQuery::getInstance();
    auto configMap = config->getConfigMap();
    const char *dict_path = configMap["DICT_PATH"].c_str();
    const char *hmm_path = configMap["HMM_PATH"].c_str();
    const char *user_dict_path = configMap["USER_DICT_PATH"].c_str();

    pInstance->_splitTool =
        new SplitTool(*config, dict_path, hmm_path, user_dict_path);

    auto confMap = config->getConfigMap();

    const string webLibPath = confMap["WEB_LIB"];
    const string offsetLibPath = confMap["OFFSET_LIB"];
    const string invertIdxPath = confMap["INVERT_IDX_LIB"];

    // 读取倒排索引
    ifstream ifs(invertIdxPath);
    if (!ifs) {
        Logerror("open invert failed.");
        return;
    }
    string line;
    while (getline(ifs, line, '\n')) {
        std::istringstream iss(line);
        string word;
        iss >> word;
        while (iss) {
            size_t id;
            double weight;
            iss >> id >> weight;
            pInstance->_invertIndexLib[word].insert(id);
            pInstance->_weightLib[{word, id}] = weight;
        }
    }

#if 0
    std::cout << "invert size " << pInstance->_invertIndexLib.size()
              << std::endl;
    for (auto &word : pInstance->_invertIndexLib) {
        std::cout << word.first << "\n";
        for (auto &id : word.second) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
#endif
    ifs.close();

    // 读取偏移索引
    ifs.open(offsetLibPath);
    if (!ifs) {
        Logerror("open offsetLib failed.");
        return;
    }

    ifstream ifs2(webLibPath);
    if (!ifs) {
        Logerror("open weblibPath failed.");
        return;
    }

    size_t id, offset, length;
    while (getline(ifs, line, '\n')) {
        std::istringstream iss(line);
        iss >> id >> offset >> length;
        pInstance->_offsetlib[id] = {offset, length};

        char buf[length + 1];
        buf[length] = 0;
        ifs2.seekg(offset);
        ifs2.read(buf, length);
        XMLDocument doc;
        doc.Parse(buf);

        if (doc.ErrorID()) {
            Logerror("parse webLib failed.");
            std::cout << doc.ErrorStr() << std::endl;
            return;
        }
        XMLElement *root = doc.FirstChildElement("doc");
        string title = root->FirstChildElement("title")->GetText();
        string link = root->FirstChildElement("link")->GetText();
        string content = root->FirstChildElement("desc")->GetText();

        string summary = getSummary(content);

        pInstance->_pages.emplace_back(title, link, summary);
    }

#if 0
    std::cout << "pages size " << pInstance->_pages.size() << std::endl;
    std::cout << "offset size: " << pInstance->_offsetlib.size() << std::endl;
#endif
    ifs.close();
    ifs2.close();
}

void WebPageQuery::getQueryWeights(string query) {}

/**
 * @param key
 * @return string
 */
string WebPageQuery::doQuery(const string &key) {
    WebPageQuery *pInstance = WebPageQuery::getInstance();
    // 切分查询语句，得到每个单词的权重

    pInstance->_queryWeights.clear();
    auto wordVec = pInstance->_splitTool->cut(key);
    for (auto &word : wordVec) {
        if (!word.empty())
            std::cout << word << "\n";
    }
    unordered_map<string, int> termFrequency;
    for (auto &word : wordVec) {
        if (word.empty() || word == " " || word == "\n") {
            continue;
        }
        termFrequency[word]++;
    }
    for (auto &word : termFrequency) {
        double weight = word.second;
        pInstance->_queryWeights.push_back({word.first, weight});
    }
    double weightPowSum = 0;
    for (auto &word : pInstance->_queryWeights) {
        weightPowSum += pow(word.second, 2);
    }
    double weightNormlization = 1.0 / sqrt(weightPowSum);
    for (auto &word : pInstance->_queryWeights) {
        word.second *= weightNormlization;
    }

#if 0
    std::cout << "query weights:" << std::endl;
    for (auto &word : pInstance->_queryWeights) {
        std::cout << word.first << " " << word.second << std::endl;
    }

#endif
    // 通过倒排索引库获得候选网页
    vector<set<int>> candidateSequence;
    for (auto &word : pInstance->_queryWeights) {
        set<size_t> &tmp = pInstance->_invertIndexLib[word.first];
        candidateSequence.emplace_back();
        candidateSequence.back().insert(tmp.begin(), tmp.end());
    }
#if 0
    int i = 0;
    std::cout << "candidate set" << "\n";
    for (auto &candidate : candidateSequence) {
        std::cout << "candidate set" << i++ << "\n";
        for (auto &idx : candidate) {
            std::cout << idx << " ";
        }
        std::cout << "\n";
    }
#endif
    // 求出符合交集的网页的下标数组
    set<int> candidateResult;
    if (candidateSequence.size() < pInstance->_queryWeights.size()) {
        return createJson();
    }
    set<int> intersection = candidateSequence[0];
    if (candidateSequence.size() >= 2) {
        for (size_t i = 1; i < candidateSequence.size(); i++) {
            set<int> tmp;
            std::set_intersection(intersection.begin(), intersection.end(),
                                  candidateSequence[i].begin(),
                                  candidateSequence[i].end(),
                                  std::inserter(tmp, tmp.begin()));
            intersection.swap(tmp);
        }
    }

    std::cout << "intersection size: " << intersection.size() << std::endl;
    for (auto &idx : intersection) {
        std::cout << idx << std::endl;
    }

    vector<double> weights;
    vector<int> weightsIndex;
    for (auto &idx : intersection) {
        for (auto &word : pInstance->_queryWeights) {
            weights.push_back(pInstance->_weightLib[{word.first, idx}]);
        }
        pInstance->_webWeights.push_back(weights);
        weightsIndex.push_back(idx);
    }

    auto webWeight = pInstance->_webWeights;
    int topK = pInstance->_topK;
    priority_queue<pair<int, double>, vector<pair<int, double>>, Cmp> pq;
    size_t idx = 0;
    for (auto &weights : webWeight) {
        double cosin = calcCos(weights);
        pq.emplace(idx, cosin);
        idx++;
    }
    vector<int> result;
    for (int i = 0; i < topK && !pq.empty(); i++) {
        result.push_back(pq.top().first);
        pq.pop();
    }
    return createJson(result);
}

/* vector<set<int>> WebPageQuery::getCandidateIdxs() { return candidateSequence;
 * } */

/* vector<int> */
/* WebPageQuery::getIntersection(const vector<set<int>> &candidateSequence) { */
/*     set<int> candidateResult; */
/*     if (candidateSequence.size() < _queryWeights.size()) { */
/*         return vector<int>(); */
/*     } */
/*     set<int> result = candidateSequence[0]; */
/*     if (candidateSequence.size() >= 2) { */
/*         for (size_t i = 1; i < candidateSequence.size(); i++) { */
/*             set<int> tmp; */
/*             std::set_intersection( */
/*                 result.begin(), result.end(), candidateSequence[i].begin(),
 */
/*                 candidateSequence[i].end(), std::inserter(tmp, tmp.begin()));
 */
/*         } */
/*     } */
/*     return vector<int>(result.begin(), result.end()); */
/* } */

/* vector<int> WebPageQuery::indexWeightInit(vector<int> &intersection) { */
/*     vector<double> weights; */
/*     vector<int> weightsIndex; */
/*     for (auto &idx : intersection) { */
/*         for (auto &word : _queryWeights) { */
/*             weights.push_back(_weightLib[{word.first, idx}]); */
/*         } */
/*         _webWeights.push_back(weights); */
/*         weightsIndex.push_back(idx); */
/*     } */
/*     return weightsIndex; */
/* } */

double WebPageQuery::dotProduct(vector<pair<string, double>> &queryWeights,
                                vector<double> &webWeights) {
    double result = 0;
    if (queryWeights.size() != webWeights.size()) {
        // error
        return -1;
    }
    for (size_t i = 0; i < queryWeights.size(); i++) {
        result += queryWeights[i].second * webWeights[i];
    }
    return result;
}

double WebPageQuery::modulo(vector<double> &weights) {
    double result = 0;
    for (auto &weight : weights) {
        result += pow(weight, 2);
    }
    return sqrt(result);
}

double WebPageQuery::modulo(vector<pair<string, double>> &weights) {
    double result = 0;
    for (auto &weight : weights) {
        result += pow(weight.second, 2);
    }
    return sqrt(result);
}

double WebPageQuery::calcCos(vector<double> &webWeight) {
    auto queryWeights = WebPageQuery::getInstance()->_queryWeights;
    return dotProduct(queryWeights, webWeight) /
           (modulo(queryWeights) * modulo(webWeight));
}

string WebPageQuery::createJson(const vector<int> &result) {
    Json json;
    vector<WebPage> pages = getInstance()->_pages;
    string ret;
    for (const auto &idx : result) {
        json["title"] = pages[idx]._docTitle;
        json["url"] = pages[idx]._docUrl;
        json["content"] = pages[idx]._docContent;
        ret += json.dump() + "\n";
    }
    return ret;
}

string WebPageQuery::createJson() {
    Json json;
    return "No answer.\n";
}
