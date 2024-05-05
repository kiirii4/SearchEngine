#include "DictProducer.h"

void test() {
    DirScanner scanner;
    Configuration config;
    auto configMap = config.getConfigMap();
    const char *dict_path = configMap["DICT_PATH"].c_str();
    const char *hmm_path = configMap["HMM_PATH"].c_str();
    const char *user_dict_path = configMap["USER_DICT_PATH"].c_str();
    SplitTool splitTool(config, dict_path, hmm_path, user_dict_path);

    DictProducer dictProducer(config, &splitTool, &scanner);

    dictProducer.buildEnDict();
    dictProducer.buildCnDict();
    return;
}

int main() { test(); }
