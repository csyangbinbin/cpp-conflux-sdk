// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_CLIENT_INCLUDE__
#define __CPP_CONFLUX_SDK_CLIENT_INCLUDE__
#include <string>
#include <memory>
#include "abi.h"
#include "../currency.h"
#include "rpc/stubclient.h"
#include "TransactionBase.h"
#include "types/transaction_receipt.h"
#include "types/log_filter.h"
#include "rpc_parameters.h"

namespace cfx_cpp {

	enum ClientConnectorType {
		HttpConnector,
		TCPConnector,
		WebSocketConnector
	};

	class JsonRpcClient :
		public std::enable_shared_from_this<JsonRpcClient> {

	public:
		JsonRpcClient(const std::string& remote_addr, uint32_t  networkID = NetworkType::NetworkTypeTestnetID);
		virtual ~JsonRpcClient();

	public:
		//send transaction and return transaction hash 
		std::string sendTransaction(TransactionBase& transaction);

		//send transaction simple
		std::string sendTransaction(const cfx_cpp::Address& to, const dev::u256& value, const dev::Secret& secret);

		//deploy contract
		ContractDeployResult	DeployContract(TransactionBase& transaction);
		ContractDeployResult	DeployContract(const dev::bytes& contractBytes, const dev::bytes& constructor_abi, const dev::Secret& secret);

		uint32_t GetNetworkID() const { return m_networkID; }
		std::string GetRemoteAddr() const { return m_remoteSrever; }
	public:
		//Returns the conflux-rust version.
		std::string	cfx_clientVersion();

		//Returns the node status
		NodeStatus	cfx_getStatus();

		//Returns the next nonce that should be used by the given account when sending a transaction.
		dev::u256	cfx_getNextNonce(cfx_cpp::Address address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the epoch number corresponding to the given tag.
		dev::u256	cfx_epochNumber(cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the current price per gas in Drip.
		dev::u256	cfx_gasPrice();

		//Returns information about a transaction, identified by its hash.
		TransactionByHashResult	cfx_getTransactionByHash(const std::string& transactionHash);

		//Sends a signed transaction into the network for processing.
		std::string cfx_sendRawTransaction(dev::bytes raw);

		//Virtually calls a contract, returns the output data. The transaction will not be added to the blockchain.
		dev::bytes cfx_call(CallRequest callRequest);

		//Returns a transaction receipt, identified by the corresponding transaction hash
		TransactionReceipt cfx_getTransactionReceipt(const std::string& transactionHash);

		//Returns logs matching the filter provided.
		LocalisedLogEntries cfx_getLogs(const LogFilterParam& filter);

		//Returns the balance of the given account, identified by its address
		dev::u256 cfx_getBalance(cfx_cpp::Address addr, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Virtually executes a transaction, returns an estimate for the size of storage collateralized and the gas used by the transaction.
		EstimateGasAndCollateralResult cfx_estimateGasAndCollateral(const TransactionBase* transaction, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns information about a block, identified by its hash.
		cfx_getBlockByHashResult cfx_getBlockByHash(const std::string& block_hash);


		//Returns information about a block, identified by its epoch number.
		cfx_getBlockByHashResult cfx_getBlockByEpochNumber(cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the hash of the best block
		dev::h256 cfx_getBestBlockHash();

		//Returns the block hashes in the specified epoch
		dev::h256s cfx_getBlocksByEpoch(cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the stacking balance of the given account, identified by its address.
		dev::u256 cfx_getStakingBalance(cfx_cpp::Address address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the size of the collateral storage of a given address, in bytes.
		dev::u256 cfx_getCollateralForStorage(cfx_cpp::Address address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the admin of the specified contract.
		boost::optional<cfx_cpp::Address> cfx_getAdmin(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the code of the specified contract.
		dev::bytes cfx_getCode(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns storage entries from a given contract.
		dev::u256 cfx_getStorageAt(cfx_cpp::Address contract_address, dev::u256 storage_position,
			cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the storage root of a given contract.
		cfx_getStorageRootResult cfx_getStorageRoot(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the sponsor info of a given contract.
		cfx_getSponsorInfoResult cfx_getSponsorInfo(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns an account, identified by its address.
		cfx_getAccountResult cfx_getAccount(cfx_cpp::Address contract_address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the interest rate at the given epoch.
		dev::u256 cfx_getInterestRate(cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the accumulate interest rate at the given epoch.
		dev::u256 cfx_getAccumulateInterestRate(cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		/*
		Check if a user's balance is enough to send a transaction with the specified gas and storage limits to the specified contract.
		The balance is enough if the user can cover the up-front payment of both execution and storage,
		or if these costs are sponsored by the contract.
		*/
		cfx_checkBalanceAgainstTransactionResult  cfx_checkBalanceAgainstTransaction(
			cfx_cpp::Address account_address, cfx_cpp::Address contract_address,
			dev::u256 gas_limit, dev::u256 gas_price, dev::u256 storage_limit,
			cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		/*
		Returns the list of non - executed blocks in an epoch.By default,
		Conflux only executes the last 200 blocks in each epoch(note that under normal circumstances, epochs should be much smaller).
		*/
		dev::h256s cfx_getSkippedBlocksByEpoch(cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the confirmation risk of a given block, identified by its hash.
		dev::u256 cfx_getConfirmationRiskByHash(const std::string& block_hash);

		//Returns the reward info for all executed blocks in the specified epoch.
		std::vector<cfx_getBlockRewardInfoReslut> cfx_getBlockRewardInfo(cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the requested block if the provided pivot hash is correct, returns an error otherwise.
		cfx_getBlockByHashResult cfx_getBlockByHashWithPivotAssumption(const std::string& block_hash,
			const std::string& assumed_pivot_hash, dev::u256 integer_epoch_number);

		//Returns the deposit list of the given account, identified by its address.
		std::vector<cfx_getDepositListResult> cfx_getDepositList(cfx_cpp::Address account_address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//Returns the vote list of the given account, identified by its address.
		std::vector<cfx_getVoteListResult> cfx_getVoteList(cfx_cpp::Address account_address, cfx_cpp::EpochNumber en = cfx_cpp::EpochNumber::latest_state);

		//ApplyUnsignedTransactionDefault set empty fields to value fetched from conflux node.
		void ApplyUnsignedTransactionDefault(TransactionBase* transaction);

		//wait an transaction success
		bool WaitTransactionFinished(dev::h256 transHash , uint32_t max_wait_ms = 5000);
		bool WaitTransactionFinished(const std::string& transHash, uint32_t max_wait_ms = 5000);

	protected:
		std::shared_ptr<StubClient>					stub_clientPtr;

	private:
		std::string			m_remoteSrever;
		uint32_t			m_networkID;
		static dev::u256	MIN_GAS_PRICE;
		static dev::u256	TRANSACTION_GAS;
		static dev::u256	TRANSACTION_STORAGE_LIMIT;
		static DecFloat50	defaultGasRatio;
		static DecFloat50	defaultStorageRatio;
	};
}

#endif