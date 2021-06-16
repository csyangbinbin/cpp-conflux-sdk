// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "log_filter.h"
#include <string>
#include <libdevcore/CommonJS.h>

namespace cfx_cpp {
	LogFilterParam::LogFilterParam() :
		fromEpoch("latest_checkpoint"),
		toEpoch("latest_state")
	{

	}
	LogFilterParam::~LogFilterParam() {

	}
	LogFilterParam& LogFilterParam::SetFromEpoch(const EpochNumber& v) {
		fromEpoch = v; 
		return *this; 
	
	}
	LogFilterParam& LogFilterParam::SetToEpoch(const EpochNumber& v) {
		toEpoch = v; 
		return *this;
	}

	LogFilterParam& LogFilterParam::AddBlockHash(const dev::h256& block_hash) {
		if (blockHashes.is_initialized()) {
			blockHashes.get().push_back(block_hash);
		}
		else {
			dev::h256s hs;
			hs.push_back(block_hash);
			blockHashes = hs; 
		}
	
		return *this;
	}

	LogFilterParam& LogFilterParam::AddTopic(size_t index, const dev::h256& t) {
		if (index < 4) {
			topics[index].insert(t); 
		}
		return *this; 
	}
	LogFilterParam& LogFilterParam::AddAddress(const cfx_cpp::Address& addr) {
		if (addresses.is_initialized()) {
			addresses.get().push_back(addr);
		}
		else {
			std::vector<cfx_cpp::Address> addrs_{ addr };
			addresses = addrs_; 
		}
		return *this; 
	}
	LogFilterParam& LogFilterParam::SetLimit(uint64_t v) {
		limit = v; 
		return *this; 
	}

	Json::Value LogFilterParam::toJS() const {
		Json::Value value;
		value["fromEpoch"] = std::string(fromEpoch);
		value["toEpoch"] = std::string(toEpoch);

		if (blockHashes.is_initialized()) {
			Json::Value blockHashes_;
			for (auto& bc : blockHashes.get()) {
				blockHashes_.append(dev::toJS(bc));
			}
			value["blockHashes"] = blockHashes_;
		}
		else {
			value["blockHashes"] = Json::nullValue;
		}

		if (addresses.is_initialized()) {
			Json::Value address_;
			for (auto& addr : addresses.get()) {
				address_.append(addr.GetVerboseBase32Address());
			}
			value["address"] = address_;
		}
		else {
			value["address"] = Json::nullValue;
		}

		Json::Value topics_;
		topics_.resize(4);
		for (unsigned int i = 0; i < 4; i++) {
			const dev::h256Hash& tp_set = topics[i];
			Json::Value tp_set_;
			if (!tp_set.empty()) {
				for (const auto& tp : tp_set) {
					tp_set_.append(dev::toJS(tp));
				}
				topics_[i] = tp_set_;
			}
		}

		value["topics"] = topics_;

		if (limit.is_initialized()) {
			value["limit"] = dev::toJS(limit.get());
		}
		else {
			value["limit"] = Json::nullValue;
		}

		return value;
	}



}

