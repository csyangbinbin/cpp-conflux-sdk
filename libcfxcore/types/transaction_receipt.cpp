// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "transaction_receipt.h"
#include "common/util/json_convert.h"
namespace cfx_cpp {


	bool  Json2Struct(const Json::Value& js, TransactionReceipt& retValue) {
		if (!js.isObject()) {
			return false;
		}
		jsMem2Hash(js, "transactionHash", retValue.transactionHash);
		jsMem2Uint64(js, "index", retValue.index);
		jsMem2Hash(js, "blockHash", retValue.blockHash);
		jsMem2U256(js, "epochNumber", retValue.epochNumber);
		if (jsMem2CfxAddress(js, "from", retValue.from) == false) {
			retValue.from = cfx_cpp::Address::InvaildAddress();
		}
		jsMem2CfxAddress(js, "to", retValue.to);
		jsMem2U256(js, "gasUsed", retValue.gasUsed);
		jsMem2U256(js, "gasFee", retValue.gasFee);
		jsMem2CfxAddress(js, "contractCreated", retValue.contractCreated);
		jsMem2Hash<256>(js, "logsBloom", retValue.logsBloom);
		jsMem2Hash<32>(js, "stateRoot", retValue.stateRoot);
		jsMem2Uint64(js, "outcomeStatus", retValue.outcomeStatus);
		jsMem2String(js, "txExecErrorMsg", retValue.txExecErrorMsg);
		jsMem2Boolean(js, "gasCoveredBySponsor", retValue.gasCoveredBySponsor);
		jsMem2Boolean(js, "storageCoveredBySponsor", retValue.storageCoveredBySponsor);
		jsMem2U256(js, "storageCollateralized", retValue.storageCollateralized);
		if (js.isMember("storageReleased") && js["storageReleased"].isArray()) {
			auto const& storageReleasedArray = js["storageReleased"];
			for (unsigned int i = 0; i < storageReleasedArray.size(); i++) {
				StorageChange change;
				jsMem2CfxAddress(storageReleasedArray[i], "address", change.address);
				jsMem2U256(storageReleasedArray[i], "collaterals", change.collaterals);
				retValue.storageReleased.push_back(change);
			}
		}
		if (js.isMember("logs") && js["logs"].isArray()) {
			auto const& logs = js["logs"];
			for (unsigned int i = 0; i < logs.size(); i++) {
				auto const& log = logs[i];
				cfx_cpp::Address  address;
				dev::bytes data;
				dev::h256s topics;
				jsMem2CfxAddress(log, "address", address);
				jsMem2Bytes(log, "data", data);
				if (log.isMember("topics") && log["topics"].isArray()) {

					for (auto& topic : log["topics"])
						topics.push_back(dev::jsToFixed<32>(topic.asString()));
				}
				retValue.logs.emplace_back(address, topics, data);
			}
		}
		return true;
	}

	std::ostream& operator<<(std::ostream& _out, TransactionReceipt const& _r) {
		_out << "transactionHash:" << dev::toJS(_r.transactionHash) << std::endl;
		_out << "index:" << _r.index << std::endl;
		_out << "blockHash:" << dev::toJS(_r.blockHash) << std::endl;
		_out << "epochNumber:" << dev::toJS(_r.epochNumber) << std::endl;
		_out << "from:" << _r.from.GetVerboseBase32Address() << std::endl;
		if (_r.to) {
			_out << "to:" << _r.to.get().GetVerboseBase32Address() << std::endl;
		}
		else {
			_out << "to: null" << std::endl;
		}
		_out << "gasUsed:" << dev::toJS(_r.gasUsed) << std::endl;
		_out << "gasFee:" << dev::toJS(_r.gasFee) << std::endl;
		if (_r.contractCreated) {
			_out << "contractCreated:" << _r.contractCreated.get().GetVerboseBase32Address() << std::endl;
		}
		else {
			_out << "contractCreated: null" << std::endl;
		}
		_out << "logsBloom:" << dev::toJS(_r.logsBloom) << std::endl;
		_out << "stateRoot:" << dev::toJS(_r.stateRoot) << std::endl;
		_out << "outcomeStatus:" << _r.outcomeStatus << std::endl;
		if (_r.txExecErrorMsg) {
			_out << "txExecErrorMsg:" << _r.txExecErrorMsg.get() << std::endl;
		}
		else {
			_out << "txExecErrorMsg: null" << std::endl;
		}
		_out << "gasCoveredBySponsor:" << (_r.gasCoveredBySponsor ? "true" : "false") << std::endl;
		_out << "storageCoveredBySponsor:" << (_r.storageCoveredBySponsor ? "true" : "false") << std::endl;
		_out << "storageCollateralized:" << dev::toJS(_r.storageCollateralized) << std::endl;
		_out << "logs:[" << std::endl;
		for (size_t i = 0; i < _r.logs.size(); i++) {
			const LogEntry& log = _r.logs[i];
			_out << log << ",";
		}
		_out << "]" << std::endl;

		for (size_t i = 0; i < _r.storageReleased.size(); i++) {
			std::cout << "storageReleased: [" << std::endl;
			for (auto& item : _r.storageReleased) {
				_out << "{" << std::endl;
				_out << "address:" << item.address.GetVerboseBase32Address() << "," << std::endl;
				_out << "collaterals:" << dev::toJS(item.collaterals) << "}," << std::endl; ;
			}
			_out << "]" << std::endl;
		}

		return _out;
	}

}

