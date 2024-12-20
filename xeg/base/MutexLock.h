#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include <pthread.h>
#include "xeg/base/nocopyable.h"

namespace xeg
{
    class MutexLock : nocopyable
    {

    public:
        MutexLock()
        {
            pthread_mutex_init(&mutex_, NULL);
        }
        ~MutexLock()
        {
            pthread_mutex_destroy(&mutex_);
        }
        pthread_mutex_t *getLock() { return &mutex_; }
        int lock() { return pthread_mutex_lock(&mutex_); }
        int unlock() { return pthread_mutex_unlock(&mutex_); }

    private:
        pthread_mutex_t mutex_;

    private:
        friend class Cond;
    };

    class MutexLockGuard : nocopyable
    {
    public:
        MutexLockGuard(MutexLock &lock) : lock_(lock)
        {
            lock_.lock();
        }
        ~MutexLockGuard()
        {
            lock_.unlock();
        }

    private:
        MutexLock &lock_;
    };
} // namespace xeg

#endif