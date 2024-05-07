/**
 * Project Untitled
 */

#include "Thread.h"
/**
 * Thread implementation
 */

__thread int cache_idx = 0;
#define ERROR_CHECK(ret, retval, msg)                                          \
    {                                                                          \
        if (ret != retval) {                                                   \
            perror(msg);                                                       \
            return;                                                            \
        }                                                                      \
    }

Thread::Thread(ThreadCallBack &&cb, int cacheIdx)
    : _cb(std::move(cb)), _cacheIdx(cacheIdx) {}

Thread::~Thread() {}

void Thread::start() {
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    ERROR_CHECK(ret, 0, "pthread_create");
    _isRunning = true;
}

void Thread::stop() {
    if (_isRunning) {
        pthread_join(_thid, nullptr);
        _isRunning = false;
    }
}

/**
 * @param arg
 * @return void *
 */
void *Thread::threadFunc(void *arg) {
    Thread *pth = static_cast<Thread *>(arg);
    cache_idx = pth->_cacheIdx;
    std::cout << "cache_idx = " << cache_idx << "\n";
    if (pth) {
        pth->_cb();
    } else {
        std::cout << "thread is empty." << std::endl;
    }

    pthread_exit(nullptr);
}
