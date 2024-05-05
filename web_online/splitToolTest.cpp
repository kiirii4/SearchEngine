#include "SplitTool.h"
#include <cctype>
int main() {

    Configuration config("../conf/init.config");
    auto configMap = config.getConfigMap();
    const char *dict_path = configMap["DICT_PATH"].c_str();
    const char *hmm_path = configMap["HMM_PATH"].c_str();
    const char *user_dict_path = configMap["USER_DICT_PATH"].c_str();
    const char *idf_path = configMap["IDF_PATH"].c_str();
    const char *stop_path = configMap["JIEBA_STOP_WORD_PATH"].c_str();
    SplitTool tool(config, dict_path, hmm_path, user_dict_path, idf_path,
                   stop_path);
    string s = "Welcome to beijing 北京还你就是了.";
    auto out = tool.cut(s);

    for (const auto &word : out) {
        std::cout << word << "\n";
    }
    return 0;
}
