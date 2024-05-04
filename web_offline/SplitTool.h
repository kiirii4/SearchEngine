#ifndef _SPLITTOOL_H
#define _SPLITTOOL_H

#include "../../include/cppjieba/Jieba.hpp"
#include "Configuration.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class SplitTool {
  public:
    vector<string> cut(const string &str);
    SplitTool(Configuration &conf, const char *, const char *, const char *,
              const char *, const char *);

  private:
    cppjieba::Jieba _jieba;
    vector<string> _result;
    Configuration &_conf;
};

#endif //_SPLITTOOL_H
