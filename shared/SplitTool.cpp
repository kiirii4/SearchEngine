/**
 * Project SearchEngine
 */
#include "SplitTool.h"
#include <cctype>

SplitTool::SplitTool() {
    Configuration *conf = Configuration::getInstance();
    const char *dict_path = conf->getConfigMap()["DICT_PATH"].c_str();
    const char *hmm_path = conf->getConfigMap()["HMM_PATH"].c_str();
    const char *user_dict_path = conf->getConfigMap()["USER_DICT_PATH"].c_str();
    _jieba = new cppjieba::Jieba(dict_path, hmm_path, user_dict_path);
}
vector<string> SplitTool::cut(const string &str) {
    auto stopWordList = Configuration::getInstance()->getStopWordSet();
    vector<string> out;
    vector<string> result;
    _jieba->Cut(str, out, true);
    for (auto &word : out) {
        if (stopWordList.find(word) == stopWordList.end()) {
            result.push_back(word);
        }
    }
    return result;
}
