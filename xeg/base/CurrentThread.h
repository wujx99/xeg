#ifndef CURRENTTHREAD_H
#define CURRENTTHREAD_H
#include <stdint.h>
namespace xeg
{
    namespace CurrentThread
    {
        // internal
        extern __thread int t_cachedTid;
        extern __thread char t_tidString[32];
        extern __thread int t_tidStringLength;
        extern __thread const char *t_threadName;
        void cacheTid();
        inline int tid()
        {
            if (__builtin_expect(t_cachedTid == 0, 0))
            {
                cacheTid();
            }
            return t_cachedTid;
        }

        inline const char *tidString() // for logging
        {
            return t_tidString;
        }

        inline int tidStringLength() // for logging
        {
            return t_tidStringLength;
        }
        bool isMainThread();
        inline const char *name() { return t_threadName; }
    } // namespace CurrentThread

} // namespace xeg

#endif