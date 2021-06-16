// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CFX_STRING_JSON_CONVERT_INCLUDE__
#define __CFX_STRING_JSON_CONVERT_INCLUDE__
#include <json/json.h>
#include <libdevcore/CommonJS.h>
#include "string_util.h"
#include "common/encoding/hex.h"
#include <libcfxcore/types/cfxaddress/address.h>

namespace cfx_cpp {
	inline bool jsHasStringValue(const Json::Value& js, const std::string& mem) {
		return js.isMember(mem) && js[mem].isString();
	}

	inline bool jsHasBooleanValue(const Json::Value& js, const std::string& mem) {
		return js.isMember(mem) && js[mem].isBool();
	}

	template<unsigned N = 32, typename T >
	bool jsMem2Hash(const Json::Value& js, const std::string& mem, T& v) {
		if (jsHasStringValue(js, mem)) {
			v = dev::jsToFixed<N>(js[mem].asString());
			return true;
		}
		else
			return false;
	}
	template<typename T>
	bool jsMem2U256(const Json::Value& js, const std::string& mem, T& v) {
		if (jsHasStringValue(js, mem)) {
			v = dev::jsToU256(js[mem].asString());
			return true;
		}
		else
			return false;
	}
	template<typename T>
	bool jsMem2Uint64(const Json::Value& js, const std::string& mem, T& v) {
		if (jsHasStringValue(js, mem)) {
			v = static_cast<uint64_t>(dev::jsToInt(js[mem].asString()));
			return true;
		}
		else
			return false;
	}
	template<typename T>
	bool jsMem2CfxAddress(const Json::Value& js, const std::string& mem, T& v) {
		if (jsHasStringValue(js, mem)) {
			v = cfx_cpp::Address(cfx_cpp::string_util::tolower_string(js[mem].asString()));
			return true;
		}
		else
			return false;
	}
	template<typename T>
	bool jsMem2String(const Json::Value& js, const std::string& mem, T& v) {
		if (jsHasStringValue(js, mem)) {
			v = js[mem].asString();
			return true;
		}
		else
			return false;
	}

	template<typename T>
	bool jsMem2Boolean(const Json::Value& js, const std::string& mem, T& v) {
		if (jsHasBooleanValue(js, mem)) {
			v = js[mem].asBool();
			return true;
		}
		else
			return false;
	}

	template<typename T>
	bool jsMem2Bytes(const Json::Value& js, const std::string& mem, T& v) {
		if (jsHasStringValue(js, mem)) {
			std::string str = js[mem].asString();
			if (str.size() > 2) { // 0x prefix
				v = cfx_cpp::encoding::hex::DecodeString(str.substr(2));
			}
			return true;
		}
		else
			return false;
	}

}
#endif
