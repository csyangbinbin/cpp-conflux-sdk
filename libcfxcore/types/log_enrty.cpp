// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "log_enrty.h"
#include <libdevcore/SHA3.h>
#include <libdevcore/CommonJS.h>
#include <cassert>
#include "common/util/json_convert.h"

namespace cfx_cpp
{
	LogEntry::LogEntry(dev::RLP const& _r) {
		assert(_r.itemCount() == 3);
		address = (dev::Address)_r[0];
		topics = _r[1].toVector<dev::h256>();
		data = _r[2].toBytes();
	}

	void LogEntry::streamRLP(dev::RLPStream& _s) const {
		_s.appendList(3) << address << topics << data;
	}

	LogBloom LogEntry::bloom() const {
		LogBloom ret;
		ret.shiftBloom<3>(sha3(address.ref()));
		for (auto t : topics)
			ret.shiftBloom<3>(sha3(t.ref()));
		return ret;
	}

	std::ostream& operator<<(std::ostream& _out, LogEntry const& _r) {
		_out << "{ \r\n address:" << _r.cfx_address.GetVerboseBase32Address() << "," << std::endl;
		_out << "data:" << dev::toJS(_r.data) << "," << std::endl;
		_out << "topics:[" << std::endl;
		for (auto& topic : _r.topics) {
			_out << dev::toJS(topic) << "," << std::endl;
		}
		_out << "]}" << std::endl;
		return _out;
	}

	std::ostream& operator<<(std::ostream& _out, LocalisedLogEntry const& _r) {
		_out << "{ blockHash:" << dev::toJS(_r.blockHash) << "," << std::endl;
		_out << " epochNumber:" << std::string(_r.epochNumber) << "," << std::endl;
		_out << " logIndex:" << _r.logIndex << "," << std::endl;
		_out << " transactionHash:" << dev::toJS(_r.transactionHash) << "," << std::endl;
		_out << " transactionIndex:" << _r.transactionIndex << "," << std::endl;
		_out << " transactionLogIndex:" << _r.transactionLogIndex << "," << std::endl;
		_out << "{ \r\n address:" << _r.cfx_address.GetVerboseBase32Address() << "," << std::endl;
		_out << "data:" << dev::toJS(_r.data) << "," << std::endl;
		_out << "topics:[" << std::endl;
		for (auto& topic : _r.topics) {
			_out << dev::toJS(topic) << "," << std::endl;
		}
		_out << "]}" << std::endl;
		return _out;
	}


	bool  LocalisedLogEntry::fromJson(const Json::Value& js, LocalisedLogEntry& entry) {
		if (!js.isObject())
			return false;

		cfx_cpp::Address addr;
		dev::bytes data;
		dev::h256s topics;

		jsMem2CfxAddress(js, "address", addr);
		jsMem2Bytes(js, "data", data);

		if (js.isMember("topics") && js["topics"].isArray()) {
			auto const& tp_array = js["topics"];
			for (unsigned int i = 0; i < tp_array.size(); i++) {
				topics.push_back(dev::jsToFixed<32>(tp_array[i].asString()));
			}
		}

		LogEntry log_entry(addr, topics, data);
		LocalisedLogEntry retValue(log_entry);

		jsMem2Hash<32>(js, "blockHash", retValue.blockHash);

		dev::u256 epoch;
		jsMem2U256(js, "epochNumber", epoch);
		retValue.epochNumber = cfx_cpp::EpochNumber(epoch);
		jsMem2Uint64(js, "logIndex", retValue.logIndex);
		jsMem2Hash<32>(js, "transactionHash", retValue.transactionHash);
		jsMem2Uint64(js, "transactionIndex", retValue.transactionIndex);
		jsMem2Uint64(js, "transactionLogIndex", retValue.transactionLogIndex);

		entry = retValue;

		return true;
	}
}
