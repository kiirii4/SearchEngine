#include "LRUCache.h"
#include "../shared/Configuration.h"
#include <fstream>
#include <wfrest/json.hpp>

using Json = nlohmann::json;
using std::ifstream;

void LRUCache::addRecord(const string &key, const string &json) {
    if (_hashmap.find(key) == _hashmap.end()) {
        _resultList.push_front({key, json});
        _hashmap[key] = _resultList.begin();
        ++_size;
        if (_size > _capacity) {
            auto removed = remove_tail();
            _hashmap.erase(removed);
            --_size;
        }
    } else {
        move_to_head(_hashmap[key]);
        (*_hashmap[key]).second = json;
    }
}

string LRUCache::getRecord(const string &key) {
    if (_hashmap.find(key) == _hashmap.end()) {
        std::cout << "can't find " << key << " in cache." << std::endl;
        static string ret{};
        return ret;
    }

    auto &elem = _hashmap[key];
    move_to_head(elem);
    return (*elem).second;
}

void LRUCache::move_to_head(list<pair<string, string>>::iterator it) {
    _resultList.splice(_resultList.begin(), _resultList, it);
}

void LRUCache::remove_elem(list<pair<string, string>>::iterator it) {
    _resultList.erase(it);
}
string LRUCache::remove_tail() {
    auto key = (*prev(_resultList.end())).first;
    _resultList.erase(prev(_resultList.end()));
    return key;
}

void LRUCache::update(const LRUCache &) {
    // todo
    return;
}

void LRUCache::writeToFile() {
    // todo
    return;
}

void LRUCache::readFromFile() {
    Configuration *config = Configuration::getInstance();
    auto &confMap = config->getConfigMap();
    const string &cacheFile = confMap["CACHE_PATH"];

    ifstream ifs(cacheFile);
    if (!ifs) {
        Logerror("open cache file failed.");
        return;
    }

    Json json = Json::parse(ifs);
    // todo
}
