#include "FileProcessor.h"
#include "mylog.h"
#include "tinyxml2.h"
#include <regex>
#include <sstream>
using namespace tinyxml2;
using std::ostringstream;

struct WebItem {
    WebItem(const string &title, const string &link, const string &desc)
        : _title(title), _link(link), _desc(desc) {}
    string _title;
    string _link;
    string _desc;
};

vector<string> FileProcessor::process(const string &fileName) {
    vector<WebItem> items;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName.c_str());
    if (doc.Error()) {
        Logerror("load file error");
        return vector<string>();
    }
    XMLElement *root = doc.FirstChildElement("rss")
                           ->FirstChildElement("channel")
                           ->FirstChildElement("item");
    while (root) {
        string title, link, desc;
        XMLElement *ptemp = root->FirstChildElement("description");

        if (ptemp == nullptr) {
            continue;
        }
        desc = ptemp->GetText();
        ptemp = root->FirstChildElement("title");
        if (ptemp != nullptr) {
            title = ptemp->GetText();
        }
        ptemp = root->FirstChildElement("link");
        if (ptemp != nullptr) {
            link = ptemp->GetText();
        }
        std::regex reg("<[^>]*>");
        desc = std::regex_replace(desc, reg, "");
        items.emplace_back(title, link, desc);
        root = root->NextSiblingElement("item");
    }

    vector<string> result;
    result.reserve(items.size());
    ostringstream oss;
    for (auto &item : items) {
        oss << "<doc>\n<title>" << item._title << "</title>\n<link>"
            << item._link << "</link>\n<desc>" << item._desc
            << "</desc>\n</doc>\n";
        result.emplace_back(oss.str());
    }
    return result;
}
