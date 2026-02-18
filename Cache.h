#ifndef CACHE_H
#define CACHE_H

#include <vector>

struct CacheLine {
    unsigned int tag;
    bool valid;
    bool dirty;
    unsigned long long last_used;

    CacheLine();
};

class Cache {
private:
    int cache_size;
    int block_size;
    int associativity;
    int number_of_sets;

    int offset_bits;
    int index_bits;

    std::vector<std::vector<CacheLine>> cache;

    unsigned long long global_time;
    unsigned long long hits;
    unsigned long long misses;
    unsigned long long writebacks;

    double hit_time;
    double miss_penalty;

public:
    Cache(int c_size, int b_size, int assoc,
          double h_time = 1.0,
          double m_penalty = 50.0);

    void access(unsigned int address, bool is_write);

    void printCacheState();
    void printStats();
};

#endif
