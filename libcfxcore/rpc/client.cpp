// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "client.h"
#include <thread>         // std::thread
#include <chrono>         // std::chrono::seconds
#include <libdevcore/Common.h>
#include <libdevcore/CommonJS.h>
#include <libdevcore/FixedHash.h>
#include "rpc/httpclient.h"
#include "TransactionBase.h"
#include "common/util/string_util.h"
#include "common/encoding/hex.h"
#include "common/util/json_convert.h"
#include "../contract.h"

namespace cfx_cpp {
	dev::u256	JsonRpcClient::MIN_GAS_PRICE = 1;
	dev::u256	JsonRpcClient::TRANSACTION_GAS = 21000;
	dev::u256	JsonRpcClient::TRANSACTION_STORAGE_LIMIT = 0;
	DecFloat50	JsonRpcClient::defaultGasRatio = 1.1;
	DecFloat50	JsonRpcClient::defaultStorageRatio = 1.1;

	JsonRpcClient::JsonRpcClient(const std::string& remote_addr, uint32_t  networkID) :
		m_networkID(networkID),
		m_remoteSrever(remote_addr) {
	}
	JsonRpcClient::~JsonRpcClient() {
	}

	std::string JsonRpcClient::cfx_clientVersion() {
		return stub_clientPtr->cfx_clientVersion();
	}


	NodeStatus JsonRpcClient::cfx_getStatus() {
		NodeStatus retValue;
		auto js = stub_clientPtr->cfx_getStatus();
		bool ok = retValue.fromJson(js);
		assert("ok" && "JsonRpcClient::cfx_getStatus from Json failed!");
		return retValue;
	}

	dev::u256 JsonRpcClient::cfx_getNextNonce(cfx_cpp::Address address, cfx_cpp::EpochNumber en) {
		return  dev::jsToU256(stub_clientPtr->cfx_getNextNonce(address.GetVerboseBase32Address(), en));
	}


	dev::u256 JsonRpcClient::cfx_epochNumber(cfx_cpp::EpochNumber en) {
		return  dev::jsToU256(stub_clientPtr->cfx_epochNumber(en));
	}


	dev::u256 JsonRpcClient::cfx_gasPrice() {
		return  dev::jsToU256(stub_clientPtr->cfx_gasPrice());
	}

	std::string JsonRpcClient::sendTransaction(TransactionBase& transaction) {
		ApplyUnsignedTransactionDefault(&transaction);
		auto raw = transaction.SignTransaction();
		return cfx_sendRawTransaction(raw);
	}
	std::string JsonRpcClient::sendTransaction(const cfx_cpp::Address& to, const dev::u256& value, const dev::Secret& secret) {
		cfx_cpp::TransactionBase trans(to, value, secret);
		return sendTransaction(trans);

	}

	std::string JsonRpcClient::cfx_sendRawTransaction(dev::bytes raw) {
		return stub_clientPtr->cfx_sendRawTransaction(dev::toJS(raw));
	}


	TransactionByHashResult	JsonRpcClient::cfx_getTransactionByHash(const std::string& transactionHash) {
		TransactionByHashResult retValue;
		Json::Value js = stub_clientPtr->cfx_getTransactionByHash(transactionHash);
		bool isok = retValue.fromJson(js);
		assert(isok && "JsonRpcClient::cfx_getTransactionByHash fromJson failed!");
		return retValue;
	}
	ContractDeployResult	JsonRpcClient::DeployContract(const dev::bytes& contractBytes, const dev::bytes& constructor_abi, const dev::Secret& secret) {
		dev::bytes payload = contractBytes;
		if (constructor_abi.size() > 0)
			payload.insert(payload.end(), constructor_abi.begin(), constructor_abi.end());
		cfx_cpp::TransactionBase trans(payload, secret);
		return DeployContract(trans);
	}

