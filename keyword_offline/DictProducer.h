#ifndef _DICTPRODUCER_H
#define _DICTPRODUCER_H

#include "../shared/Configuration.h"
#include "../shared/DirScanner.h"
#include "../shared/SplitTool.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

using std::map;
using std::pair;
using std::string;
using std::vector;

class DictProducer {

  public:
    DictProducer(Configuration &config, SplitTool *tool, DirScanner *scanner);

    void buildEnDict();

    void buildCnDict();

  private:
    void createIndex();
    void store(const string &);
    Configuration &_config;
    DirScanner *_scanner;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index;
    SplitTool *_cuttor;
};

#endif //_DICTPRODUCER_H
