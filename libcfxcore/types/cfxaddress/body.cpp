// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "body.h"
#include <iterator>     // std::back_inserter

using namespace cfx_cpp;

std::string Body::alphabet = "abcdefghjkmnprstuvwxyz0123456789";
std::map<uint8_t, uint8_t > Body::alphabetToIndexMap;
std::once_flag Body::init_flag;

void Body::init() {
	for (size_t i = 0; i < alphabet.size(); i++) {
		uint8_t c = alphabet[i];
		if (c != 0)
			alphabetToIndexMap[c] = i;
	}
}

Body::Body(const std::string& base32Str) {
	std::call_once(init_flag, &Body::init);

	for (auto& c : base32Str) {
		uint8_t index = alphabetToIndexMap[c];
		body.push_back(index);
	}
}
Body::Body(const VersionByte& vrsByte, const byteArray& hexAddress) {
	std::call_once(init_flag, &Body::init);

	body.push_back(vrsByte.ToByte());
	body.insert(body.end(), hexAddress.begin(), hexAddress.end());
	body = cfx_cpp::convert(body, 8, 5);
}

void Body::ToHexAddress(VersionByte& vrsType, byteArray& hexAddress) {
	byteArray b = cfx_cpp::convert(body, 5, 8);
	vrsType = b[0];
	std::copy(b.begin() + 1, b.end(), std::back_inserter(hexAddress));
}

std::string Body::String() const {
	return bits5sToString(body);
}

std::string Body::bits5sToString(const cfx_cpp::byteArray& data) {
	std::string ret;
	for (auto& c : data)
		ret.push_back(alphabet[c]);
	return ret;
}