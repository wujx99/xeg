#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include "xeg/base/nocopyable.h"

#include <string>
#include <assert.h>
#include <cstring>

namespace xeg
{
    namespace detail
    {
        const int KSmallBufferSize = 4000;
        const int KLargeBufferSize = 4000 * 1000;

        template <int SIZE>
        class FixedBuffer : nocopyable
        {
        public:
            FixedBuffer() : cur_(data_) {}
            ~FixedBuffer() {}

            void append(const char *buf, size_t len)
            {
                if (avail() > static_cast<int>(len))
                {
                    memcpy(cur_, buf, len);
                    cur_ += len;
                }
            }

            const char *data() const { return data_; }
            int length() const { return static_cast<int>(cur_ - data_); }

            char *current() { return cur_; }
            int avail() { return static_cast<int>(end() - cur_); }
            void add(size_t len) { cur_ += len; }

            void reset() { cur_ = data_; }
            void bzero() { memset(data_, 0, sizeof(data_)); }

        private:
            const char *end() const { return data_ + sizeof(data_); }

        private:
            char data_[SIZE];
            char *cur_;
        };

    } // namespace detail

    class LogStream : nocopyable
    {
    public:
        using Buffer = detail::FixedBuffer<detail::KSmallBufferSize>;

        LogStream &operator<<(bool v)
        {
            buffer_.append(v ? "1" : "0", 1);
            return *this;
        }

        LogStream &operator<<(short);
        LogStream &operator<<(unsigned short);
        LogStream &operator<<(int);
        LogStream &operator<<(unsigned int);
        LogStream &operator<<(long);
        LogStream &operator<<(unsigned long);
        LogStream &operator<<(long long);
        LogStream &operator<<(unsigned long long);

        LogStream &operator<<(const void *);

        LogStream &operator<<(float v)
        {
            *this << static_cast<double>(v);
            return *this;
        }

        LogStream &operator<<(double);

        LogStream &operator<<(char v)
        {
            buffer_.append(&v, 1);
            return *this;
        }
        LogStream &operator<<(const char *str)
        {
            if (str)
                buffer_.append(str, strlen(str));
            else
                buffer_.append("(null)", 6);
            return *this;
        }

        LogStream &operator<<(const unsigned char *str)
        {
            return operator<<(reinterpret_cast<const char *>(str));
        }

        LogStream &operator<<(const std::string &str)
        {
            buffer_.append(str.c_str(), str.size());
            return *this;
        }

        void append(const char *data, int len) { buffer_.append(data, len); }
        const Buffer &buffer() const { return buffer_; }
        void resetBuffer() { buffer_.reset(); }

    private:
        void staticCheck();

        template <typename T>
        void formatInteger(T);

    private:
        Buffer buffer_;
        static const int kMaxNumericSize = 48;
    };

    class Fmt
    {
    public:
        template <typename T>
        Fmt(const char *fmt, T val);

        const char *data() const { return buf_; }
        int length() const { return len_; }

    private:
        char buf_[32];
        int len_;
    };

    inline LogStream &operator<<(LogStream &s, const Fmt &fmt)
    {
        s.append(fmt.data(), fmt.length());
        return s;
    }

    // Format quantity n in SI units (k, M, G, T, P, E).
    // The returned string is atmost 5 characters long.
    // Requires n >= 0
    std::string formatSI(int64_t n);

    // Format quantity n in IEC (binary) units (Ki, Mi, Gi, Ti, Pi, Ei).
    // The returned string is atmost 6 characters long.
    // Requires n >= 0
    std::string formatIEC(int64_t n);

} // namespace xeg

#endif