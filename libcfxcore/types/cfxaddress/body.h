// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_BODY_INCLUDE__
#define __CPP_CONFLUX_SDK_BODY_INCLUDE__
#include <string>
#include <mutex> //call_once
#include <map>
#include "lib.h"
#include "version_byte.h"
/*
Base32-encode address:
To create the payload, first, concatenate the version-byte with addr to get a 21-byte array. Then, encode it left-to-right, mapping each 5-bit sequence to the corresponding ASCII character (see alphabet below). Pad to the right with zero bits to complete any unfinished chunk at the end. In our case, 21-byte payload + 2 bit 0-padding will result in a 34-byte base32-encoded string.

We use the following alphabet: abcdefghjkmnprstuvwxyz0123456789 (i, l, o, q removed).

0x00 => a    0x08 => j    0x10 => u    0x18 => 2
0x01 => b    0x09 => k    0x11 => v    0x19 => 3
0x02 => c    0x0a => m    0x12 => w    0x1a => 4
0x03 => d    0x0b => n    0x13 => x    0x1b => 5
0x04 => e    0x0c => p    0x14 => y    0x1c => 6
0x05 => f    0x0d => r    0x15 => z    0x1d => 7
0x06 => g    0x0e => s    0x16 => 0    0x1e => 8
0x07 => h    0x0f => t    0x17 => 1    0x1f => 9
*/

namespace cfx_cpp {
	class Body {
	public:
		Body(const std::string& base32Str = "");
		Body(const VersionByte& vrsByte, const byteArray& hexAddress);
		virtual ~Body() {}
		void ToHexAddress(VersionByte& vrsType, byteArray& hexAddress);
		std::string String() const;
		byteArray GetBody() const { return body; }
		static std::string bits5sToString(const cfx_cpp::byteArray& data);
	private:
		static void init();
	private:
		byteArray body;
		static std::string alphabet;
		static std::map<uint8_t, uint8_t > alphabetToIndexMap;
		static std::once_flag init_flag;
	};
}

#endif