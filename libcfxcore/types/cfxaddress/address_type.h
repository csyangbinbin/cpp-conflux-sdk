// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_ADDRESS_TYPE_INCLUDE__
#define __CPP_CONFLUX_SDK_ADDRESS_TYPE_INCLUDE__
#include <string>
#include "lib.h"
/*
	Address-type: For the null address (0x0000000000000000000000000000000000000000), address-type must be "type.null". Otherwise,
	match addr[0] & 0xf0
		case b00000000: "type.builtin"
		case b00010000: "type.user"
		case b10000000: "type.contract"
*/
namespace cfx_cpp {
	/**
	 * address type,There are four types( null,builtin,usercontract ).
	 */
	class AddressType {
	public:
		/**
		 * constructor function.
		 * 
		 * \param addr_type_str type string(null,builtin,usercontract)
		 */
		AddressType(const std::string addr_type_str = "invalid");
		virtual ~AddressType();
		/**
		 * return the original type string does not contain prefix "type".
		 * 
		 * \return  original type string
		 */
		std::string Type() const { return address_type_str; }
		/**
		 * addr[0] & 0xf0 ,a byte represents type ,ref https://github.com/Conflux-Chain/CIPs/blob/master/CIPs/cip-37.md .
		 * 
		 * \return  a byte represents type
		 */
		uint8_t ToByte() const;
		/**
		 * return address type string with "type." prefix.
		 * 
		 * \return address type
		 */
		std::string String() const;
	private:
		std::string address_type_str;
	public:
		static  const std::string	AddressTypeBuiltin; ///< contract
		static const std::string	AddressTypeUser; ///< user
		static const std::string	AddressTypeContract; ///< builtin
		static const std::string	AddressTypeNull;	///< null
		static const std::string	AddressTypeInvalid;	///< User-defined, indicating an invalid value
	};

	/**
	 * get address type of hex20 format address.
	 * 
	 * \param addr_type_str hex20 type addaress
	 * 
	 * \return address type
	 */
	AddressType CalcAddressType(const cfx_cpp::byteArray& hexAddress);

}

#endif