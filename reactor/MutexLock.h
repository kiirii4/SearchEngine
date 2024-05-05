/**
 * Project Untitled
 */
#ifndef _MUTEXLOCK_H
#define _MUTEXLOCK_H
#include "marco.h"
#include <pthread.h>

class MutexLock {
  public:
    MutexLock();
    ~MutexLock();

    void lock();

    void unlock();

    void trylcok();

    pthread_mutex_t *getMutexLockPtr();

  private:
    pthread_mutex_t _mutex;
};

#endif //_MUTEXLOCK_H
