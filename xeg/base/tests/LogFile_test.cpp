#include "xeg/base/LogFile.h"
#include "xeg/base/Logging.h"

#include <unistd.h>
#include <string>

using namespace xeg;
std::unique_ptr<LogFile> g_logFile;

void outputFunc(const char *msg, int len)
{
    g_logFile->append(msg, len);
}

void flushFunc()
{
    g_logFile->flush();
}

int main(int argc, char *argv[])
{
    char name[256] = {'\0'};
    strncpy(name, argv[0], sizeof name - 5);
    strcat(name, ".txt");
    g_logFile.reset(new LogFile(::basename(name), 200 * 1000));
    Logger::setOutput(outputFunc);
    Logger::setFlush(flushFunc);
    std::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    for (size_t i = 0; i < 1000; i++)
    {
        LOG_INFO << line << i;
        usleep(1000);
    }
}