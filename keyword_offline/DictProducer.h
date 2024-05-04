/**
 * Project SearchEngine
 */


#ifndef _DICTPRODUCER_H
#define _DICTPRODUCER_H

class DictProducer {
public: 
    
/**
 * @param string
 * @param tool
 */
void DictProducer(void string, void tool);
    
void buildEnDict();
    
void buildCnDict();
    
void createIndex();
    
void store();
private: 
    vector<string> _files;
    vector<pair<string,int>> _dict;
    map<string,set<int>> _index;
    SplitTool* _cuttor;
};

#endif //_DICTPRODUCER_H