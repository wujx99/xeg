#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

namespace xeg
{
    class nocopyable
    {
    protected:
        nocopyable() {};
        ~nocopyable() {};

    private:
        nocopyable(const nocopyable &nc);

        const nocopyable &operator=(const nocopyable &nc);
    };
} // namespace xeg

#endif