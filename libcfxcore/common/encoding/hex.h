// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_ENCODING_HEX_INCLUDE__
#define __CPP_CONFLUX_SDK_ENCODING_HEX_INCLUDE__
#include <stdint.h>
#include <vector>
#include <string>

namespace cfx_cpp {
	namespace encoding {
		namespace hex {
			uint8_t fromHexChar(uint8_t c, bool* isOK = NULL);
			int Decode(std::vector<uint8_t>& dst, const std::string& src);
			std::vector<uint8_t> DecodeString(const std::string& src);
			std::string EncodeToString(const std::vector<uint8_t>& src);
		}
	}
}

#endif