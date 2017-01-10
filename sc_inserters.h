#pragma once

#include <cstring>
#include <cstdio>

namespace sc
{

    template<typename T>
    void insert(T value, int max_out, char* out);

    template<>
    void insert(bool value, int max_out, char* out)
    {
        if (max_out > 0 && out != nullptr)
        {
            if (value)
            {
                std::strncpy(out, "true", max_out);
            }
            else
            {
                std::strncpy(out, "false", max_out);
            }
            out[max_out-1] = 0;
        }
    }

    template<>
    void insert(int value, int max_out, char* out)
    {
        std::snprintf(out, max_out, "%d", value);
    }

    template<>
    void insert(float value, int max_out, char* out)
    {
        std::snprintf(out, max_out, "%f", value);
    }

    template<>
    void insert(double value, int max_out, char* out)
    {
        std::snprintf(out, max_out, "%f", value);
    }

    template<>
    void insert(const char* value, int max_out, char* out)
    {
        if (max_out > 0 && out != nullptr)
        {
            std::strncpy(out, value, max_out);
            out[max_out-1] = 0;
        }
    }

}
