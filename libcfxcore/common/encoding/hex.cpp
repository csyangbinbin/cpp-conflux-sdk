// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "hex.h"
uint8_t  cfx_cpp::encoding::hex::fromHexChar(uint8_t c, bool* isOK) {
	if ('0' <= c && c <= '9') {
		if (isOK)
			*isOK = true;
		return c - '0';
	}
	else if ('a' <= c && c <= 'f') {
		if (isOK)
			*isOK = true;
		return c - 'a' + 10;
	}
	else if ('A' <= c && c <= 'F') {
		if (isOK)
			*isOK = true;
		return c - 'A' + 10;
	}
	else {
		if (isOK)
			*isOK = false;
		return 0;
	}
}


int cfx_cpp::encoding::hex::Decode(std::vector<uint8_t>& dst, const std::string& src) {
	size_t i = 0;
	size_t j = 1;
	bool ok = false;
	for (; j < src.size(); j += 2) {
		uint8_t a = cfx_cpp::encoding::hex::fromHexChar(src[j - 1], &ok);
		if (!ok) {
			return i;
		}
		uint8_t b = cfx_cpp::encoding::hex::fromHexChar(src[j], &ok);
		if (!ok) {
			return i;
		}
		dst.push_back((a << 4) | b);
		i++;
	}
	if ((src.size() % 2) == 1) {
		return 0;
	}
	return i;
}


std::vector<uint8_t> cfx_cpp::encoding::hex::DecodeString(const std::string& src) {
	std::vector<uint8_t> dst;
	Decode(dst, src);
	return dst;
}

std::string cfx_cpp::encoding::hex::EncodeToString(const std::vector<uint8_t>& src) {
	static const char hextable[] = "0123456789abcdef";
	std::string dst;
	for (auto v : src) {
		dst.push_back(hextable[v >> 4]);
		dst.push_back(hextable[v & 0x0f]);
	}
	return dst;
}