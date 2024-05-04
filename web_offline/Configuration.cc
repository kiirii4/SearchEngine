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
    string key, value;
    if (ifs.is_open()) {
        while (getline(ifs, line, '\n')) {
            std::stringstream ss(line);
            ss >> key >> value;
            _configMap[key] = value;
        }
    } else {
        Logerror("open config file failed");
    }
    stop_wordsInit();
    ifs.close();
}

void Configuration::stop_wordsInit() {
    std::ifstream ifs(_configMap["JIEBA_STOP_WORD_PATH"]);
    std::string word;
    if (ifs.is_open()) {
        while (ifs) {
            ifs >> word;
            _stopWordSet.insert(word);
        }
    } else {
        Logerror("open stop word file failed");
    }
    ifs.close();
    _stopWordSet.insert(" ");
}

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
