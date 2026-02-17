#include "hash.h"
#include <vector>
#include <sstream>

namespace search {

uint64_t HashUtil::fnv1a_64(std::string_view text) {
    uint64_t hash = 14695981039346656037ULL;
    for (char c : text) {
        hash ^= static_cast<uint64_t>(static_cast<unsigned char>(c));
        hash *= 1099511628211ULL;
    }
    return hash;
}

uint64_t HashUtil::simhash_64(const std::string& text) {
    std::vector<int> v(64, 0);
    std::istringstream iss(text);
    std::string token;

    while (iss >> token) {
        uint64_t h = fnv1a_64(token);
        for (int i = 0; i < 64; ++i) {
            if ((h >> i) & 1) {
                v[i] += 1;
            } else {
                v[i] -= 1;
            }
        }
    }

    uint64_t fingerprint = 0;
    for (int i = 0; i < 64; ++i) {
        if (v[i] > 0) {
            fingerprint |= (1ULL << i);
        }
    }
    return fingerprint;
}

int HashUtil::hamming_distance(uint64_t a, uint64_t b) {
    uint64_t x = a ^ b;
    int count = 0;
    while (x > 0) {
        count += (x & 1);
        x >>= 1;
    }
    return count;
}

} // namespace search
