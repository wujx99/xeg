#ifndef THREAD_H
#define THREAD_H

#include "xeg/base/nocopyable.h"

#include <string.h>
#include <pthread.h>
#include <functional>
#include <atomic>
#include <thread>
namespace xeg
{
    class Thread : nocopyable
    {

    public:
        using ThreadFunc = std::function<void()>;

        explicit Thread(ThreadFunc, const std::string &name = std::string());

        ~Thread();

        void start();
        void join();

        bool started() const { return started_; }
        pid_t tid() const { return tid_; }
        const std::string &name() const { return name_; }

        static int numCreated() { return numCreated_; }

    private:
        void setDefaultName();

    private:
        bool started_;
        bool joined_;
        std::shared_ptr<std::thread> thread_;
        pid_t tid_; // 全局唯一
        ThreadFunc func_;
        std::string name_;

        static std::atomic_int numCreated_;
    };
} // namespace xeg


#endif