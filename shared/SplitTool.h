#ifndef _SPLITTOOL_H
#define _SPLITTOOL_H

#include "../../include/cppjieba/Jieba.hpp"
#include "Configuration.h"
#include <memory>
#include <string>
#include <vector>
using std::string;
using std::unique_ptr;
using std::vector;

class SplitTool {
  public:
    vector<string> cut(const string &str);
    SplitTool();
    ~SplitTool() {
        if (_jieba != nullptr) {
            delete _jieba;
            _jieba = nullptr;
        }
    }

  private:
    cppjieba::Jieba *_jieba;
};

#endif //_SPLITTOOL_H
