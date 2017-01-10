#pragma once

#include <cctype>

namespace sc
{

    template<typename T>
    T extract(const char*& input);

    template<>
    char extract(const char*& input)
    {
        int result;
        char* pos;

        result = strtol(input, &pos, 0);
        input = pos;

        return result;
    }

    template<>
    int extract(const char*& input)
    {
        int result;
        char* pos;

        result = strtol(input, &pos, 0);
        input = pos;

        return result;
    }

    template<>
    float extract(const char*& input)
    {
        float result;
        char* pos;

        result = (float)strtod(input, &pos);
        input = pos;

        return result;
    }

    template<>
    double extract(const char*& input)
    {
        double result;
        char* pos;

        result = strtod(input, &pos);
        input = pos;

        return result;
    }

    template<>
    const char* extract(const char*& input)
    {
        const char* result = input + std::strspn(input, " \t");
        input = result + std::strcspn(result, " \t\r\n");

        return result;
    }

    char to_bin(char ascii)
    {
        char bin = 0;
        if (ascii >= '0' && ascii <= '9')
        {
            bin = ascii - '0' + 0;
        }
        else if (ascii >= 'a' && ascii <= 'f')
        {
            bin = ascii - 'a' + 0x0A;
        }
        else if (ascii >= 'A' && ascii <= 'F')
        {
            bin = ascii - 'A' + 0x0A;
        }
        return bin;
    }

    char* extract(const char*& input, int& count, char* buffer)
    {
        int used = 0;
        const char* pos = input + std::strspn(input, " \t");
        for ( ;
            used < count
                && !isspace(pos[2*used]) && !isspace(pos[2*used+1])
                && pos[2*used] != 0 && pos[2*used+1] != 0;
            ++used)
        {
            buffer[used] = (to_bin(pos[2*used]) << 4)
                | to_bin(pos[2*used+1]);
        }
        count = used;

        return buffer;
    }
}

