#include "xeg/base/AsyncLogging.h"
#include "xeg/base/Logging.h"
#include "xeg/base/Timestamp.h"

#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
using namespace std;
using namespace xeg;

AsyncLogging *g_asyncLog = NULL;

void asyncOutput(const char *msg, int len)
{
    g_asyncLog->append(msg, len);
}
double timeDifference(Timestamp end, Timestamp start)
{
    return static_cast<double>(end.microSecondsSinceEpoch() - start.microSecondsSinceEpoch());
}
void bench(bool longLog)
{
    Logger::setOutput(asyncOutput);

    int cnt = 0;
    const int kBatch = 1000;
    string empty = " ";
    string longStr(3000, 'X');
    longStr += " ";

    for (int t = 0; t < 30; ++t)
    {
        Timestamp start = Timestamp::now();
        for (int i = 0; i < kBatch; ++i)
        {
            LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
                     << (longLog ? longStr : empty)
                     << cnt;
            ++cnt;
        }
        Timestamp end = Timestamp::now();
        printf("%f\n", timeDifference(end, start) * 1000000 / kBatch);
        struct timespec ts = {0, 500 * 1000 * 1000};
        nanosleep(&ts, NULL);
    }
}

int main(int argc, char *argv[])
{
    {
        // set max virtual memory to 2GB.
        size_t kOneGB = 1000 * 1024 * 1024;
        rlimit rl = {2 * kOneGB, 2 * kOneGB};
        setrlimit(RLIMIT_AS, &rl);
    }

    printf("pid = %d\n", getpid());

    char name[256] = {'\0'};
    strncpy(name, argv[0], sizeof name - 5);
    strcat(name, ".txt");
    AsyncLogging log(::basename(name));
    log.start();
    g_asyncLog = &log;

    bool longLog = argc > 1;
    longLog = true;
    printf("argc:%d\n", argc);
    bench(longLog);
}