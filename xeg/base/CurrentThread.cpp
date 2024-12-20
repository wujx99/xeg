#include "xeg/base/CurrentThread.h"
#include <type_traits>
#include <assert.h>
#include <sys/types.h>

namespace xeg
{
    namespace CurrentThread
    {
        __thread int t_cachedTid = 0;
        __thread char t_tidString[32];
        __thread int t_tidStringLength = 6;
        __thread const char *t_threadName = "default";
        static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

    } // namespace CurrentThread

} // namespace xeg
