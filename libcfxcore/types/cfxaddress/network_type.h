// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef CPP_CONFLUX_SDK_NETWORK_TYPE_H
#define CPP_CONFLUX_SDK_NETWORK_TYPE_H

#include <string>
#include <stdint.h>
namespace cfx_cpp {
	class NetworkType {
	public:
		NetworkType(const std::string& type_str = "cfxtest");
		NetworkType(uint32_t networkID = 1);
		virtual ~NetworkType();
	public:
		uint32_t  ToNetworkID() const;
		static uint32_t  getIDWhenBeginWithNet(const std::string& net_type);
		std::string GetNetWorkTypeString() const { return net_work_type_str; }
	public:
		std::string net_work_type_str;
	public:
		static   std::string   NetworkTypeMainnetPrefix;
		static   std::string   NetworkTypeTestNetPrefix;
		static   uint32_t NetowrkTypeMainnetID;
		static   uint32_t NetworkTypeTestnetID;
	};
}

#endif //CPP_CONFLUX_SDK_NETWORK_TYPE_H
