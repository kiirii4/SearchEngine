#include "../Configuration.h"
#include <iostream>

int main() {
    Configuration config("../../conf/dict.config");
    auto stop_words = config.getStopWordSet();
    auto config_map = config.getConfigMap();

    for (auto &it : (config_map)) {
        std::cout << it.first << " " << it.second << std::endl;
    }
    std::cout << "stop words" << "\n";
    for (auto &it : (stop_words)) {
        std::cout << it << std::endl;
    }
}
