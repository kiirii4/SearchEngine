/**
 * Project SearchEngine
 */

#ifndef _KEYRECOMMANDER_H
#define _KEYRECOMMANDER_H
#include "../shared/Configuration.h"
#include "../shared/SplitTool.h"
#include "CandidateResult.hpp"
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::pair;
using std::priority_queue;
using std::string;
using std::unordered_map;
using std::vector;

class KeyRecommander {
  public:
    static KeyRecommander *getInstance();
    static void dictInit(Configuration *config);
    static string doQuery(const string &msg);

  private:
    KeyRecommander() { ::atexit(&KeyRecommander::destory); }
    ~KeyRecommander() {}
    static void destory();

  private:
    static KeyRecommander *_pInstance;
    vector<pair<string, size_t>> _enDict;
    vector<pair<string, size_t>> _cnDict;
    unordered_map<string, set<size_t>> _enIdx;
    unordered_map<string, set<size_t>> _cnIdx;
    int _topN = 10;
    priority_queue<CandidateResult> _pq;
};

#endif //_KEYRECOMMANDER_H
