/**
 * Project SearchEngine
 */

#ifndef _WEBPAGEQUERY_H
#define _WEBPAGEQUERY_H

#include "../shared/SplitTool.h"
#include "../shared/WebPage.h"
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::pair;
using std::priority_queue;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;
// using Json = nlohmann::json;

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

struct IndexComp;

struct IndexWithCos {
  public:
    double _cos;
    int _index;
    friend IndexComp;
};

struct IndexComp {
    bool operator()(const IndexWithCos &lhs, const IndexWithCos &rhs) {
        return lhs._cos > rhs._cos;
    }
};

class WebPageQuery {
  public:
    WebPageQuery(const string &, SplitTool &tool, Configuration &);

    /**
     * @param key
     */
    static string doQuery(string key);

  private:
    int topK = 20;
    vector<WebPage> _pages;
    unordered_map<int, pair<int, int>> _offsetlib;
    unordered_map<string, set<int>> _invertIndexLib;
    unordered_map<pair<string, int>, double, PairHash> _weightLib;
    vector<pair<string, double>> _queryWeights;
    vector<vector<double>> _webWeights;
    // priority_queue<IndexWithCos, vector<IndexWithCos>, IndexComp>
    // _topRelated;
    // vector<IndexWithCos> _candidateIdxs;

    SplitTool &_splitTool;
    Configuration &_config;

  private:
    /**
     * @brief 加载网页库，网页偏移库，倒排索引库
     */
    void loadLibrary();

    // 切分查询语句，得到每个单词的权重
    void getQueryWeights(string query);

    // 通过倒排索引库获得候选网页
    vector<set<int>> getCandidateIdxs();

    // 求出符合交集的网页的下标数组
    vector<int> getIntersection(const vector<set<int>> &candidateSequence);

    // 通过交集查权重索引，获得每个网页中待查单词的权重, 返回网页权重数组的索引
    vector<int> indexWeightInit(vector<int> &intersection);

    string createJson(const vector<int> &result);

    // 传入一个网页中待查词的权重，计算与查询语句的余弦相似度
    double calcCos(vector<double> &webWeight);

    // 求内积
    double dotProduct(vector<pair<string, double>> &queryWeights,
                      vector<double> &webWeights);

    // 求模
    double modulo(vector<double> &);

    double modulo(vector<pair<string, double>> &);
};

#endif //_WEBPAGEQUERY_H
