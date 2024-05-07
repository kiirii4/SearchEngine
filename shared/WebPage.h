#ifndef _WEBPAGE_H
#define _WEBPAGE_H
#include "Configuration.h"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

struct WebPage {

  public:
    WebPage(const string &docTitle, const string &docUrl,
            const string &docContent)
        : _docTitle(docTitle), _docUrl(docUrl), _docContent(docContent) {}

    string _docTitle;
    string _docUrl;
    string _docContent;
};

#endif //_WEBPAGE_H
