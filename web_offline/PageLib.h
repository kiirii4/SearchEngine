#ifndef _PAGELIB_H
#define _PAGELIB_H

#include "../shared/Configuration.h"
#include "../shared/DirScanner.h"
#include "FileProcessor.h"
#include <map>
#include <utility>

using std::map;
using std::pair;
class PageLib {
  public:
    PageLib(Configuration &conf) : _conf(conf) {};

    void create();

    void store();

  private:
    Configuration &_conf;
    DirScanner _dirScanner;
    FileProcessor _fileProcessor;
    vector<string> _pages;
    map<int, pair<int, int>> _offsetLib;
};

#endif //_PAGELIB_H
