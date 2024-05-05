/**
 * Project SearchEngine
 */
#include "SplitTool.h"
#include <cctype>

SplitTool::SplitTool(Configuration &conf, const char *dict_path,
                     const char *hmm_path, const char *user_dict_path)
    : _conf(conf), _jieba(dict_path, hmm_path, user_dict_path) {}

vector<string> SplitTool::cut(const string &str) {
    vector<string> out;
    vector<string> result;
    _jieba.Cut(str, out, true);
    auto stopWordList = _conf.getStopWordSet();
    for (auto &word : out) {
        if (stopWordList.find(word) == stopWordList.end()) {
            result.push_back(word);
        }
    }
    return result;
}
