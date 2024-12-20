#include "xeg/base/AsyncLogging.h"

namespace xeg
{
    AsyncLogging::AsyncLogging(const std::string &basename, int flushInerval)
        : flushInterval_(flushInerval),
          running_(false),
          basename_(basename),
          thread_(std::bind(&AsyncLogging::threadFunc, this), "logging"),
          lock_(),
          cond_(lock_),
          curBuffer_(new Buffer),
          nextBuffer_(new Buffer),
          bufferVec_(),
          sem_()
    {
        assert(basename_.size() > 1);
        curBuffer_->bzero();
        nextBuffer_->bzero();
        bufferVec_.reserve(16);
    }

    AsyncLogging::~AsyncLogging()
    {
    }
    void AsyncLogging::append(const char *logline, int len)
    {
        MutexLockGuard guard(lock_);
        if (curBuffer_->avail() > len)
        {
            curBuffer_->append(logline, len);
        }
        else
        {
            bufferVec_.push_back(std::move(curBuffer_));
            if (nextBuffer_)
            {
                curBuffer_ = std::move(nextBuffer_);
            }
            else
            {
                curBuffer_.reset(new Buffer());
            }
            curBuffer_->append(logline, len);
            cond_.notify();
        }
    }

    void AsyncLogging::threadFunc() // 后端
    {
        assert(running_ == true);
        sem_post(&sem_); // 通知线程已经启动
        LogFile output(basename_);

        BufferPtr newPtr1(new Buffer());
        BufferPtr newPtr2(new Buffer());
        newPtr1->bzero();
        newPtr2->bzero();
        BufferVector tobeWrite;
        tobeWrite.reserve(16);

        while (running_)
        {
            assert(newPtr1 && newPtr2->length() == 0);
            assert(newPtr2 && newPtr2->length() == 0);
            assert(tobeWrite.empty());

            // 临界区，swap
            {
                MutexLockGuard guard(lock_);

                if (bufferVec_.empty())
                {
                    cond_.waitForSeconds(flushInterval_);
                }
                bufferVec_.push_back(std::move(curBuffer_)); // 要全部清理
                curBuffer_ = std::move(newPtr1);
                tobeWrite.swap(bufferVec_);
                if (!nextBuffer_)
                {
                    nextBuffer_ = std::move(newPtr2);
                }
            }

            assert(!tobeWrite.empty());

            // 防止过多的
            if (tobeWrite.size() > 25)
            {
                char buf[256];
                snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
                         Timestamp::now().toString().c_str(),
                         tobeWrite.size() - 2);
                fputs(buf, stderr);
                output.append(buf, static_cast<int>(strlen(buf)));
                tobeWrite.erase(tobeWrite.begin() + 2, tobeWrite.end());
            }

            for (const auto &buffer : tobeWrite)
            {
                // FIXME: use unbuffered stdio FILE ? or use ::writev ?
                output.append(buffer->data(), buffer->length());
            }

            if (tobeWrite.size() > 2)
            {
                // drop non-bzero-ed buffers, avoid trashing
                tobeWrite.resize(2);
            }

            if (!newPtr1)
            {
                assert(!tobeWrite.empty());
                newPtr1 = std::move(tobeWrite.back());
                tobeWrite.pop_back();
                newPtr1->reset();
            }

            if (!newPtr2)
            {
                assert(!tobeWrite.empty());
                newPtr2 = std::move(tobeWrite.back());
                tobeWrite.pop_back();
                newPtr2->reset();
            }

            tobeWrite.clear();
            output.flush();
        }

        output.flush();
    }
} // namespace xeg
