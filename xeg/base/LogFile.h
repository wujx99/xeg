#ifndef LOGFILE_H
#define LOGFILE_H

#include <string>
#include <memory>
#include "xeg/base/nocopyable.h"
#include "xeg/base/FileUtil.h"
#include "xeg/base/MutexLock.h"

namespace xeg
{
    class LogFile : nocopyable
    {
    public:
        LogFile(const std::string &name, int flushEveryN = 1024);
        ~LogFile();

        void append(const char *logline, int len);
        void flush();

    private:
        void append_unlocked(const char *logline, int len);

    private:
        std::string name_;
        int flushEveryN_;

        std::unique_ptr<FileUtil::AppendFile> appendfile_;

        MutexLock lock_;
        int curTimes_;
    };

} // namespace xeg

#endif