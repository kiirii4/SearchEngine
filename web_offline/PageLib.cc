#include "PageLib.h"
#include <fstream>

using std::ofstream;

void PageLib::create() {
    auto configMap = _conf.getConfigMap();

    long total_size = 0;
    int docId = 0;
    _dirScanner.traverse(configMap["XML_PATH"]);
    auto xmlFiles = _dirScanner.getFiles();
    for (auto const &xml : xmlFiles) {
        vector<string> web = _fileProcessor.process(xml);
        for (auto const &webPage : web) {
            _pages.emplace_back(webPage);
            size_t pageSize = webPage.size();
            _offsetLib[docId] = pair<long, long>(total_size, pageSize);

            total_size += pageSize;
            docId++;
        }
    }
}

void PageLib::store() {
    const string webLibPath = _conf.getConfigMap()["WEB_LIB_RAW"];
    const string offsetLibPath = _conf.getConfigMap()["OFFSET_LIB_RAW"];

    ofstream ofs(webLibPath);

    if (!ofs) {
        Logerror("can't open webLib file!");
        return;
    }

    for (auto const &page : _pages) {
        ofs << page;
    }

    ofs.close();

    ofs.open(offsetLibPath);
    if (!ofs) {
        Logerror("can't open offsetLib file!");
        return;
    }
    for (auto const &offset : _offsetLib) {
        ofs << offset.first << " " << offset.second.first << " "
            << offset.second.second << "\n";
    }
    ofs.close();
}
