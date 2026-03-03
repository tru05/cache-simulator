#include "Cache.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstdlib>   // for rand()
using namespace std;

CacheLine::CacheLine() {
    tag = 0;
    valid = false;
    dirty = false;
    last_used = 0;
}

Cache::Cache(int c_size, int b_size, int assoc,
             double h_time,
             double m_penalty,
             ReplacementPolicy rp)
{
    cache_size = c_size;
    block_size = b_size;
    associativity = assoc;
    hit_time = h_time;
    miss_penalty = m_penalty;
    policy = rp;

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

            if (policy == LRU) {
                cache[set_index][i].last_used = global_time;
            }

            if (is_write)
                cache[set_index][i].dirty = true;

            return;
        }
    }

    misses++;

    // Try empty slot first
    for (int i = 0; i < associativity; i++) {
        if (!cache[set_index][i].valid) {
            cache[set_index][i].valid = true;
            cache[set_index][i].tag = tag;
            cache[set_index][i].last_used = global_time;
            cache[set_index][i].dirty = is_write;
            return;
        }
    }

  
    int victim_index = 0;

    if (policy == LRU) {
        for (int i = 1; i < associativity; i++) {
            if (cache[set_index][i].last_used <
                cache[set_index][victim_index].last_used)
            {
                victim_index = i;
            }
        }
    }
    else if (policy == FIFO) {
        // FIFO uses insertion time only (last_used never updated on hit)
        for (int i = 1; i < associativity; i++) {
            if (cache[set_index][i].last_used <
                cache[set_index][victim_index].last_used)
            {
                victim_index = i;
            }
        }
    }
    else if (policy == RANDOM) {
        victim_index = rand() % associativity;
    }

    if (cache[set_index][victim_index].dirty) {
        writebacks++;
    }

    cache[set_index][victim_index].tag = tag;
    cache[set_index][victim_index].last_used = global_time;
    cache[set_index][victim_index].dirty = is_write;
}
