#include <string>
#include <vector>

using std::string;
using std::vector;

inline size_t nBytesCode(const char ch) {
    if (ch & (1 << 7)) {
        int nBytes = 1;
        for (int idx = 0; idx != 6; ++idx) {
            if (ch & (1 << (6 - idx))) {
                ++nBytes;
            } else
                break;
        }
        return nBytes;
    }
    return 1;
}

inline std::size_t length(const std::string &str) {
    std::size_t ilen = 0;
    for (std::size_t idx = 0; idx != str.size(); ++idx) {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

class splitChar {
  public:
    static vector<string> cut(const string &str) {
        size_t nbyte = nBytesCode(str[0]);
        size_t len = length(str);
        vector<string> result;
        for (int i = 0; i < len; i += nbyte) {
            result.emplace_back(str.substr(i, nbyte));
        }
        return result;
    }
};
