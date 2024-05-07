#ifndef _CACHEMANAGER_H
#define _CACHEMANAGER_H
#include "../shared/Configuration.h"
#include "LRUCache.h"
#include <vector>

using std::string;
using std::vector;

class CacheManager {
  public:
    static CacheManager *getInstance();

    LRUCache &getCache(int);

    void updateCache();

  private:
    void init(int cahcheSize);
    CacheManager(int cacheSize);
    ~CacheManager() {}
    static void destory();

  private:
    static CacheManager *_pInstance;
    vector<LRUCache> _caches;
};

#endif //_CACHEMANAGER_H
