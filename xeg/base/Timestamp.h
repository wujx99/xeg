
#ifndef Timestamp_H
#define Timestamp_H

#include <string>

namespace xeg
{
    class Timestamp
    {
    public:
        Timestamp();
        explicit Timestamp(int64_t microSecondsSinceEpoch);
        static Timestamp now();
        std::string toString() const;

        int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }

        static const int kMicroSecondsPerSecond = 1000 * 1000;

    private:
        int64_t microSecondsSinceEpoch_;
    };
} // namespace xeg

#endif