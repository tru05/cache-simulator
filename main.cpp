#include <iostream>
#include <vector>
#include "Cache.h"

using namespace std;

int main()
{
    cout << "Select Replacement Policy:\n";
    cout << "1. LRU\n";
    cout << "2. FIFO\n";
    cout << "3. RANDOM\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    ReplacementPolicy selectedPolicy;

    if (choice == 1)
        selectedPolicy = LRU;
    else if (choice == 2)
        selectedPolicy = FIFO;
    else if (choice == 3)
        selectedPolicy = RANDOM;
    else {
        cout << "Invalid choice. Defaulting to LRU.\n";
        selectedPolicy = LRU;
    }

    // Cache parameters
    int cache_size = 64;
    int block_size = 8;
    int associativity = 2;

    Cache myCache(cache_size, block_size, associativity,
                  1.0, 50.0, selectedPolicy);

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
