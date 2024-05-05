/**
 * Project SearchEngine
 */

#include "WebPageQuery.h"
#include <algorithm>
#include <cmath>

/**
 * WebPageQuery implementation
 */

struct Cmp {
    bool operator()(const pair<int, double> &p1, const pair<int, double> &p2) {
        return p1.second > p2.second;
    }
};

WebPageQuery::WebPageQuery(const string &query, SplitTool &tool,
                           Configuration &config)
    : _splitTool(tool), _config(config) {
    loadLibrary();
    getQueryWeights(query);
}

void WebPageQuery::getQueryWeights(string query) {
    auto wordVec = _splitTool.cut(query);
    unordered_map<string, int> termFrequency;
    for (auto &word : wordVec) {
        termFrequency[word]++;
    }
    for (auto &word : termFrequency) {
        double weight = word.second * log2(0.5);
        _queryWeights.push_back({word.first, weight});
    }
    double weightPowSum = 0;
    for (auto &word : _queryWeights) {
        weightPowSum += pow(word.second, 2);
    }
    double weightNormlization = 1.0 / sqrt(weightPowSum);
    for (auto &word : _queryWeights) {
        word.second *= weightNormlization;
    }
}

/**
 * @param key
 * @return string
 */
string WebPageQuery::doQuery(string key) {
    getQueryWeights(key);
    auto candidateIdxs = getCandidateIdxs();
    auto intersection = getIntersection(candidateIdxs);
    auto weightIndex = indexWeightInit(intersection);
    priority_queue<pair<int, double>, vector<pair<int, double>>, Cmp> pq;
    size_t idx = 0;
    for (auto &weights : _webWeights) {
        auto cosin = calcCos(weights);
        pq.push({weightIndex[idx], cosin});
        idx++;
    }
    vector<int> result;
    for (int i = 0; i < topK; i++) {
        result.push_back(pq.top().first);
        pq.pop();
    }
    return createJson(result);
}

void WebPageQuery::loadLibrary() {}

vector<set<int>> WebPageQuery::getCandidateIdxs() {
    vector<set<int>> candidateSequence;
    for (auto &word : _queryWeights) {
        candidateSequence.push_back(_invertIndexLib[word.first]);
    }
    return candidateSequence;
}

vector<int>
WebPageQuery::getIntersection(const vector<set<int>> &candidateSequence) {
    set<int> candidateResult;
    if (candidateSequence.size() < _queryWeights.size()) {
        return vector<int>();
    }
    set<int> result = candidateSequence[0];
    if (candidateSequence.size() >= 2) {
        for (size_t i = 1; i < candidateSequence.size(); i++) {
            set<int> tmp;
            std::set_intersection(
                result.begin(), result.end(), candidateSequence[i].begin(),
                candidateSequence[i].end(), std::inserter(tmp, tmp.begin()));
        }
    }
    return vector<int>(result.begin(), result.end());
}

vector<int> WebPageQuery::indexWeightInit(vector<int> &intersection) {
    vector<double> weights;
    vector<int> weightsIndex;
    for (auto &idx : intersection) {
        for (auto &word : _queryWeights) {
            weights.push_back(_weightLib[{word.first, idx}]);
        }
        _webWeights.push_back(weights);
        weightsIndex.push_back(idx);
    }
    return weightsIndex;
}

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
    return dotProduct(_queryWeights, webWeight) /
           (modulo(_queryWeights) * modulo(webWeight));
}

string WebPageQuery::createJson(const vector<int> &result) { return ""; }
