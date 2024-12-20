#ifndef COND_H
#define COND_H

#include "xeg/base/MutexLock.h"
#include "xeg/base/nocopyable.h"
#include <pthread.h>
#include <time.h>
#include <errno.h>

namespace xeg
{
    class Cond : nocopyable
    {
    public:
        Cond(MutexLock &lock) : lock_(lock)
        {
            pthread_cond_init(&cond_var_, nullptr);
        }
        ~Cond()
        {
            pthread_cond_destroy(&cond_var_);
        }
        void wait() { pthread_cond_wait(&cond_var_, lock_.getLock()); }
        void notify() { pthread_cond_signal(&cond_var_); }
        void notifyAll() { pthread_cond_broadcast(&cond_var_); }
        bool waitForSeconds(int seconds)
        {
            struct timespec abstime;
            clock_gettime(CLOCK_REALTIME, &abstime);
            abstime.tv_sec += static_cast<time_t>(seconds);
            return ETIMEDOUT == pthread_cond_timedwait(&cond_var_, lock_.getLock(), &abstime);
        }

    private:
        MutexLock &lock_;
        pthread_cond_t cond_var_;
    };

} // namespace xeg

#endif