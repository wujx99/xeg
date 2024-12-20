
#include "xeg/base/Thread.h"
#include "xeg/base/CurrentThread.h"

#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>

namespace xeg
{
    namespace internal
    {
        pid_t gettid()
        {
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }

    } // namespace internal

    void CurrentThread::cacheTid()
    {
        if (t_cachedTid == 0)
        {
            t_cachedTid = internal::gettid();
            t_tidStringLength =
                snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
        }
    }

    bool CurrentThread::isMainThread()
    {
        return tid() == getpid();
    }

    std::atomic_int Thread::numCreated_(0);

    Thread::Thread(ThreadFunc func, const std::string &name)
        : started_(false),
          joined_(false),
          thread_(),
          tid_(0),
          func_(std::move(func)),
          name_(name)
    {
        setDefaultName();
    }

    void Thread::setDefaultName()
    {
        int num = ++numCreated_;
        if (name_.empty())
        {
            char buf[32];
            snprintf(buf, sizeof(buf), "Thread%d", num);
            name_ = buf;
        }
    }

    Thread::~Thread()
    {
        if (started_ && !joined_)
            thread_->detach();
    }

    void Thread::start()
    {
        assert(!started_);
        started_ = true;

        sem_t sem;
        sem_init(&sem, 0, 0);
        thread_ = std::make_shared<std::thread>([&]()
                                                {
        tid_ = CurrentThread::tid();
        sem_post(&sem);
        CurrentThread::t_threadName = name_.empty()?"Thread" : name_.c_str();
        func_();
        CurrentThread::t_threadName = "finished"; });
        sem_wait(&sem);
        assert(tid_ > 0);
    }

    void Thread::join()
    {
        assert(started_);
        assert(!joined_);
        joined_ = true;
        return thread_->join();
    }
} // namespace xeg
