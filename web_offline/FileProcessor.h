#ifndef _FILEPROCESSOR_H
#define _FILEPROCESSOR_H

#include <string>
#include <vector>
using std::string;
using std::vector;

class FileProcessor {
  public:
    vector<string> process(const string &fileName);
};

#endif //_FILEPROCESSOR_H
