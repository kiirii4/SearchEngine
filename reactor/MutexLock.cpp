/**
 * Project Untitled
 */

#include "MutexLock.h"

/**
 * MutexLock implementation
 */

MutexLock::MutexLock() {
    int ret = pthread_mutex_init(&_mutex, nullptr);
    ERROR_CHECK(ret, 0, "pthread_mutex_init");
}

void MutexLock::lock() {
    int ret = pthread_mutex_lock(&_mutex);
    ERROR_CHECK(ret, 0, "pthread_mutex_lock");
}

void MutexLock::unlock() {
    int ret = pthread_mutex_unlock(&_mutex);
    ERROR_CHECK(ret, 0, "pthread_mutex_unlock");
}

void MutexLock::trylcok() {
    int ret = pthread_mutex_trylock(&_mutex);
    ERROR_CHECK(ret, 0, "pthread_mutex_trylock");
}

pthread_mutex_t *MutexLock::getMutexLockPtr() { return &_mutex; }

MutexLock::~MutexLock() {
    int ret = pthread_mutex_destroy(&_mutex);
    ERROR_CHECK(ret, 0, "pthread_mutex_destroy");
}
