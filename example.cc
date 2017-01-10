#include "sc.h"

#include <array>
#include <cstdio>

const char* compiletime()
{
    return __DATE__ " " __TIME__;
}

float add(float a, float b)
{
    return a + b;
}

float subtract(float a, float b)
{
    return a - b;
}

double multiply(double a, double b)
{
    return a * b;
}

double divide(double a, int b)
{
    return a / b;
}

class ClassMath
{
    public:
    int factorial(int n)
    {
        if (n == 1)
        {
            return 1;
        }
        else
        {
            return n*factorial(n-1);
        }
    }
};

static std::array<char, 16> buffer_data;

bool buffer_write(int offset, int size, char* data)
{
    for (int i = 0; i < size; ++i)
    {
        buffer_data.at(offset+i) = data[i];
    }

    return true;
}

bool buffer_read(int offset, int size, char* data)
{
    for (int i = 0; i < size; ++i)
    {
        data[i] = buffer_data.at(offset+i);
    }

    return true;
}

int main(int, char*[])
{
    ClassMath obj;

    auto c = sc::collect(
        sc::name("ctime",   sc::wrap<const char*()>(compiletime)),
        sc::name("add",     sc::wrap<float(float, float)>(add)),
        sc::name("sub",     sc::wrap<float(float, float)>(subtract)),
        sc::name("mul",     sc::wrap<double(double, double)>(multiply)),
        sc::name("div",     sc::wrap<double(double, int)>(divide)),
        sc::name("fact",    sc::wrap<int(int)>(
            [&obj](int n) -> int
            {
                return obj.factorial(n);
            })),
        sc::name("bufw",    sc::wrap<bool(int, const char*)>(
            [](int offset, const char* data)
            {
                char buffer[8];
                int size = sizeof(buffer);
                sc::extract(data, size, buffer);
                return buffer_write(offset, size, buffer);
            })),
        sc::name("bufr",    sc::wrap<bool(int, int)>(
            [](int offset, int size)
            {
                char buffer[8];
                size = std::min(size, (int)sizeof(buffer));
                auto result = buffer_read(offset, size, buffer);
                for (int i = 0; i < size; ++i)
                {
                    printf("buffer[%2d + %d] = 0x%02X\n",
                        offset, i, (unsigned char)buffer[i]);
                }
                return result;
            }))
        );

    for (;;)
    {
        char in[64];
        char out[64];

        std::fgets(in, sizeof(in), stdin);
        in[sizeof(in) - 1] = 0;
        if (!c.execute(in, sizeof(out), out))
        {
            break;
        }
        std::printf("-> %s\n", out);
    }

    return 0;
}
