#include "hash.h"

uint64_t MurmurHash2_64(const void* key, int len, uint32_t seed) {
  const uint64_t m = 0xc6a4a7935bd1e995;
  const int r = 47;

  uint64_t h = seed ^ (len * m);

  const uint64_t* data = (const uint64_t *)key;
  const uint64_t* end = data + (len/8);

  while (data != end) {
    uint64_t k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  const uint8_t* data2 = (const uint8_t*)data;

  switch (len & 7) {
    case 7: h ^= static_cast<uint64_t>(data2[6]) << 48;
    case 6: h ^= static_cast<uint64_t>(data2[5]) << 40;
    case 5: h ^= static_cast<uint64_t>(data2[4]) << 32;
    case 4: h ^= static_cast<uint64_t>(data2[3]) << 24;
    case 3: h ^= static_cast<uint64_t>(data2[2]) << 16;
    case 2: h ^= static_cast<uint64_t>(data2[1]) << 8;
    case 1: h ^= static_cast<uint64_t>(data2[0]);
            h *= m;
  };

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

uint32_t MurmurHash2_32(const void* key, int len, uint32_t seed) {
  const uint32_t m = 0x5bd1e995;
  const int r = 24;

  uint32_t h = seed ^ (len * m);

  const uint32_t* data = (const uint32_t *)key;

  while (len >= 4) {
    uint32_t k = *(uint32_t *)data;

    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    data += 1;
    len -= 4;
  }

  // Handle the last few bytes of the input array
 const uint8_t* data2 = (const uint8_t*)data;

  switch (len) {
    case 3: h ^= static_cast<uint32_t>(data2[2]) << 16;
    case 2: h ^= static_cast<uint32_t>(data2[1]) << 8;
    case 1: h ^= static_cast<uint32_t>(data2[0]);
            h *= m;
  };

  // Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

uint32_t MurmurHash3_32(const void* key, int len, uint32_t seed) {
  const uint8_t* data = (const uint8_t*)key;
  const int nblocks = len / 4;

  uint32_t h1 = seed;

  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t* blocks = (const uint32_t*)(data + nblocks*4);

  for (int i = -nblocks; i; i++) {
    uint32_t k1 = blocks[i];

    k1 *= c1;
    k1 = (k1 << 15) | (k1 >> (32 - 15));
    k1 *= c2;

    h1 ^= k1;
    h1 = (h1 << 13) | (h1 >> (32 - 13));
    h1 = h1*5 + 0xe6546b64;
  }

  //----------
  // tail

  const uint8_t* tail = (const uint8_t*)(data + nblocks*4);

  uint32_t k1 = 0;

  switch (len & 3) {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1: k1 ^= tail[0];

      k1 *= c1;
      k1 = (k1 << 15) | (k1 >> (32 - 15));
      k1 *= c2;
      h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;

  h1 ^= h1 >> 16;
  h1 *= 0x85ebca6b;
  h1 ^= h1 >> 13;
  h1 *= 0xc2b2ae35;
  h1 ^= h1 >> 16;

  return h1;
}
