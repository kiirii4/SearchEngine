/**
 * Project Untitled
 */
#include "TaskQueue.h"
#include <functional>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#ifndef _THREAD_H
#define _THREAD_H

extern __thread int cache_idx;
class Thread {
  public:
    using ThreadCallBack = std::function<void()>;
    Thread() {}
    Thread(ThreadCallBack &&, int cacheIdx);

    ~Thread();

    void start();

    void stop();

  private:
    pthread_t _thid{0};
    bool _isRunning{false};
    ThreadCallBack _cb;
    /**
     * @param arg
     */
    static void *threadFunc(void *arg);
};

#endif //_THREAD_H
