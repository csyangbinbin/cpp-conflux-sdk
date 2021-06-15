// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef  __CPP_CONFLUX_SDK_LOG_ENTRY_TYPE_INCLUDE__
#define __CPP_CONFLUX_SDK_LOG_ENTRY_TYPE_INCLUDE__

#include <libdevcore/RLP.h>
#include "bloom_filter.h"
#include "epoch_number.h"
#include <json/json.h>

namespace cfx_cpp
{
	class dev::RLP;
	class dev::RLPStream;

	struct LogEntry
	{
		LogEntry() = default;
		explicit LogEntry(dev::RLP const& _r);
		LogEntry(cfx_cpp::Address const& _address, dev::h256s _topics, dev::bytes _data) :
			address(_address.toEthCommonAddr()), topics(std::move(_topics)), data(std::move(_data)), cfx_address(_address)
		{}

		void streamRLP(dev::RLPStream& _s) const;

		LogBloom bloom() const;

		dev::Address address;
		cfx_cpp::Address cfx_address;
		dev::h256s topics;
		dev::bytes data;
	};

	using LogEntries = std::vector<LogEntry>;


	struct LocalisedLogEntry : public LogEntry {
		LocalisedLogEntry() = default;
		explicit LocalisedLogEntry(const LogEntry& _le) : LogEntry(_le) {}

		LocalisedLogEntry(
			const LogEntry& _le,
			const dev::h256& _blockHash,
			const cfx_cpp::EpochNumber& _blockNumber,
			const dev::h256& _transactionHash,
			uint64_t _transactionIndex,
			uint64_t _logIndex,
			uint64_t _transactionLogIndex
		) :LogEntry(_le),
			blockHash(_blockHash),
			epochNumber(_blockNumber),
			logIndex(_logIndex),
			transactionHash(_transactionHash),
			transactionIndex(_transactionIndex),
			transactionLogIndex(_transactionLogIndex) {}


		static bool  fromJson(const Json::Value& js, LocalisedLogEntry& entry);

		dev::h256 blockHash;
		cfx_cpp::EpochNumber epochNumber;
		uint64_t logIndex;
		dev::h256 transactionHash;
		uint64_t transactionIndex;
		uint64_t transactionLogIndex;

	};

	using LocalisedLogEntries = std::vector<LocalisedLogEntry>;

	inline LogBloom bloom(LogEntries const& _logs)
	{
		LogBloom ret;
		for (auto const& l : _logs)
			ret |= l.bloom();
		return ret;
	}
	std::ostream& operator<<(std::ostream& _out, LogEntry const& _r);
	std::ostream& operator<<(std::ostream& _out, LocalisedLogEntry const& _r);
}

#endif