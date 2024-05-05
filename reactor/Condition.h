/**
 * Project Untitled
 */
#include "MutexLock.h"
#include "marco.h"
#include <pthread.h>
#ifndef _CONDITION_H
#define _CONDITION_H

class Condition {
  public:
    Condition(MutexLock &);

    ~Condition();

    void wait();

    void notify();

    void notifyAll();

  private:
    pthread_cond_t _cond;
    MutexLock &_mutex;
};

#endif //_CONDITION_H
