#ifndef _PAGELIBPREPROCESSOR_H
#define _PAGELIBPREPROCESSOR_H

#include "../../include/simhash/Simhasher.hpp"
#include "../shared/Configuration.h"
#include "../shared/SplitTool.h"
#include "../shared/WebPage.h"
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
using std::pair;
using std::set;
using std::string;
using std::unique_ptr;
using std::unordered_map;

class PageLibPreprocessor {
  public:
    PageLibPreprocessor(Configuration &conf, SplitTool *wordCutter);

    void cutRedundantPage();

    void bulidInvertIndexMap();

    void storeOnDisk();

  private:
    simhash::Simhasher *createSimhasher();

  private:
    Configuration &_conf;
    unique_ptr<simhash::Simhasher> _psimhasher;
    vector<WebPage> _pageList;
    // vector<std::tuple<int, size_t, size_t>> _offsetLib;
    unordered_map<string, map<int, double>> _invertIndexLib;
    SplitTool *_wordCutter;
};

#endif //_PAGELIBPREPROCESSOR_H
