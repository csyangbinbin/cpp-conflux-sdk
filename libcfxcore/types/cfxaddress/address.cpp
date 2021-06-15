// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "address.h"
#include <regex>
#include "common/util/string_util.h"
#include "common/encoding/hex.h"

namespace cfx_cpp {
	Address::Address(const std::string& base32OrHex, uint32_t  networkID) :
		networkType(networkID) {
		New(base32OrHex, networkID);
	}
	Address::~Address() {

	}

	void Address::New(const std::string& base32OrHex, uint32_t  networkID) {

		std::string base32OrHex_lower = cfx_cpp::string_util::tolower_string(base32OrHex);

		const std::string  hexPattern = R"(^0x[a-f0-9]{40}$)";
		const std::string  base32Pattern = R"(^(cfx|cfxtest|net\d+):(type\.user:|type\.builtin:|type\.contract:|type\.null:|)\w{42}$)";

		std::regex reHexPattern(hexPattern);
		std::regex reBase32Pattern(base32Pattern);

		if (std::regex_match(base32OrHex_lower, reHexPattern)) {
			NewFromHex(base32OrHex_lower, networkID);
			return;
		}
		else if (std::regex_match(base32OrHex_lower, reBase32Pattern)) {
			int retCode = NewFromBase32(base32OrHex_lower);
			if (retCode <= 0) {
				this->addressType = AddressType(AddressType::AddressTypeInvalid);
				return;
			}

			if ((networkID != 0) && (this->GetNetworkID() != networkID)) {
				NewFromHex(base32OrHex_lower, networkID);
				return;
			}
		}
		else {
			//invalid address
			this->addressType = AddressType(AddressType::AddressTypeInvalid);
			return;
		}
	}

	int Address::NewFromHex(const std::string& hexAddressStr, uint32_t networkID) {
		std::string prefix = "0x";
		if (!string_util::starts_with(hexAddressStr, prefix)) {
			return 0;
		}
		cfx_cpp::byteArray hexAddress = encoding::hex::DecodeString(hexAddressStr.substr(2));
		return NewFromBytes(hexAddress, networkID);
	}

	int Address::NewFromBytes(cfx_cpp::byteArray hexAddress, uint32_t networkID) {
		this->networkType = NetworkType(networkID);
		this->addressType = CalcAddressType(hexAddress);
		auto versionByte = CalcVersionByte(hexAddress);
		this->body = cfx_cpp::Body(versionByte, hexAddress);
		this->checksum = CalcChecksum(networkType, body);
		setCache();
		return 1;
	}

	void Address::setCache() {
		VersionByte vrsType_;
		hex.clear();
		body.ToHexAddress(vrsType_, hex);
		networkID = networkType.ToNetworkID();
	}

	int Address::NewFromBase32(const std::string& base32Str) {
		if (cfx_cpp::string_util::tolower_string(base32Str) != base32Str && \
			cfx_cpp::string_util::toupper_string(base32Str) != base32Str) {
			return 0;
		}
		std::string base32StrLower = cfx_cpp::string_util::tolower_string(base32Str);

		auto parts = cfx_cpp::string_util::split_string(base32StrLower, ':');
		if (parts.size() < 2 || parts.size() > 3) {
			return 0;
		}

		this->networkType = NetworkType(parts[0]);
		auto bodyWithChecksum = parts[parts.size() - 1];
		if (bodyWithChecksum.size() < 8)
			return 0;

		auto bodyStr = bodyWithChecksum.substr(0, bodyWithChecksum.size() - 8);
		this->body = cfx_cpp::Body(bodyStr);

		VersionByte vrsType_;
		byteArray hexAddress;
		this->body.ToHexAddress(vrsType_, hexAddress);
		this->addressType = CalcAddressType(hexAddress);

		if ((parts.size() == 3) && \
			cfx_cpp::string_util::tolower_string(parts[1]) != this->addressType.String()) {
			return 0;
		}

		this->checksum = CalcChecksum(this->networkType, this->body);
		std::string expectChk = cfx_cpp::Body::bits5sToString(this->checksum);
		std::string actualChk = bodyWithChecksum.substr(bodyWithChecksum.size() - 8);
		if (expectChk != actualChk) {
			return 0;
		}
		setCache();
		return 1;
	}


	uint32_t Address::GetNetworkID() const {
		return networkType.ToNetworkID();
	}
	void Address::ToHex(std::string& hexAddressStr, uint32_t& networkID) const {
		hexAddressStr = cfx_cpp::encoding::hex::EncodeToString(this->hex);
		networkID = this->networkID;
	}
	std::string Address::GetHexAddress() const {
		std::string hexAddressStr;
		uint32_t networkID;
		ToHex(hexAddressStr, networkID);
		return "0x"+hexAddressStr;
	}

	bool Address::IsValid() const {
		std::string address_type_string = this->addressType.Type();
		return address_type_string == AddressType::AddressTypeNull || \
			address_type_string == AddressType::AddressTypeBuiltin || \
			address_type_string == AddressType::AddressTypeUser || \
			address_type_string == AddressType::AddressTypeContract;
	}

	bool Address::operator==(const Address& rv) const {
		if (rv.networkType.GetNetWorkTypeString() != this->networkType.GetNetWorkTypeString())
			return false;
		if (rv.addressType.String() != this->addressType.String())
			return false;
		if (rv.body.GetBody() != rv.body.GetBody())
			return false;
		if (rv.checksum != this->checksum)
			return false;

		return true;
	}

	bool Address::operator<(const Address& rv) const {
		return this->GetVerboseBase32Address() < rv.GetVerboseBase32Address();
	}

	std::string Address::GetBase32Address() const {
		char retValue[128];
		sprintf(retValue, "%s:%s%s",
			this->networkType.GetNetWorkTypeString().c_str(),
			this->body.String().c_str(),
			cfx_cpp::Body::bits5sToString(this->checksum).c_str());
		return retValue;
	}
	std::string Address::GetVerboseBase32Address() const {
		char retValue[128];
		sprintf(retValue, "%s:%s:%s%s",
			this->networkType.GetNetWorkTypeString().c_str(),
			this->addressType.String().c_str(),
			this->body.String().c_str(),
			cfx_cpp::Body::bits5sToString(this->checksum).c_str());
		return retValue;
	}

	dev::Address Address::toEthCommonAddr() const {
		dev::Address ret;
		for (size_t i = 0; i < 20; i++) {
			ret[i] = this->hex[i];
		}
		return ret;

	}

	std::ostream& operator<<(std::ostream& output, const cfx_cpp::Address& v) {
		output << v.GetVerboseBase32Address();
		return output;
	}


	Address Address::InvaildAddress() {
		cfx_cpp::Address addr;
		addr.addressType = cfx_cpp::AddressType(cfx_cpp::AddressType::AddressTypeInvalid);
		return addr;

	}
	Address Address::TestnetNullAddress() {
		return cfx_cpp::Address("0x0000000000000000000000000000000000000000", NetworkType::NetworkTypeTestnetID);

	}
	Address Address::MainnetNullAddress() {
		return cfx_cpp::Address("0x0000000000000000000000000000000000000000", NetworkType::NetowrkTypeMainnetID);
	}
	Address Address::NullAddress(uint32_t  networkID) {
		return cfx_cpp::Address("0x0000000000000000000000000000000000000000", networkID);
	}
}