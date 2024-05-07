#include "../keyword_online/KeyRecommander.h"
#include "../reactor/EventServer.h"
#include "../shared/Configuration.h"
#include "../web_online/WebPageQuery.h"
#include <iostream>
void test() {
    Configuration config;
    KeyRecommander::dictInit(&config);
    WebPageQuery::dictInit(&config);
    EventServer server(4, 10, "127.0.0.1", 8888, 3000);
    server.start();

    std::cout << "server over" << "\n";
}

int main() { test(); }
