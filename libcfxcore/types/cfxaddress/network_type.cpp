// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "network_type.h"

using namespace cfx_cpp;

std::string   NetworkType::NetworkTypeMainnetPrefix = "cfx";
std::string   NetworkType::NetworkTypeTestNetPrefix = "cfxtest";
uint32_t NetworkType::NetowrkTypeMainnetID = 1029;
uint32_t NetworkType::NetworkTypeTestnetID = 1;

NetworkType::NetworkType(const std::string& type_str) :
	net_work_type_str(type_str) {

}

NetworkType::NetworkType(uint32_t networkID) {
	if (networkID == NetworkType::NetowrkTypeMainnetID)
		net_work_type_str = NetworkType::NetworkTypeMainnetPrefix;
	else if (networkID == NetworkType::NetworkTypeTestnetID)
		net_work_type_str = NetworkType::NetworkTypeTestNetPrefix;
	else {
		char spec_net_type[128];
		sprintf(spec_net_type, "net%d", networkID);
		net_work_type_str = spec_net_type;
	}
}


NetworkType::~NetworkType() {

}

uint32_t  NetworkType::ToNetworkID() const {
	if (NetworkType::NetworkTypeMainnetPrefix == net_work_type_str)
		return NetworkType::NetowrkTypeMainnetID;
	else if (NetworkType::NetworkTypeTestNetPrefix == net_work_type_str)
		return NetworkType::NetworkTypeTestnetID;
	else
		return getIDWhenBeginWithNet(net_work_type_str);
}

uint32_t  NetworkType::getIDWhenBeginWithNet(const std::string& net_type) {
	if (net_type.size() < 3)
		return 0;
	if (net_type.substr(0, 3) != "net")
		return 0;
	else
		return std::stoi(net_type.substr(3), NULL, 10);
}