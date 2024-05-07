#include "CacheManager.h"
#include "../shared/mylog.h"

CacheManager::CacheManager(int cacheSize) { init(cacheSize); }

void CacheManager::init(int cacheSize) {
    _caches.reserve(cacheSize + 1);
    for (int i = 0; i < cacheSize; ++i) {
        _caches.push_back(LRUCache());
    }
}

LRUCache &CacheManager::getCache(int idx) { return _caches[idx]; }

void CacheManager::updateCache() { return; }
