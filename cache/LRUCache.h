#ifndef _LRUCACHE_H
#define _LRUCACHE_H
#include "../shared/Configuration.h"
#include <list>
#include <string>
#include <unordered_map>
#include <utility>

using std::list;
using std::pair;
using std::string;
using std::unordered_map;

class LRUCache {
  public:
    LRUCache(Configuration *config) : _config() { readFromFile(); }
    void readFromFile();

    void addRecord(const string &key, const string &json);

    string getRecord(const string &key);

    void update(const LRUCache &);

    void writeToFile();

  private:
    void move_to_head(list<pair<string, string>>::iterator it);
    void add_to_head(pair<string, string> &);
    void remove_elem(list<pair<string, string>>::iterator it);
    string remove_tail();

  private:
    size_t _size{0};
    size_t _capacity{0};
    Configuration *_config;
    list<pair<string, string>> _resultList;
    unordered_map<string, list<pair<string, string>>::iterator> _hashmap;
    list<pair<string, string>> _pendingUpdateList;
};

#endif //_LRUCACHE_H
