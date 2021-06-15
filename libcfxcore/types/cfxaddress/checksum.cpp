// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "checksum.h"
#include <iterator>     // std::back_inserter

using namespace cfx_cpp;

cfx_cpp::byteArray cfx_cpp::CalcChecksum(NetworkType nt, Body body) {
	byteArray checksumInput;

	std::string ntString = nt.GetNetWorkTypeString();
	byteArray bodyArray = body.GetBody();

	for (auto& c : ntString) {
		checksumInput.push_back(static_cast<uint8_t>(c) & 0x1F);
	}
	uint8_t separator = 0;
	checksumInput.push_back(separator);
	std::copy(bodyArray.begin(), bodyArray.end(), std::back_inserter(checksumInput));

	byteArray templateArray(8, 0);
	std::copy(templateArray.begin(), templateArray.end(), std::back_inserter(checksumInput));

	uint64_t uint64Chc = polymod(checksumInput);

	auto tempVar = cfx_cpp::uint64ToBytes(uint64Chc);
	byteArray low40BitsChc;
	std::copy(tempVar.begin() + 3, tempVar.end(), std::back_inserter(low40BitsChc));

	auto checksumIn5Bits = cfx_cpp::convert(low40BitsChc, 8, 5);
	return checksumIn5Bits;
}