#ifndef ASYNCLOGGING_H
#define ASYNCLOGGING_H

#include "xeg/base/nocopyable.h"
#include "xeg/base/Thread.h"
#include "xeg/base/MutexLock.h"
#include "xeg/base/Cond.h"
#include "xeg/base/LogStream.h"
#include "xeg/base/LogFile.h"
#include "xeg/base/Timestamp.h"

#include <vector>
#include <semaphore.h>
#include <memory>

namespace xeg
{
    // 开启一个logging线程

    class AsyncLogging : nocopyable
    {

    public:
        AsyncLogging(const std::string &basename, int flushInerval = 3);
        ~AsyncLogging();

    public:
        void start()
        {
            running_ = true;
            sem_init(&sem_, 0, 0);
            thread_.start();
            sem_wait(&sem_);
        }
        void stop()
        {
            running_ = false;
            cond_.notify();
            thread_.join();
        }

        void append(const char *logline, int len); // 前端

    private:
        void threadFunc(); // 后端

    private:
        using Buffer = detail::FixedBuffer<detail::KLargeBufferSize>;
        using BufferVector = std::vector<std::unique_ptr<Buffer>>; // 使用uinque_ptr
        using BufferPtr = BufferVector::value_type;

        const int flushInterval_;
        bool running_;
        std::string basename_;
        Thread thread_;
        MutexLock lock_;
        Cond cond_;
        BufferPtr curBuffer_;
        BufferPtr nextBuffer_;
        BufferVector bufferVec_;
        sem_t sem_;
    };

} // namespace xeg

#endif