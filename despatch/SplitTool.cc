/**
 * Project SearchEngine
 */
#include "SplitTool.h"
#include <cctype>
#include <iostream>
// vector<string> SplitTool::cut(const string &str) {
//     size_t j = 0;
//     size_t size = 0;
//     for (size_t i = 0; i <= str.size();) {
//         if (i == str.size() || std::isspace(str[i]) ||
//             (i > 0 && (str[i - 1] & 0x80) != (str[i] & 0x80))) {
//             if (str[i - 1] & 0x80) {
//                 _chBuf.push_back(str.substr(j, i - j));
//             } else if (std::isspace(str[i]) || i == str.size()) {
//                 _result.push_back(str.substr(j, i - j + 1));
//             } else {
//                 _result.push_back(str.substr(j, i - j));
//             }
//             j = i;
//         }
//         size = (str[i] & 0x80) ? 3 : 1;
//         i += size;
//     }
//     return _result;
// }

SplitTool::SplitTool(Configuration &conf, const char *dict_path,
                     const char *hmm_path, const char *user_dict_path,
                     const char *idf_path, const char *stop_word_path)
    : _conf(conf),
      _jieba(dict_path, hmm_path, user_dict_path, idf_path, stop_word_path) {}

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
