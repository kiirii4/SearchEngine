/**
 * Project SearchEngine
 */

#include "Configuration.h"
#include <fstream>
#include <sstream>

static string configPath = "../conf/init.config";

Configuration::Configuration() {
    ::atexit(&Configuration::destory);
    std::ifstream ifs(configPath);
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
    // string ch_stop = _configMap["CH_STOP_WORD_PATH"];
    // string en_stop = _configMap["EN_STOP_WORD_PATH"];
    std::ifstream ifs(
        "/home/x/3_cpp/SearchEngine/data/stop_word/stop_words_zh.txt");
    // std::cout << _configMap["CH_STOP_WORD_PATH"] << std::endl;
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
    ifs.open("/home/x/3_cpp/SearchEngine/data/stop_word/stop_words_eng.txt");
    // std::cout << _configMap["EN_STOP_WORD_PATH"] << std::endl;
    if (ifs.is_open()) {
        while (ifs) {
            ifs >> word;
            _stopWordSet.insert(word);
        }
    } else {
        Logerror("open eng stop word file failed");
    }
    ifs.close();
    _stopWordSet.insert(" ");
}
