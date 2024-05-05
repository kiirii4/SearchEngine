/**
 * Project SearchEngine
 */

#include "Configuration.h"
#include <fstream>
#include <iostream>
#include <sstream>

Configuration::Configuration(const string &filepath) {
    std::ifstream ifs(filepath);
    std::string line;
    if (ifs.is_open()) {
        while (getline(ifs, line, '\n')) {
            std::stringstream ss(line);
            string key;
            string value;
            ss >> key;
            ss >> value;
            _configMap[key] = value;
        }
    }
    stop_wordsInit();
    ifs.close();
}

void Configuration::stop_wordsInit() {
    std::ifstream ifs(_configMap["JIEBA_STOP_WORD_PATH"]);
    std::string line;
    if (ifs.is_open()) {
        while (getline(ifs, line, '\n')) {
            _stopWordSet.insert(line);
        }
    } else {
        std::cerr << "open stop_words file failed" << std::endl;
    }
    ifs.close();
}

unordered_map<string, string> Configuration::getConfigMap() {
    return _configMap;
}

set<string> Configuration::getStopWordSet() { return _stopWordSet; }

// int main() {
//     Configuration config("dict.config");
//     auto stop_words = config.getStopWordSet();
//     auto config_map = config.getConfigMap();

//     for (auto &it : (*config_map)) {
//         std::cout << it.first << " " << it.second << std::endl;
//     }
//     std::cout << "stop words" << "\n";
//     for (auto &it : (*stop_words)) {
//         std::cout << it << std::endl;
//     }
// }
