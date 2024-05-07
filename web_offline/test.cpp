#include "../../include/simhash/Simhasher.hpp"
#include "../shared/Configuration.h"
#include "../shared/SplitTool.h"
#include "../shared/tinyxml2.h"
#include "PageLib.h"
#include "PageLibPreprocessor.h"
#include <iostream>

void testPageLib() {
    Configuration *conf = Configuration::getInstance();
    PageLib pageLib;
    pageLib.create();
    pageLib.store();
}

void testPageLibPreprocessor() {
    Configuration *config = Configuration::getInstance();
    auto configMap = config->getConfigMap();
    const char *dict_path = configMap["DICT_PATH"].c_str();
    const char *hmm_path = configMap["HMM_PATH"].c_str();
    const char *user_dict_path = configMap["USER_DICT_PATH"].c_str();
    SplitTool splitTool;
    PageLibPreprocessor pageLibPreprocessor(&splitTool);
    pageLibPreprocessor.cutRedundantPage();
    pageLibPreprocessor.bulidInvertIndexMap();
    pageLibPreprocessor.storeOnDisk();
    std::cout << "ok\n";
}

void testSimhasher() {
    Configuration *config = Configuration::getInstance();
    auto configMap = config->getConfigMap();
    const char *dict_path = configMap["DICT_PATH"].c_str();
    const char *hmm_path = configMap["HMM_PATH"].c_str();
    const char *idf_path = configMap["IDF_PATH"].c_str();
    const char *user_dict_path = configMap["USER_DICT_PATH"].c_str();
    const char *stop_words_path = configMap["JIEBA_STOP_WORD_PATH"].c_str();
    std::cout << "ok\n";
    string str = "something happend 我发现了something";
    simhash::Simhasher simhasher(dict_path, hmm_path, idf_path,
                                 stop_words_path);
}

void testXml2() {
    Configuration *config = Configuration::getInstance();
    auto configMap = config->getConfigMap();
    const char *web_lib_raw = configMap["WEB_LIB_RAW"].c_str();
    std::cout << web_lib_raw << "\n";
    tinyxml2::XMLDocument doc;
    doc.LoadFile(web_lib_raw);
    if (doc.ErrorID()) {
        std::cout << "doc error id" << doc.ErrorID() << "\n";
        return;
    }
    std::cout << "ok\n";
}

int main() {
    // testPageLib();

    testPageLibPreprocessor();
}
