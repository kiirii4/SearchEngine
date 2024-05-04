/**
 * Project SearchEngine
 */

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
  public:
    WebPage(string &doc, Configuration &config);
    int getDocId();
    string getTitle();
    map<string, int> &getWordsMap();

  private:
    void processDoc(const string &doc);
    void calcTopK(vector<string> &worsVec, int kl, set<string> &stopWordlist);

  private:
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    vector<string> _topWords;
    map<string, int> _wordsMap;
};

#endif //_WEBPAGE_H
