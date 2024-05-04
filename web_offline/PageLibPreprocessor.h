#ifndef _PAGELIBPREPROCESSOR_H
#define _PAGELIBPREPROCESSOR_H

class PageLibPreprocessor {
  public:
    void cutRedundantPage();

    void bulidInvertIndexMap();

    void storeOnDisk();

  private:
    vector<WebPage> _pageList;
    unordered_map _offsetLib;
    unordered_map _invertIndexLib;
    SplitTool *_wordCutter;
};

#endif //_PAGELIBPREPROCESSOR_H
