#ifndef _WEBPAGEQUERY_H
#define _WEBPAGEQUERY_H

#include "../shared/Configuration.h"
#include "../shared/SplitTool.h"
#include "../shared/WebPage.h"
#include <memory>
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
using std::unique_ptr;
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

class WebPageQuery {
  public:
    static string doQuery(const string &key);

    static WebPageQuery *getInstance();

    static void dictInit();

  private:
    static WebPageQuery *_pInstance;
    WebPageQuery();
    ~WebPageQuery() {
        if (_splitTool != nullptr) {
            delete _splitTool;
            _splitTool = nullptr;
        }
    }

    static void destory();

  private:
    int _topK = 20;
    vector<WebPage> _pages;
    unordered_map<int, pair<size_t, size_t>> _offsetlib;
    unordered_map<string, set<size_t>> _invertIndexLib;
    unordered_map<pair<string, size_t>, double, PairHash> _weightLib;
    vector<pair<string, double>> _queryWeights;
    vector<vector<double>> _webWeights;

    SplitTool *_splitTool = nullptr;

  private:
    /**
     * @brief 加载网页库，网页偏移库，倒排索引库
     */
    void init();

    // 切分查询语句，得到每个单词的权重
    void getQueryWeights(string query);

    // 通过倒排索引库获得候选网页
    vector<set<int>> getCandidateIdxs();

    // 求出符合交集的网页的下标数组
    vector<int> getIntersection(const vector<set<int>> &candidateSequence);

    // 通过交集查权重索引，获得每个网页中待查单词的权重,
    // 返回网页权重数组的索引
    vector<int> indexWeightInit(vector<int> &intersection);

    static string createJson(const vector<int> &result);

    static string createJson();

    // 传入一个网页中待查词的权重，计算与查询语句的余弦相似度
    static double calcCos(vector<double> &webWeight);

    // 求内积
    static double dotProduct(vector<pair<string, double>> &queryWeights,
                             vector<double> &webWeights);

    // 求模
    static double modulo(vector<double> &);

    static double modulo(vector<pair<string, double>> &);
};

#endif //_WEBPAGEQUERY_Hconfiguration
