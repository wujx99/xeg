#include "xeg/base/LogFile.h"

namespace xeg
{
    LogFile::LogFile(const std::string &name, int flushEveryN)
        : name_(name),
          flushEveryN_(flushEveryN),
          lock_(),
          curTimes_(0)
    {
        appendfile_.reset(new FileUtil::AppendFile(name_));
    }

    LogFile::~LogFile()
    {
    }

    void LogFile::append(const char *logline, int len)
    {
        MutexLockGuard guard(lock_);
        append_unlocked(logline, len);
    }

    void LogFile::flush()
    {
        MutexLockGuard guard(lock_);
        appendfile_->flush();
    }

    void LogFile::append_unlocked(const char *logline, int len)
    {
        appendfile_->append(logline, len);
        ++curTimes_;
        if (curTimes_ >= flushEveryN_)
        {
            curTimes_ = 0;
            appendfile_->flush();
        }
    }
} // namespace xeg
