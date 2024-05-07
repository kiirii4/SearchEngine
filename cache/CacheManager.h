#ifndef _CACHEMANAGER_H
#define _CACHEMANAGER_H
#include "../shared/Configuration.h"
#include "LRUCache.h"
#include <vector>

using std::string;
using std::vector;

class CacheManager {
  public:
    CacheManager(int cacheSize);

    void init(int cahcheSize);

    LRUCache &getCache(int);

    void updateCache();

  private:
    vector<LRUCache> _caches;
};

#endif //_CACHEMANAGER_H
