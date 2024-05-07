#include "CacheManager.h"
#include "../shared/mylog.h"

CacheManager *CacheManager::getInstance() {
    if (_pInstance == nullptr) {
        Configuration *config = Configuration::getInstance();
        auto &confMap = config->getConfigMap();
        int cacheSize = std::stoi(confMap["THREAD_NUM"]);
        _pInstance = new CacheManager(cacheSize);
    }
    return _pInstance;
}

CacheManager::CacheManager(int cacheSize) {
    ::atexit(&CacheManager::destory);
    init(cacheSize);
}

void CacheManager::init(int cacheSize) {
    _caches.reserve(cacheSize + 1);
    for (int i = 0; i < cacheSize; ++i) {
        _caches.push_back(LRUCache());
    }
}

LRUCache &CacheManager::getCache(int idx) { return _caches[idx]; }

void CacheManager::updateCache() { return; }
