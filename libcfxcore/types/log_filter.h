// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_LOG_FILTER_INCLUDE__
#define __CPP_CONFLUX_SDK_LOG_FILTER_INCLUDE__
#include <stdint.h>
#include <libdevcore/Common.h>
#include <libdevcore/FixedHash.h>
#include <boost/optional.hpp>
#include <vector>
#include <array>
#include <json/json.h>
#include "epoch_number.h"
#include "cfxaddress/address.h"

/*
Parameters#
Object - A log filter object:
fromEpoch: QUANTITY|TAG - (optional, default: "latest_checkpoint") the epoch number, or the string "latest_state", "latest_confirmed", "latest_checkpoint" or "earliest", see the epoch number parameter. Search will be applied from this epoch number.
toEpoch: QUANTITY|TAG - (optional, default: "latest_state") the epoch number, or the string "latest_state", "latest_confirmed", "latest_checkpoint" or "earliest", see the epoch number parameter. Search will be applied up until (and including) this epoch number.
blockHashes: Array of DATA - (optional, default: null) Array of up to 128 block hashes that the search will be applied to. This will override from/to epoch fields if it's not null.
address: Array of BASE32 - (optional, default: null) Search contract addresses. If null, match all. If specified, the log must be produced by one of these contracts.
topics: Array - (optional, default: null) 32-byte earch topics. Logs can have 4 topics: the event signature and up to 3 indexed event arguments. The elements of topics match the corresponding log topics. Example: ["0xA", null, ["0xB", "0xC"], null] matches logs with "0xA" as the 1st topic AND ("0xB" OR "0xC") as the 3rd topic. If null, match all.
limit: QUANTITY - (optional, default: null) If null return all logs, otherwise should only return the last limit logs. Note: if the node has get_logs_filter_max_limit set, it will override limit if it is null or greater than the preset value.
*/
namespace cfx_cpp {


	class LogFilterParam {
	public:
		LogFilterParam();
		virtual ~LogFilterParam();

	public:
		Json::Value toJS() const ;
		LogFilterParam& SetFromEpoch(const EpochNumber& v);
		LogFilterParam& SetToEpoch(const EpochNumber& v);
		LogFilterParam& AddAddress(const cfx_cpp::Address& addr );
		LogFilterParam& AddBlockHash(const dev::h256& block_hash);
		LogFilterParam& AddTopic(size_t index, const dev::h256& t); 
		LogFilterParam& SetLimit(uint64_t v); 

	private:
		EpochNumber fromEpoch;
		EpochNumber toEpoch;
		boost::optional<dev::h256s> blockHashes;
		boost::optional< std::vector<cfx_cpp::Address> > addresses;
		std::array<dev::h256Hash, 4> topics;
		boost::optional<uint64_t> limit; 

	};
}



#endif