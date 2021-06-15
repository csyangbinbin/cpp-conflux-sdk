// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "rpc_parameters.h"
#include "../common/util/json_convert.h"

namespace cfx_cpp {

	bool cfx_getBlockByHashResult::fromJson(const Json::Value& js) {
		if (!js.isObject()) {
			return false;
		}
		jsMem2Boolean(js , "adaptive" ,this->adaptive);
		jsMem2Uint64(js, "blame", this->blame);
		jsMem2Hash(js, "deferredLogsBloomHash", this->deferredLogsBloomHash);
		jsMem2Hash(js, "deferredReceiptsRoot", this->deferredReceiptsRoot);
		jsMem2Hash(js, "deferredStateRoot", this->deferredStateRoot);
		jsMem2U256(js, "difficulty", this->difficulty);
		jsMem2U256(js, "epochNumber", this->epochNumber);
		jsMem2U256(js, "gasLimit", this->gasLimit);
		jsMem2U256(js, "gasUsed", this->gasUsed);
		jsMem2Hash(js, "hash", this->hash);
		jsMem2U256(js, "height", this->height);
		jsMem2CfxAddress(js ,"miner",this->miner);
		jsMem2U256(js, "nonce", this->nonce);
		jsMem2Hash(js, "parentHash", this->parentHash);
		jsMem2U256(js, "powQuality", this->powQuality);
		jsMem2Uint64(js,"size",this->size);
		jsMem2Uint64(js, "timestamp", this->timestamp);
		jsMem2Hash(js, "transactionsRoot", this->transactionsRoot);
		if (js.isMember("refereeHashes") && js["refereeHashes"].isArray()) {
			for (auto& item : js["refereeHashes"]) {
				dev::h256  h = dev::jsToFixed<32>(item.asString());
				this->refereeHashes.push_back(h);
			}
		}
		if (js.isMember("transactions") && js["transactions"].isArray()) {
			for (auto& item : js["transactions"]) {
				dev::h256  h = dev::jsToFixed<32>(item.asString());
				this->transactions.push_back(h);
			}
		}
		return true;
	}

	bool NodeStatus::fromJson(const Json::Value& js) {
		if (!js.isObject()) {
			return false;
		}
		jsMem2Hash(js, "bestHash", this->bestHash);
		jsMem2Uint64(js, "chainId", this->chainId);
		jsMem2Uint64(js, "networkId", this->networkId);
		jsMem2U256(js, "blockNumber", this->blockNumber);
		jsMem2U256(js, "epochNumber", this->epochNumber);
		jsMem2U256(js, "latestCheckpoint", this->latestCheckpoint);
		jsMem2U256(js, "latestConfirmed", this->latestConfirmed);
		jsMem2U256(js, "latestState", this->latestState);
		jsMem2U256(js, "pendingTxNumber", this->pendingTxNumber);
		return true;
	}

	bool TransactionByHashResult::fromJson(const Json::Value& js) {
		if (!js.isObject()) {
			return false;
		}
		jsMem2Hash(js, "hash", this->hash);
		jsMem2U256(js, "nonce", this->nonce);
		jsMem2Hash(js, "blockHash", this->blockHash);
		jsMem2U256(js, "transactionIndex", this->transactionIndex);
		jsMem2CfxAddress(js, "from", this->from);
		jsMem2Uint64(js, "chainId", this->chainId);
		if (jsMem2CfxAddress(js, "to", this->to) == false) {
			uint32_t chainid = boost::lexical_cast<uint32_t>(this->chainId);
			this->to = cfx_cpp::Address::NullAddress(chainid);
		}
		jsMem2U256(js, "value", this->value);
		jsMem2U256(js, "gasPrice", this->gasPrice);
		jsMem2U256(js, "gas", this->gas);

		if (jsMem2CfxAddress(js, "contractCreated", this->contractCreated) == false) {
			this->contractCreated = cfx_cpp::Address::InvaildAddress();
		}
		jsMem2Bytes(js, "data", this->data);
		jsMem2U256(js, "storageLimit", this->storageLimit);
		jsMem2U256(js, "epochHeight", this->epochHeight);

		if (js.isMember("status") && js["status"].isString()) {
			int status = dev::jsToInt(js["status"].asString());
			if (status == 0) {
				this->status = TransactionStatus::Success;
			}
			else if (status == 1) {
				this->status = TransactionStatus::ErrorOccurred;
			}
			else {
				this->status = TransactionStatus::SkippedOrNotPacked;
			}
		}
		else {
			//null when the transaction is skipped or not packed
			this->status = TransactionStatus::SkippedOrNotPacked;
		}
		if (js.isMember("r") && js["r"].isString()) {
			this->signature.r = dev::jsToFixed<32>(js["r"].asString());
		}
		if (js.isMember("s") && js["s"].isString()) {
			this->signature.s = dev::jsToFixed<32>(js["s"].asString());
		}
		if (js.isMember("v") && js["v"].isString()) {
			this->signature.v = static_cast<uint8_t>(dev::jsToInt(js["v"].asString()));
		}
		return true;
	}
	bool EstimateGasAndCollateralResult::fromJson(const Json::Value& js) {
		if (!js.isObject()) {
			return false;
		}

		jsMem2U256(js, "gasLimit", this->gasLimit);
		jsMem2U256(js, "gasUsed", this->gasUsed);
		jsMem2U256(js, "storageCollateralized", this->storageCollateralized);
		return true;

	}
}