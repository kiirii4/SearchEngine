/**
 * Project Untitled
 */

#include "Condition.h"

/**
 * Condition implementation
 */

Condition::Condition(MutexLock &mutex) : _mutex(mutex) {
    int ret = pthread_cond_init(&_cond, nullptr);
    ERROR_CHECK(ret, 0, "pthread_cond_init");
}

Condition::~Condition() {
    int ret = pthread_cond_destroy(&_cond);
    ERROR_CHECK(ret, 0, "pthread_cond_destroy");
}

/**
 * @return void
 */
void Condition::wait() {
    int ret = pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
    ERROR_CHECK(ret, 0, "pthread_cond_wait");
}

/**
 * @return void
 */
void Condition::notify() {
    int ret = pthread_cond_signal(&_cond);
    ERROR_CHECK(ret, 0, "pthread_cond_signal");
}

/**
 * @return void
 */
void Condition::notifyAll() {
    int ret = pthread_cond_broadcast(&_cond);
    ERROR_CHECK(ret, 0, "pthread_cond_broadcast");
}
