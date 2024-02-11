#include "Utils.h"
#include <time.h>

template<typename T>
T random(T min, T max)
{
    return min + rand() % (max - min + 1);
}

template unsigned random<unsigned>(unsigned, unsigned);
template int random<int>(int, int);