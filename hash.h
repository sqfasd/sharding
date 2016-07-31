#ifndef HASH_H_
#define HASH_H_

#include <stdint.h>

uint64_t MurmurHash2_64(const void* key, int len, uint32_t seed);
uint32_t MurmurHash2_32(const void* key, int len, uint32_t seed);
uint32_t MurmurHash3_32(const void* key, int len, uint32_t seed);

#endif  // HASH_H_