	ContractDeployResult	JsonRpcClient::DeployContract(TransactionBase& transaction) {
		ApplyUnsignedTransactionDefault(&transaction);
		auto raw = transaction.SignTransaction();
		std::string transHash = cfx_sendRawTransaction(raw);
		cfx_cpp::TransactionByHashResult	res;
		for (;;) {
			res = cfx_getTransactionByHash(transHash);
			if (res.status == TransactionStatus::SkippedOrNotPacked) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //wait status
			}
			else
				break;
		}
#if defined(WIN32)
		if (res.IsOK())
			std::cout << "DeployContract Success: contractCreated:" << res.contractCreated.GetVerboseBase32Address() << std::endl;
		else
			std::cout << "DeployContract Failed:" << res.status << std::endl;
#endif
		ContractDeployResult result;
		result.status = res.status;
		if (res.IsOK()) {
			result.contractCreatedAddr = res.contractCreated;
			result.transactionHash = res.hash;
			result.deployedContract = std::make_shared<Contract>(shared_from_this(), res.contractCreated);
		}
		else {
			result.contractCreatedAddr = cfx_cpp::Address::InvaildAddress();
			result.transactionHash = dev::h256();
			result.deployedContract.reset();
		}
		return result;
	}

	EstimateGasAndCollateralResult JsonRpcClient::cfx_estimateGasAndCollateral(const TransactionBase* transaction, cfx_cpp::EpochNumber en) {
		Json::Value param1;
		//param1["gas"] = dev::toJS(transaction->m_gas);
		param1["gasPrice"] = dev::toJS(transaction->m_gasPrice);
		param1["nonce"] = dev::toJS(transaction->m_nonce);
		param1["value"] = dev::toJS(transaction->m_value);
		std::string  js_data_str = dev::toJS(transaction->m_data);
		param1["data"] = js_data_str.empty() ? "0x" : js_data_str; //if data is empty ,set "0x"
		if (!transaction->isCreation()) {
			auto fromAddr = Secret2CfxAddr(transaction->m_secret, m_networkID);
			param1["from"] = fromAddr.GetVerboseBase32Address();
			param1["to"] = transaction->m_cfx_receiveAddress.GetVerboseBase32Address();
		}
		//std::string paramstr = param1.toStyledString();
		auto js = stub_clientPtr->cfx_estimateGasAndCollateral(param1, en);
		EstimateGasAndCollateralResult result;
		bool isok = result.fromJson(js);
		assert(isok && "JsonRpcClient::cfx_estimateGasAndCollateral fromJson Failed");
		return result;
	}

	dev::bytes JsonRpcClient::cfx_call(CallRequest callRequest) {
		Json::Value param1;
		if (callRequest.from.IsValid())
			param1["from"] = callRequest.from.GetVerboseBase32Address();
		if (callRequest.to.IsValid())
			param1["to"] = callRequest.to.GetVerboseBase32Address();
		param1["gas"] = dev::toJS(callRequest.gas);
		param1["gasPrice"] = dev::toJS(callRequest.gasPrice);
		param1["nonce"] = dev::toJS(callRequest.nonce);
		param1["value"] = dev::toJS(callRequest.value);
		std::string  js_data_str = dev::toJS(callRequest.data);
		param1["data"] = js_data_str.empty() ? "0x" : js_data_str; //if data is empty ,set "0x"

		std::string retValue = stub_clientPtr->cfx_call(param1, callRequest.epochNumber);
		if (retValue.size() > 2)
			return cfx_cpp::encoding::hex::DecodeString(retValue.substr(2));
		else
			return dev::bytes();
	}

	void JsonRpcClient::ApplyUnsignedTransactionDefault(TransactionBase* transaction) {
		if (transaction->m_nonce == dev::Invalid256) {
			auto fromAddr = Secret2CfxAddr(transaction->m_secret, m_networkID);
			transaction->m_nonce = cfx_getNextNonce(fromAddr);
		}
		if (transaction->m_chainId == dev::Invalid256) {
			transaction->m_chainId = cfx_getStatus().chainId;
		}
		if (transaction->m_gasPrice == dev::Invalid256) {
			transaction->m_gasPrice = cfx_gasPrice();
			if (transaction->m_gasPrice < MIN_GAS_PRICE)
				transaction->m_gasPrice = MIN_GAS_PRICE;
		}
		if (transaction->m_epochHeight == dev::Invalid256) {
			transaction->m_epochHeight = cfx_epochNumber();
		}

		if (transaction->m_gas == dev::Invalid256 || \
			transaction->m_storageLimit == dev::Invalid256) {

			if (transaction->m_data.empty()) {
				transaction->m_gas = TRANSACTION_GAS;
				transaction->m_storageLimit = TRANSACTION_STORAGE_LIMIT;
			}
			else {
				auto estimate = cfx_estimateGasAndCollateral(transaction);
				if (estimate.storageCollateralized == 0)
					estimate.storageCollateralized = 250;
				auto gas = dev::u256(DecFloat50(estimate.gasUsed) * defaultGasRatio);
				auto storageLimit = dev::u256(DecFloat50(estimate.storageCollateralized) * defaultStorageRatio);

				if (transaction->m_gas == dev::Invalid256) {
					transaction->m_gas = gas;
				}

				if (transaction->m_storageLimit == dev::Invalid256) {
					transaction->m_storageLimit = storageLimit;
				}
			}

		}

#if defined(WIN32)
		std::cout << "ApplyUnsignedTransactionDefault Debug Info:" << std::endl;
		std::cout << "transaction->m_nonce:" << dev::toJS(transaction->m_nonce) << std::endl;
		std::cout << "transaction->m_chainId:" << dev::toJS(transaction->m_chainId) << std::endl;
		std::cout << "transaction->m_gasPrice:" << dev::toJS(transaction->m_gasPrice) << std::endl;
		std::cout << "transaction->m_epochHeight:" << dev::toJS(transaction->m_epochHeight) << std::endl;
		std::cout << "transaction->m_gas:" << dev::toJS(transaction->m_gas) << std::endl;
		std::cout << "transaction->m_storageLimit:" << dev::toJS(transaction->m_storageLimit) << std::endl;
#endif
	}

	TransactionReceipt JsonRpcClient::cfx_getTransactionReceipt(const std::string& transactionHash) {
		auto retvalue = stub_clientPtr->cfx_getTransactionReceipt(transactionHash);
		cfx_cpp::TransactionReceipt Receipt;
		bool  ok = cfx_cpp::Json2Struct(retvalue, Receipt);
		return Receipt;
	}


	LocalisedLogEntries JsonRpcClient::cfx_getLogs(const LogFilterParam& filter) {
		LocalisedLogEntries entries;

		auto retValue = stub_clientPtr->cfx_getLogs(filter.toJS());
		std::string retValueStr = retValue.toStyledString();
		if (!retValue.isArray())
			return entries;

		for (size_t i = 0; i < retValue.size(); i++) {
			LocalisedLogEntry entry;
			bool  isok = LocalisedLogEntry::fromJson(retValue[i], entry);
			if (isok)
				entries.push_back(entry);
		}
		return entries;
	}

	dev::u256 JsonRpcClient::cfx_getBalance(cfx_cpp::Address addr, cfx_cpp::EpochNumber epoch) {
		std::string value = stub_clientPtr->cfx_getBalance(addr.GetVerboseBase32Address(), std::string(epoch));
		return dev::jsToU256(value);
	}

	cfx_getBlockByHashResult JsonRpcClient::cfx_getBlockByHash(const std::string& block_hash) {
		auto js = stub_clientPtr->cfx_getBlockByHash(block_hash, false);
		cfx_getBlockByHashResult retValue;
		retValue.fromJson(js);
		return retValue;
	}
	cfx_getBlockByHashResult JsonRpcClient::cfx_getBlockByEpochNumber(cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getBlockByEpochNumber(std::string(en), false);
		cfx_getBlockByHashResult retValue;
		retValue.fromJson(js);
		return retValue;
	}

	dev::h256 JsonRpcClient::cfx_getBestBlockHash() {
		auto js = stub_clientPtr->cfx_getBestBlockHash();
		return dev::jsToFixed<32>(js);
	}


	dev::h256s JsonRpcClient::cfx_getBlocksByEpoch(cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getBlocksByEpoch(std::string(en));
		dev::h256s retValue;
		if (js.isArray()) {
			for (auto& item : js) {
				dev::h256 h = dev::jsToFixed<32>(item.asString());
				retValue.push_back(h);
			}
		}
		return retValue;
	}
	dev::u256 JsonRpcClient::cfx_getStakingBalance(cfx_cpp::Address address, cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getStakingBalance(address.GetVerboseBase32Address(), std::string(en));
		return dev::jsToU256(js);
	}

	dev::u256 JsonRpcClient::cfx_getCollateralForStorage(cfx_cpp::Address address, cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getCollateralForStorage(address.GetVerboseBase32Address(), std::string(en));
		return dev::jsToU256(js);
	}

	boost::optional<cfx_cpp::Address> JsonRpcClient::cfx_getAdmin(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en) {
		boost::optional<cfx_cpp::Address> retValue;
		try {
			std::string js = stub_clientPtr->cfx_getAdmin(contract_address.GetVerboseBase32Address(), std::string(en));
			if (!js.empty()) {
				retValue = cfx_cpp::Address(cfx_cpp::string_util::tolower_string(js));
				return retValue;
			}
			else
				return retValue;
		}
		catch (const jsonrpc::JsonRpcException& e) {
			return retValue;
		}
	}
	dev::bytes JsonRpcClient::cfx_getCode(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en) {
		dev::bytes	retValue;
		std::string js = stub_clientPtr->cfx_getCode(contract_address.GetVerboseBase32Address(), std::string(en));
		if (js.size() > 2) { // 0x prefix
			retValue = cfx_cpp::encoding::hex::DecodeString(js.substr(2));
		}
		return retValue;
	}

	dev::u256 JsonRpcClient::cfx_getStorageAt(cfx_cpp::Address contract_address, dev::u256 storage_position, cfx_cpp::EpochNumber en) {
		std::string js = stub_clientPtr->cfx_getStorageAt(contract_address.GetVerboseBase32Address(), dev::toJS(storage_position), std::string(en));
		return  dev::jsToU256(js);
	}

	cfx_getStorageRootResult JsonRpcClient::cfx_getStorageRoot(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en) {
		cfx_getStorageRootResult retValue;
		auto js = stub_clientPtr->cfx_getStorageRoot(contract_address.GetVerboseBase32Address(), std::string(en));
		jsMem2String(js, "delta", retValue.delta);
		jsMem2String(js, "intermediate", retValue.intermediate);
		jsMem2String(js, "delta", retValue.snapshot);
		return retValue;
	}

	cfx_getSponsorInfoResult JsonRpcClient::cfx_getSponsorInfo(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getSponsorInfo(contract_address.GetVerboseBase32Address(), std::string(en));

		cfx_getSponsorInfoResult retValue;
		jsMem2U256(js, "sponsorBalanceForCollateral", retValue.sponsorBalanceForCollateral);
		jsMem2U256(js, "sponsorBalanceForGas", retValue.sponsorBalanceForGas);
		jsMem2U256(js, "sponsorGasBound", retValue.sponsorGasBound);

		jsMem2CfxAddress(js, "sponsorForCollateral", retValue.sponsorForCollateral);
		jsMem2CfxAddress(js, "sponsorForGas", retValue.sponsorForGas);
		if (retValue.sponsorForCollateral.GetAddressType().Type() == cfx_cpp::AddressType::AddressTypeNull) {
			retValue.sponsorForCollateral = cfx_cpp::Address("0x0000000000000000000000000000000000000000", m_networkID);
		}
		if (retValue.sponsorForGas.GetAddressType().Type() == cfx_cpp::AddressType::AddressTypeNull) {
			retValue.sponsorForGas = cfx_cpp::Address("0x0000000000000000000000000000000000000000", m_networkID);
		}

		return retValue;
	}

	cfx_getAccountResult JsonRpcClient::cfx_getAccount(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getAccount(contract_address.GetVerboseBase32Address(), std::string(en));
		cfx_getAccountResult retValue;
		jsMem2U256(js, "balance", retValue.balance);
		jsMem2U256(js, "nonce", retValue.nonce);
		jsMem2Hash(js, "codeHash", retValue.codeHash);
		jsMem2U256(js, "collateralForStorage", retValue.collateralForStorage);
		jsMem2U256(js, "accumulatedInterestReturn", retValue.accumulatedInterestReturn);
		jsMem2CfxAddress(js, "admin", retValue.admin);
		return retValue;
	}
	dev::u256 JsonRpcClient::cfx_getInterestRate(cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getInterestRate(std::string(en));
		return dev::jsToU256(js);
	}

	dev::u256 JsonRpcClient::cfx_getAccumulateInterestRate(cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getAccumulateInterestRate(std::string(en));
		return dev::jsToU256(js);
	}

	cfx_checkBalanceAgainstTransactionResult  JsonRpcClient::cfx_checkBalanceAgainstTransaction(
		cfx_cpp::Address account_address, cfx_cpp::Address contract_address,
		dev::u256 gas_limit, dev::u256 gas_price, dev::u256 storage_limit, cfx_cpp::EpochNumber en) {

		auto js = stub_clientPtr->cfx_checkBalanceAgainstTransaction(
			account_address.GetVerboseBase32Address(),
			contract_address.GetVerboseBase32Address(),
			dev::toJS(gas_limit),
			dev::toJS(gas_price),
			dev::toJS(storage_limit),
			std::string(en));

		cfx_checkBalanceAgainstTransactionResult retValue;
		jsMem2Boolean(js, "isBalanceEnough", retValue.isBalanceEnough);
		jsMem2Boolean(js, "willPayCollateral", retValue.willPayCollateral);
		jsMem2Boolean(js, "willPayTxFee", retValue.willPayTxFee);

		return retValue;
	}

	dev::h256s JsonRpcClient::cfx_getSkippedBlocksByEpoch(cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getSkippedBlocksByEpoch(std::string(en));
		dev::h256s retValue;
		if (js.isArray() == false)
			return retValue;
		for (auto& h : js) {
			retValue.push_back(dev::jsToFixed<32>(h.asString()));
		}
		return retValue;
	}

	dev::u256 JsonRpcClient::cfx_getConfirmationRiskByHash(const std::string& block_hash) {
		auto js = stub_clientPtr->cfx_getConfirmationRiskByHash(block_hash);
		//the integer confirmation risk, or null if the block does not exist.
		return dev::jsToU256(js);
	}

	std::vector<cfx_getBlockRewardInfoReslut> JsonRpcClient::cfx_getBlockRewardInfo(cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getBlockRewardInfo(std::string(en));
		std::vector<cfx_getBlockRewardInfoReslut> retValue;
		for (auto& item : js) {
			cfx_getBlockRewardInfoReslut reward;
			jsMem2Hash(item, "blockHash", reward.blockHash);
			jsMem2CfxAddress(item, "author", reward.author);
			jsMem2U256(item, "totalReward", reward.totalReward);
			jsMem2U256(item, "baseReward", reward.baseReward);
			jsMem2U256(item, "txFee", reward.txFee);
			retValue.push_back(reward);
		}
		return retValue;
	}
	cfx_getBlockByHashResult JsonRpcClient::cfx_getBlockByHashWithPivotAssumption(const std::string& block_hash,
		const std::string& assumed_pivot_hash, dev::u256 integer_epoch_number) {
		auto js = stub_clientPtr->cfx_getBlockByHashWithPivotAssumption(block_hash, assumed_pivot_hash, dev::toJS(integer_epoch_number));
		cfx_getBlockByHashResult retValue;
		retValue.fromJson(js);
		return retValue;
	}

	std::vector<cfx_getDepositListResult> JsonRpcClient::cfx_getDepositList(cfx_cpp::Address account_address, cfx_cpp::EpochNumber en) {
		auto js = stub_clientPtr->cfx_getDepositList(account_address.GetVerboseBase32Address(), std::string(en));
		std::vector<cfx_getDepositListResult> retValue;
		for (auto& i : js) {
			cfx_getDepositListResult  deposit;
			jsMem2U256(i, "accumulatedInterestRate", deposit.accumulatedInterestRate);
			jsMem2U256(i, "amount", deposit.amount);
			deposit.depositTime = i["depositTime"].asInt64();
			retValue.push_back(deposit);
		}
		return retValue;
	}

	std::vector<cfx_getVoteListResult> JsonRpcClient::cfx_getVoteList(cfx_cpp::Address account_address, cfx_cpp::EpochNumber en) {
		Json::Value js = stub_clientPtr->cfx_getVoteList(account_address.GetVerboseBase32Address(), std::string(en));
		std::vector<cfx_getVoteListResult> retValue;
		if (js.isArray()) {
			for (size_t i = 0; i < js.size(); i++) {
				cfx_getVoteListResult vote;
				jsMem2U256(js[i], "amount", vote.amount);
				jsMem2U256(js[i], "unlockBlockNumber", vote.unlockBlockNumber);
				retValue.push_back(vote);
			}
		}
		return retValue;
	}

	bool JsonRpcClient::WaitTransactionFinished(dev::h256 transHash, uint32_t max_wait_ms) {
		return WaitTransactionFinished(dev::toJS(transHash) , max_wait_ms);
	}


	bool JsonRpcClient::WaitTransactionFinished(const std::string& transHash, uint32_t max_wait_ms) {
		using namespace std::chrono;
		bool expire = false;
		time_point<high_resolution_clock> begintime = high_resolution_clock::now();

		cfx_cpp::TransactionReceipt res;
		for (;;) {
			try {
				for (;;) {
					res = this->cfx_getTransactionReceipt(transHash);
					if (res.outcomeStatus != 0) {
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));

						int64_t count = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - begintime).count();
						if (count > max_wait_ms) {
							expire = true;
							goto DONE;
						}
						continue;
					}
					else
						goto DONE;
				}
			}
			catch (const std::exception&) {
				int64_t count = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - begintime).count();
				if (count > max_wait_ms) {
					expire = true;
					goto DONE;
				}
			}
		}
	DONE:
		return !expire;
	}




}

