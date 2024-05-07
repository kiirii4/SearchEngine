/**
 * Project SearchEngine
 */
#include "SplitTool.h"
#include <cctype>

SplitTool::SplitTool(Configuration &conf, const char *dict_path,
                     const char *hmm_path, const char *user_dict_path)
    : _conf(conf),
      _jieba(new cppjieba::Jieba(dict_path, hmm_path, user_dict_path)) {}

SplitTool::SplitTool(Configuration &conf) : _conf(conf) {
    const string &dict_path = _conf.getConfigMap()["DICT_PATH"];
    const string &hmm_path = _conf.getConfigMap()["HMM_PATH"];
    const string &user_dict_path = _conf.getConfigMap()["USER_DICT_PATH"];
    _jieba = new cppjieba::Jieba(dict_path, hmm_path, user_dict_path);
}
vector<string> SplitTool::cut(const string &str) {
    vector<string> out;
    vector<string> result;
    _jieba->Cut(str, out, true);
    auto stopWordList = _conf.getStopWordSet();
    for (auto &word : out) {
        if (stopWordList.find(word) == stopWordList.end()) {
            result.push_back(word);
        }
    }
    return result;
}
