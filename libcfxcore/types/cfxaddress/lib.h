// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_TYPES_LIB_INCLUDE__
#define __CPP_CONFLUX_SDK_TYPES_LIB_INCLUDE__

#include <vector>
#include <stdint.h>
#include <string>
#include <array>

namespace cfx_cpp {
	using byteArray = std::vector<uint8_t>;

	byteArray  convert(const byteArray& data, uint8_t inbits, uint8_t outbits);
	bool verifyConvert(const byteArray& ihnput, uint8_t inbits, uint8_t outbits, const byteArray& expectOutput);

	// Modification based on https://github.com/hlb8122/rust-bitcoincash-addr in MIT License.
	// A copy of the original license is included in LICENSE.rust-bitcoincash-addr.
	// https://github.com/bitcoincashorg/bitcoincash.org/blob/master/spec/cashaddr.md#checksum
	uint64_t polymod(byteArray v);

	std::array<uint8_t, 8> uint64ToBytes(uint64_t num);
};

#endif 
