#ifndef _DIRSCANNER_H
#define _DIRSCANNER_H

#include <cstring>
#include <dirent.h>
#include <stdlib.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

class DirScanner {
  public:
    vector<string> &getFiles();

    void traverse(string dir);

    void clear();

  private:
    vector<string> _files;
};

inline vector<string> &DirScanner::DirScanner::getFiles() { return _files; }

inline void DirScanner::clear() { _files.clear(); }

#endif //_DIRSCANNER_H
