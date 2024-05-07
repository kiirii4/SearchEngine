/**
 * Project Untitled
 */

#include "TaskQueue.h"

/**
 * TaskQueue implementation
 */

TaskQueue::TaskQueue(size_t size)
    : _size(size), _que(), _mutex(), _notFull(_mutex), _notEmpty(_mutex),
      _flag(true) {}

TaskQueue::~TaskQueue() {}

/**
 * @param value
 * @return void
 */
void TaskQueue::push(const ElemType value) {
    _mutex.lock();
    while (full()) {
        _notFull.wait();
    }

    _que.push(value);
    _notEmpty.notify();

    _mutex.unlock();
}

/**
 * @return ElemType
 */
TaskQueue::ElemType TaskQueue::pop() {
    _mutex.lock();
    while (empty() && _flag) {
        _notEmpty.wait();
    }
    if (_flag) {
        ElemType tmp = _que.front();
        _que.pop();
        _notFull.notify();
        _mutex.unlock();
        return tmp;
    } else {
        _mutex.unlock();
        return nullptr;
    }
}

/**
 * @return bool
 */
bool TaskQueue::full() { return _size == _que.size(); }

/**
 * @return bool
 */
bool TaskQueue::empty() { return 0 == _que.size(); }

void TaskQueue::wakeup() {
    _flag = false;
    _notEmpty.notifyAll();
}
