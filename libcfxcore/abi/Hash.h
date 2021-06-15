#pragma once

#include "Data.h"
#include <functional>
#include <libdevcore/SHA3.h>

namespace TW {
    namespace Hash {
        /// Computes the Keccak SHA256 hash.
        Data keccak256(const byte* data, size_t size);

        template <typename T>
        Data keccak256(const T& data) {
            ethash::hash256 h = ethash::keccak256(reinterpret_cast<const byte*>(data.data()), data.size());
            return TW::data((const byte*)&h.bytes[0], 32);
        }
    }
}