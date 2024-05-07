#include <string>

using std::string;
struct CandidateResult {
    CandidateResult(const string &word, const int &freq, const int &dist)
        : _word(word), _freq(freq), _dist(dist) {}

  public:
    string _word;
    int _freq;
    int _dist;
};

// 构建小顶堆，距离越小优先级越高
inline bool operator<(const CandidateResult &lhs, const CandidateResult &rhs) {
    return (lhs._dist == rhs._dist)
               ? ((lhs._freq == rhs._freq) ? (lhs._word > rhs._word)
                                           : (lhs._freq > rhs._freq))
               : (lhs._dist > rhs._dist);
}
