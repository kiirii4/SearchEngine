/**
 * Project SearchEngine
 */

#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::unordered_set;
class Configuration {
  public:
    Configuration(const string &filepath);
    unordered_map<string, string> getConfigMap();
    unordered_set<string> getStopWordSet();

  private:
    void stop_wordsInit();
    unordered_map<string, string> _configMap;
    unordered_set<string> _stopWordSet;
};

#endif //_CONFIGURATION_H
