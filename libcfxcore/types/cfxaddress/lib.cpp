// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "lib.h"

cfx_cpp::byteArray  cfx_cpp::convert(const byteArray& data, uint8_t inbits, uint8_t outbits) {
	byteArray ret;
	if (inbits > 8 || outbits > 8)
		return ret;
	uint32_t accBits = 0;
	uint16_t  acc = 0;
	for (auto d : data) {
		acc = acc << inbits | d;
		accBits += inbits;
		while (accBits >= outbits) {
			uint8_t val = (acc >> (accBits - outbits));
			ret.push_back(val);
			acc = acc & ((1 << (accBits - outbits)) - 1);
			accBits -= outbits;
		}
	}
	if (accBits > 0 && (inbits > outbits)) {
		ret.push_back(uint8_t(acc << uint16_t(outbits - accBits)));
	}
	return ret;
}


bool cfx_cpp::verifyConvert(const cfx_cpp::byteArray& ihnput, uint8_t inbits, uint8_t outbits,
	const cfx_cpp::byteArray& expectOutput) {
	auto  expectOutputTemp = cfx_cpp::convert(ihnput, inbits, outbits);
	return expectOutputTemp == expectOutput;
}


uint64_t cfx_cpp::polymod(byteArray v) {
	uint64_t c = 1;
	for (uint8_t d : v) {
		uint8_t c0 = c >> 35;
		c = ((c & 0x07ffffffff) << 5) ^ d;

		if (c0 & 0x01) c ^= 0x98f2bc8e61;
		if (c0 & 0x02) c ^= 0x79b76d99e2;
		if (c0 & 0x04) c ^= 0xf33e5fb3c4;
		if (c0 & 0x08) c ^= 0xae2eabe2a8;
		if (c0 & 0x10) c ^= 0x1e4f43e470;
	}

	return c ^ 1;
}

std::array<uint8_t, 8> cfx_cpp::uint64ToBytes(uint64_t num) {
	std::array<uint8_t, 8> r;
	for (size_t i = 0; i < 8; i++) {
		r[7 - i] = uint8_t(num >> (i * 8));
	}
	return r;
}
