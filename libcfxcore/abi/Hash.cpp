#include "Hash.h"
using namespace TW;
/// Number of bytes in a SHA256 hash.
static const size_t sha256Size = 32;
Data Hash::keccak256(const byte* data, size_t size) {
    ethash::hash256 h = ethash::keccak256(data ,size);
    return TW::data((const byte*)&h.bytes[0], 32);
}

