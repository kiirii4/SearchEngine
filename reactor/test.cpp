#include "../keyword_online/KeyRecommander.h"
#include "../reactor/EventServer.h"
#include "../shared/Configuration.h"
#include "../web_online/WebPageQuery.h"
#include <iostream>
void test() {
    KeyRecommander::dictInit();
    WebPageQuery::dictInit();
    Configuration *conf = Configuration::getInstance();
    int threadNum = std::stoi(conf->getConfigMap()["THREAD_NUM"]);
    int queSize = std::stoi(conf->getConfigMap()["QUEUE_SIZE"]);
    const string &ip = conf->getConfigMap()["SERVER_IP"];
    unsigned short port = std::stoi(conf->getConfigMap()["SERVER_PORT"]);

    int timeout = std::stoi(conf->getConfigMap()["TIMEOUT"]);

    EventServer server(threadNum, queSize, ip, port, 3000);
    server.start();

    std::cout << "server over" << "\n";
}

int main() { test(); }
