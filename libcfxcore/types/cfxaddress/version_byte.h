// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_VERSION_BYTE_INCLUDE__
#define __CPP_CONFLUX_SDK_VERSION_BYTE_INCLUDE__
#include <stdint.h>
#include <map>
#include "lib.h"

/*
Version-byte:
The version byte's most significant bit is reserved and must be 0. The 4 next bits indicate the type of address and the 3 least significant bits indicate the size of the hash.

Type bits	Meaning	Version byte value
0			Conflux			0
Further types might be added as new features are added.

Size bits	Hash size in bits
0			160
1			192
2			224
3			256
4			320
5			384
6			448
7			512
By encoding the size of the hash in the version field, we ensure that it is possible to check that the length of the address is correct.
*/

namespace cfx_cpp {
	class VersionByte {
	public:
		VersionByte() :TypeBits(0), AddressType(0), SizeBits(0) {}
		VersionByte(uint8_t b) {
			this->TypeBits = b >> 7;
			this->AddressType = (b & 0x7f) >> 3;
			this->SizeBits = b & 0x0f;
		}
	public:
		uint8_t TypeBits;
		// current is constant 0, it's different with AddressType defined in address_type.go
		uint8_t  AddressType;
		uint8_t  SizeBits;

		static std::map<uint32_t, uint8_t> hashSizeToBits;
	public:
		uint8_t ToByte() const {
			uint8_t ret = 0;
			ret = (TypeBits << 7) & 0x80;
			ret = ret | ((AddressType & 0x0F) << 3);
			ret = ret | SizeBits;
			return ret;
		}
	};

	inline VersionByte NewVersionByte(uint8_t b) {
		VersionByte vt;
		vt.TypeBits = b >> 7;
		vt.AddressType = (b & 0x7f) >> 3;
		vt.SizeBits = b & 0x0f;
		return vt;
	}

	inline  VersionByte CalcVersionByte(cfx_cpp::byteArray hexAddress) {
		VersionByte versionByte;
		versionByte.TypeBits = 0;
		versionByte.AddressType = 0;
		size_t addrBitsLen = hexAddress.size() * 8;
		versionByte.SizeBits = versionByte.hashSizeToBits[addrBitsLen];
		return versionByte;
	}

}

#endif