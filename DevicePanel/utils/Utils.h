#pragma once
#include <string>

inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(
        s.begin(), s.end(), [](unsigned char ch)
        {
            return !std::isspace(ch);
        }
    ));
}

template<typename T>
T random(T min, T max);
