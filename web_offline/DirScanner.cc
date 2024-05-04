#include "DirScanner.h"
#include "mylog.h"

/**
 * @param dir
 * @return void
 */
void DirScanner::traverse(string dir) {
    DIR *pDir;
    struct dirent *pEnt;
    if (!(pDir = ::opendir(dir.c_str()))) {
        Logerror("opendir error");
        return;
    }
    while ((pEnt = ::readdir(pDir)) != 0) {
        if (::strcmp(pEnt->d_name, ".") == 0 ||
            ::strcmp(pEnt->d_name, "..") == 0) {
            continue;
        }
        if (pEnt->d_type == DT_DIR) {
            traverse(dir + "/" + pEnt->d_name);
        } else if (pEnt->d_type == DT_REG) {
            _files.push_back(dir + "/" + pEnt->d_name);
        }
    }
    ::closedir(pDir);
}
