#ifndef __THREAD_POOL__
#define __THREAD_POOL__
#include "../cache/CacheManager.h"
#include "TaskQueue.h"
#include "Thread.h"
#include <functional>
#include <memory>

extern __thread int cache_idx;
class ThreadPool {
  public:
    using Task = function<void()>;
    ThreadPool(size_t, size_t);
    ~ThreadPool();
    void start();
    void addTask(Task &&);
    void stop();
    Task getTask();
    void doTask();

  private:
    size_t _thread_size;
    size_t _que_size;
    vector<unique_ptr<Thread>> _threads;
    CacheManager _cacheManger;
    TaskQueue _taskque;
    bool _isExit;
};
#endif // !__THREAD_POOL__
