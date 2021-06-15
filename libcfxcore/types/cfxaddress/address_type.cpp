// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "address_type.h"
#include "common/encoding/hex.h"

using namespace cfx_cpp;

const std::string	cfx_cpp::AddressType::AddressTypeBuiltin = "builtin";
const std::string	cfx_cpp::AddressType::AddressTypeUser = "user";
const std::string	cfx_cpp::AddressType::AddressTypeContract = "contract";
const std::string	cfx_cpp::AddressType::AddressTypeNull = "null";
const std::string	cfx_cpp::AddressType::AddressTypeInvalid = "invalid";


AddressType::AddressType(const std::string addr_type_str) :
	address_type_str(addr_type_str) {
}
AddressType::~AddressType() {

}

AddressType cfx_cpp::CalcAddressType(const cfx_cpp::byteArray& hexAddress) {
	cfx_cpp::byteArray nullAddr = cfx_cpp::encoding::hex::DecodeString("0000000000000000000000000000000000000000");
	if (hexAddress == nullAddr)
		return AddressType::AddressTypeNull;

	uint8_t addrTypeBits = hexAddress[0] & 0xf0;
	if (addrTypeBits == 0x00) {
		return AddressType::AddressTypeBuiltin;
	}
	else if (addrTypeBits == 0x10) {
		return AddressType::AddressTypeUser;
	}
	else if (addrTypeBits == 0x80) {
		return AddressType::AddressTypeContract;
	}
	else {
		return AddressType::AddressTypeInvalid;
	}
}

uint8_t AddressType::ToByte() const {
	if (address_type_str == AddressType::AddressTypeBuiltin) {
		return 0x00;
	}
	else if (address_type_str == AddressType::AddressTypeUser) {
		return 0x10;
	}
	else if (address_type_str == AddressType::AddressTypeContract) {
		return 0x80;
	}
	else if (address_type_str == AddressType::AddressTypeNull) {
		return 0x00;
	}
	else {
		return 0x00;
	}
}

std::string AddressType::String() const {
	return std::string("type.") + address_type_str;
}