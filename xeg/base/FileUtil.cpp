#include "xeg/base/FileUtil.h"
#include <assert.h>
#include <stdio.h>

namespace xeg
{
    namespace FileUtil
    {
        AppendFile::AppendFile(const std::string &filename)
            : fd_(::fopen(filename.c_str(), "ae"))
        {
            assert(fd_);
            setbuffer(fd_, buffer_, sizeof(buffer_));
        }
        AppendFile::~AppendFile()
        {
            fclose(fd_);
        }
        void AppendFile::append(const char *logline, const size_t len)
        {
            size_t n = write(logline, len);
            size_t remain = len - n;
            while (remain > 0)
            {
                size_t x = write(logline + n, remain);
                if (x == 0)
                {
                    int err = ferror(fd_);
                    if (err)
                        fprintf(stderr, "AppendFile::append() failed!\n");
                    break;
                }
                n += x;
                remain = len - n;
            }
        }
        void AppendFile::flush() { fflush(fd_); }
        size_t AppendFile::write(const char *logline, size_t len)
        {
            return fwrite_unlocked(logline, 1, len, fd_); // 只有一个线程写
        }

    } // namespace FileUtil
} // namespace xeg
