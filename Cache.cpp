#include "Cache.h"
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

CacheLine::CacheLine() {
    tag = 0;
    valid = false;
    dirty = false;
    last_used = 0;
}

Cache::Cache(int c_size, int b_size, int assoc,
             double h_time,
             double m_penalty)
{
    cache_size = c_size;
    block_size = b_size;
    associativity = assoc;
    hit_time = h_time;
    miss_penalty = m_penalty;

    int total_lines = cache_size / block_size;
    number_of_sets = total_lines / associativity;

    offset_bits = log2(block_size);
    index_bits = log2(number_of_sets);

    cache.resize(number_of_sets,
                 vector<CacheLine>(associativity));

    global_time = 0;
    hits = 0;
    misses = 0;
    writebacks = 0;
}

void Cache::access(unsigned int address, bool is_write)
{
    global_time++;

    unsigned int set_index =
        (address >> offset_bits) &
        ((1 << index_bits) - 1);

    unsigned int tag =
        address >> (offset_bits + index_bits);

    for (int i = 0; i < associativity; i++) {
        if (cache[set_index][i].valid &&
            cache[set_index][i].tag == tag)
        {
            hits++;
            cache[set_index][i].last_used = global_time;

            if (is_write)
                cache[set_index][i].dirty = true;

            return;
        }
    }

    misses++;

    for (int i = 0; i < associativity; i++) {
        if (!cache[set_index][i].valid) {
            cache[set_index][i].valid = true;
            cache[set_index][i].tag = tag;
            cache[set_index][i].last_used = global_time;
            cache[set_index][i].dirty = is_write;
            return;
        }
    }

    int lru_index = 0;
    for (int i = 1; i < associativity; i++) {
        if (cache[set_index][i].last_used <
            cache[set_index][lru_index].last_used)
        {
            lru_index = i;
        }
    }

    if (cache[set_index][lru_index].dirty) {
        writebacks++;
    }

    cache[set_index][lru_index].tag = tag;
    cache[set_index][lru_index].last_used = global_time;
    cache[set_index][lru_index].dirty = is_write;
}

void Cache::printCacheState()
{
    cout << "\n===== CACHE STATE =====\n";
    for (int i = 0; i < number_of_sets; i++) {
        cout << "Set " << i << " : ";
        for (int j = 0; j < associativity; j++) {
            if (cache[i][j].valid)
                cout << "[Tag " << cache[i][j].tag
                     << (cache[i][j].dirty ? " D" : " C")
                     << "] ";
            else
                cout << "[Empty] ";
        }
        cout << endl;
    }
}

void Cache::printStats()
{
    unsigned long long total = hits + misses;

    double miss_rate = (double)misses / total;
    double hit_rate = (double)hits / total;

    double amat =
        hit_time + miss_rate * miss_penalty;

    cout << "\n===== STATISTICS =====\n";
    cout << "Total Accesses : " << total << endl;
    cout << "Hits           : " << hits << endl;
    cout << "Misses         : " << misses << endl;
    cout << "Writebacks     : " << writebacks << endl;

    cout << fixed << setprecision(4);
    cout << "Hit Rate       : " << hit_rate << endl;
    cout << "Miss Rate      : " << miss_rate << endl;
    cout << "AMAT           : " << amat << endl;
}
