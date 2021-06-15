// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_ADDRESS_INCLUDE__
#define __CPP_CONFLUX_SDK_ADDRESS_INCLUDE__
#include "lib.h"
#include "address_type.h"
#include "checksum.h"
#include "body.h"
#include "network_type.h"
#include "version_byte.h"
#include <libdevcore/Address.h>

namespace cfx_cpp {

	class Address {
		friend std::ostream& operator<<(std::ostream& output, const cfx_cpp::Address& v);
	public:
		/**
		 * .
		 * 
		 * \param base32OrHex	base32 or 20bytes hex format address 
		 * \param networkID		network id
		 */
		Address(const std::string& base32OrHex = "0x0000000000000000000000000000000000000000",
			uint32_t  networkID = NetworkType::NetworkTypeTestnetID);
		virtual ~Address();
	public:
		
		static Address InvaildAddress(); 
		static Address TestnetNullAddress();
		static Address MainnetNullAddress();
		static Address NullAddress(uint32_t  networkID);
		bool IsValid() const;
		std::string GetBase32Address() const;
		std::string GetVerboseBase32Address() const;
		NetworkType GetNetworkType() const { return networkType; }
		AddressType GetAddressType() const { return addressType; }
		Body GetBody() const { return body; }
		cfx_cpp::byteArray GetChecksum() const { return checksum; }
		uint32_t GetNetworkID() const;
		std::string GetHexAddress() const;
		void ToHex(std::string& hexAddressStr, uint32_t& networkID) const;
		dev::Address toEthCommonAddr() const; 
		bool operator==(const Address& rv) const;
		bool operator<(const Address& rv) const;
	private:
		void New(const std::string& base32OrHex, uint32_t  networkID);
		int NewFromBase32(const std::string& base32Str);
		int NewFromHex(const std::string& hexAddressStr, uint32_t networkID);
		int NewFromBytes(cfx_cpp::byteArray hexAddress, uint32_t networkID);
		void setCache();
	private:
		NetworkType			networkType;
		AddressType 		addressType;
		Body				body;
		cfx_cpp::byteArray	checksum;
		cfx_cpp::byteArray	hex;
		uint32_t			networkID;
	};	
	std::ostream& operator<<(std::ostream& output, const cfx_cpp::Address& v);
}

#endif