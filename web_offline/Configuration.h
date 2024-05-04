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
    Configuration(const string &filepath);
    unordered_map<string, string> &getConfigMap();
    set<string> &getStopWordSet();

  private:
    void stop_wordsInit();
    unordered_map<string, string> _configMap;
    set<string> _stopWordSet;
};

inline unordered_map<string, string> &
Configuration::Configuration::getConfigMap() {
    return _configMap;
}

inline set<string> &Configuration::getStopWordSet() { return _stopWordSet; }
#endif //_CONFIGURATION_H
