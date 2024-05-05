/**
 * Project Untitled
 */
#include "Condition.h"
#include "MutexLock.h"
#include <functional>
#include <pthread.h>
#include <queue>
#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

using namespace std;
class TaskQueue {
  public:
    using ElemType = function<void()>;
    explicit TaskQueue(size_t);

    ~TaskQueue();

    /**
     * @param value
     */
    void push(const ElemType value);

    ElemType pop();

    bool full();

    bool empty();
    void wakeup();

  private:
    MutexLock _mutex;
    size_t _size;
    queue<ElemType> _que;
    Condition _notFull;
    Condition _notEmpty;
    bool _flag;
};

#endif //_TASKQUEUE_H
