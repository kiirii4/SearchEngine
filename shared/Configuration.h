/**
 * Project SearchEngine
 */

#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H
#include "mylog.h"
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

using std::set;
using std::shared_ptr;
using std::string;
using std::unordered_map;
class Configuration {
  public:
    static Configuration *getInstance() {
        if (_pInstance == nullptr) {
            _pInstance = new Configuration();
        }
        return _pInstance;
    }
    unordered_map<string, string> &getConfigMap();
    set<string> &getStopWordSet();

  private:
    Configuration();
    ~Configuration() {}
    static void destory() {
        if (_pInstance != nullptr) {
            delete _pInstance;
            _pInstance = nullptr;
        }
    }
    void stop_wordsInit();

  private:
    static Configuration *_pInstance;
    unordered_map<string, string> _configMap;
    set<string> _stopWordSet;
};

inline unordered_map<string, string> &
Configuration::Configuration::getConfigMap() {
    return _configMap;
}

inline set<string> &Configuration::getStopWordSet() { return _stopWordSet; }
#endif //_CONFIGURATION_H
