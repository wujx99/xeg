#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "xeg/base/nocopyable.h"
#include <string>
namespace xeg
{
    namespace FileUtil
    {

        // not thread safe
        class AppendFile : nocopyable
        {
        public:
            explicit AppendFile(const std::string &filename);
            ~AppendFile();

            void append(const char *logline, const size_t len);
            void flush();

        private:
            size_t write(const char *logline, size_t len);
            FILE *fd_;
            char buffer_[64 * 1024];
        };

    } // namespace FileUtil
} // namespace xeg

#endif