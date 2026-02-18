#include <iostream>
#include <vector>
#include "Cache.h"

using namespace std;

int main()
{
    Cache myCache(64, 8, 2);

    vector<pair<unsigned int, bool>> memory_trace = {
        {0, false}, {8, true}, {16, false},
        {24, true}, {0, false}, {32, true},
        {40, false}, {0, true}
    };

    for (auto &access : memory_trace) {
        myCache.access(access.first, access.second);
    }

    myCache.printCacheState();
    myCache.printStats();

    return 0;
}
