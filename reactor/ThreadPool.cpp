#include "ThreadPool.h"
#include "marco.h"

ThreadPool::ThreadPool(size_t thread_size, size_t que_size)
    : _thread_size(thread_size), _que_size(que_size), _isExit(false),
      _taskque(_que_size) {
    _threads.reserve(_thread_size);
}

ThreadPool::~ThreadPool() {}

void ThreadPool::start() {
    for (size_t idx = 0; idx != _thread_size; ++idx) {
        _threads.push_back(unique_ptr<Thread>(
            new Thread(std::bind(&ThreadPool::doTask, this), idx + 1)));
    }

    for (auto &th : _threads) {
        th->start();
    }
}

void ThreadPool::stop() {
    while (!_taskque.empty()) {
        sleep(1);
    }
    _isExit = true;
    _taskque.wakeup();
    for (auto &th : _threads) {
        th->stop();
    }
}

void ThreadPool::addTask(Task &&ptask) {
    if (ptask) {
        _taskque.push(std::move(ptask));
    }
}

ThreadPool::Task ThreadPool::getTask() { return _taskque.pop(); }

void ThreadPool::doTask() {
    while (!_isExit) {
        auto task = getTask();
        if (task) {
            task();
        } else {
            cout << "nullptr == task" << endl;
        }
    }
}
