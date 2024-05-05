#ifndef _WEBPAGE_H
#define _WEBPAGE_H
#include "Configuration.h"
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class WebPage {
    friend class PageLibPreprocessor;

  public:
    WebPage(const string &docTitle, const string &docUrl,
            const string &docContent, uint64_t simahsh)
        : _docTitle(docTitle), _docUrl(docUrl), _docContent(docContent),
          _simahsh(simahsh) {}
    string &getDoc();

  private:
    string _docTitle;
    string _docUrl;
    string _docContent;
    uint64_t _simahsh;
};

inline string &WebPage::getDoc() { return _docContent; }

#endif //_WEBPAGE_H
