// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_RPC_PARAMETERS_INCLUDE__
#define __CPP_CONFLUX_SDK_RPC_PARAMETERS_INCLUDE__
#include <stdint.h>
#include <boost/optional.hpp>
#include <libdevcore/FixedHash.h>
#include <libdevcore/Common.h>
#include <libdevcrypto/Common.h>
#include <libcfxcore/types/epoch_number.h>
#include <libcfxcore/types/cfxaddress/address.h>
#include <json/json.h>


namespace cfx_cpp {

	class cfx_getBlockByHashResult {
	public:
		cfx_getBlockByHashResult() =default;
		bool fromJson(const Json::Value& js);
	public:
		bool  adaptive; //  Boolean - true if the weight of the block is adaptive under the GHAST rule.
		uint64_t blame;// QUANTITY - if 0, then this block does not blame any blocks on its parent path. If it is n > 0, then this block blames its n predecessors on its parent path, e.g. when n = 1, then the block blames its parent but not its parent's parent.
		dev::h256 deferredLogsBloomHash;// DATA, 32 Bytes - the hash of the logs bloom after deferred execution at the block's epoch (assuming it is the pivot block).
		dev::h256  deferredReceiptsRoot;// DATA, 32 Bytes - the Merkle root of the receipts after deferred execution at the block's epoch (assuming it is the pivot block).
		dev::h256  deferredStateRoot;// DATA, 32 Bytes - the hash of the state trie root triplet after deferred execution at the block's epoch (assuming it is the pivot block).
		dev::u256 difficulty;//  QUANTITY - the PoW difficulty of this block.
		boost::optional<cfx_cpp::EpochNumber>	epochNumber;// QUANTITY - the number of the epoch containing this block in the node's view of the ledger. null when the epoch number is not determined (e.g. the block is not in the best block's past set).
		dev::u256 gasLimit; // QUANTITY - the maximum gas allowed in this block.
		boost::optional<dev::u256> gasUsed;// QUANTITY - the total gas used in this block.null when the block is pending.
		dev::h256 hash;// DATA, 32 Bytes - hash of the block.
		dev::u256 height;// QUANTITY - the height of the block.
		cfx_cpp::Address miner;// BASE32 - the address of the beneficiary to whom the mining rewards were given.
		dev::u256 nonce;// DATA, 8 Bytes - hash of the generated proof - of - work.
		dev::h256 parentHash;// DATA, 32 Bytes - hash of the parent block.
		boost::optional<dev::u256>	 powQuality; // DATA - the PoW quality.null when the block is pending.
		dev::h256s	refereeHashes; // Array - array of referee block hashes.
		uint64_t size;// QUANTITY - the size of this block in bytes, excluding the block header.
		uint64_t timestamp;// QUANTITY - the unix timestamp for when the block was created.
		dev::h256s transactions;// Array - array of transaction objects, or 32 - byte transaction hashes, depending on the second parameter.
		dev::h256 transactionsRoot;// DATA, 32 Bytes - the Merkle root of the transactions in this block.
	};



	//cfx_getStatus return value
	class  NodeStatus {
	public:
		NodeStatus() = default;
		bool fromJson(const Json::Value& js);
	public:
		dev::h256 bestHash;//DATA - hash of the latest epoch's pivot block
		uint32_t chainId;//QUANTITY - chainId
		uint32_t networkId;//QUANTITY - networkId
		dev::u256 blockNumber;//QUANTITY - total block number
		cfx_cpp::EpochNumber epochNumber;//QUANTITY - latest epoch number
		dev::u256 latestCheckpoint; //QUANTITY - latest checkpoint epoch number
		dev::u256 latestConfirmed;//QUANTITY - latest confirmed epoch number
		dev::u256 latestState;//QUANTITY - latest state epoch number
		dev::u256 pendingTxNumber;//QUANTITY - current pending transaction count
	};

	//cfx_estimateGasAndCollateral return value 
	class EstimateGasAndCollateralResult {
	public:
		EstimateGasAndCollateralResult() = default;
	public:
		bool fromJson(const Json::Value& js);
	public:
		dev::u256	gasLimit;////QUANTITY - gas limit during execution.
		dev::u256	gasUsed;//QUANTITY - gas used during execution.
		dev::u256	storageCollateralized;//QUANTITY - size of storage collateralized, in bytes.
	};

	enum TransactionStatus {
		Success = 0,
		ErrorOccurred = 1,
		SkippedOrNotPacked = 2
	};

	//cfx_getTransactionByHash return value
	class TransactionByHashResult {
	public:
		TransactionByHashResult() :
			transactionIndex(dev::Invalid256) {}
		TransactionStatus GetStatus() const { return status; }
		bool IsOK() const { return status == TransactionStatus::Success; }
		bool IsContractCreated() const { return contractCreated.GetAddressType().Type() == cfx_cpp::AddressType::AddressTypeContract; }
		bool fromJson(const Json::Value& js);
	public:
		dev::h256 hash;//DATA, 32 Bytes - hash of the transaction.
		dev::u256 nonce;//QUANTITY - the number of transactions made by the sender prior to this one.
		dev::h256 blockHash; //DATA, 32 Bytes - hash of the block where this transaction was in and got executed. null when the transaction is pending.
		dev::u256 transactionIndex; //null
		cfx_cpp::Address from;//BASE32 - address of the sender.
		cfx_cpp::Address to; //BASE32 - address of the receiver. null when it is a contract deployment transaction.
		dev::u256 value;//QUANTITY - value transferred in Drip.
		dev::u256 gasPrice;//QUANTITY - gas price provided by the sender in Drip.
		dev::u256 gas;//QUANTITY - gas provided by the sender.
		cfx_cpp::Address contractCreated; //BASE32 - address of the contract created. null when it is not a contract deployment transaction.
		dev::bytes data;//DATA - the data sent along with the transaction.
		dev::u256 storageLimit;//QUANTITY - the storage limit specified by the sender.
		dev::u256 epochHeight;//QUANTITY - the epoch proposed by the sender. Note that this is NOT the epoch of the block containing this transaction.
		uint32_t chainId;//QUANTITY - the chain ID specified by the sender.
		TransactionStatus status;//QUANTITY - 0 for success, 1 if an error occurred, null when the transaction is skipped or not packed.
		dev::SignatureStruct signature;// ECDSA signature
	};

	//cfx_call参数
	class CallRequest {
	public:
		CallRequest() :
			gasPrice(1),
			gas(500000000),
			value(0),
			nonce(0),
			epochNumber(EpochNumber::latest_state) {}
	public:
		cfx_cpp::Address from;//BASE32 - (optional, default: random address) address of sender.
		cfx_cpp::Address to;//BASE32 - (optional, default: null for contract creation) address of receiver.
		dev::u256 value;//QUANTITY - (optional, default: 0) value transferred in Drip.
		dev::bytes data;//DATA - (optional, default: 0x) the data send along with the transaction.
		dev::u256 nonce;//QUANTITY - (optional, default: 0) the number of transactions made by the sender prior to this one.
		dev::u256 gas;//QUANTITY - (optional, default: 500000000) gas provided by the sender.
		dev::u256 gasPrice;//QUANTITY - (optional, default: 0) gas price provided by the sender in Drip.
		cfx_cpp::EpochNumber epochNumber;//(optional, default: "latest_state") integer epoch number, or the string "latest_state", "latest_confirmed", "latest_checkpoint" or "earliest",
	};

	class cfx_getStorageRootResult {
	public:
		cfx_getStorageRootResult() = default; 
	public:
		boost::optional<std::string>	delta;//DATA, 32 Bytes - the storage root in the delta trie, or the string "TOMBSTONE", or null.
		boost::optional<std::string>	intermediate;//DATA, 32 Bytes - storage root in the intermediate trie, or the string "TOMBSTONE", or null.
		boost::optional<std::string>	snapshot;//DATA, 32 Bytes - storage root in the snapshot, or the string "TOMBSTONE", or null.

	};

	/*
	 A sponsor info object.
	 If the contract doesn't have a sponsor, then all fields in the object returned will be 0
	*/
	class cfx_getSponsorInfoResult {
	public:
		cfx_getSponsorInfoResult() = default; 
	public:
		dev::u256	sponsorBalanceForCollateral;//QUANTITY - the sponsored balance for storage.
		dev::u256	sponsorBalanceForGas;//QUANTITY - the sponsored balance for gas.
		dev::u256	sponsorGasBound;//QUANTITY - the max gas that could be sponsored for one transaction.
		cfx_cpp::Address	sponsorForCollateral;//BASE32 - the address of the storage sponsor.
		cfx_cpp::Address	sponsorForGas;//BASE32 - the address of the gas sponsor.
	};

	/*
	Returns an account, identified by its address.
	*/
	class cfx_getAccountResult {
	public:
		cfx_getAccountResult() = default;
	public:
		dev::u256	balance;//QUANTITY - the balance of the account.
		dev::u256	nonce;//QUANTITY - the nonce of the account's next transaction.
		dev::h256	codeHash;//QUANTITY - the code hash of the account.
		dev::u256	stakingBalance;//QUANTITY - the staking balance of the account.
		dev::u256	collateralForStorage;//QUANTITY - the collateral storage of the account.
		dev::u256	accumulatedInterestReturn;//QUANTITY - accumulated interest return of the account.
		cfx_cpp::Address	admin;//BASE32 - admin of the account.
	};

	class cfx_checkBalanceAgainstTransactionResult {
	public:
		cfx_checkBalanceAgainstTransactionResult() = default;
	public:
		bool	isBalanceEnough;//Boolean - indicate balance is enough
		bool	willPayCollateral;//Boolean - false if the transaction is eligible for storage collateral sponsorship, true otherwise.
		bool	willPayTxFee;//Boolean - false if the transaction is eligible for gas sponsorship, true otherwise.
	};

	class cfx_getBlockRewardInfoReslut {
	public:
		cfx_getBlockRewardInfoReslut() = default; 
	public:
		dev::h256	blockHash;//DATA - the block hash
		cfx_cpp::Address	author;//BASE32 - the address of block miner
		dev::u256	totalReward;//QUANTITY - total reward of the block including base reward, tx fee, staking reward
		dev::u256	baseReward; //QUANTITY - base reward
		dev::u256	txFee;//QUANTITY - tx fee
	};

	class cfx_getDepositListResult {
	public:
		cfx_getDepositListResult() = default; 
	public:
		dev::u256	accumulatedInterestRate;//QUANTITY - the accumulated interest rate at the time of the deposit.
		dev::u256	amount;//QUANTITY - the number of tokens deposited.
		uint64_t	depositTime;//QUANTITY - the time of the deposit.
	};

	class cfx_getVoteListResult {
	public:
		cfx_getVoteListResult() = default;
	public:
		dev::u256	amount;//QUANTITY - the number of tokens locked.
		dev::u256	unlockBlockNumber;// QUANTITY - the block number at which the locked tokens are released.
	};

	class Contract;
	//合约部署调用返回结构
	class ContractDeployResult {
	public:
		ContractDeployResult() :
			status(TransactionStatus::ErrorOccurred) {}
	public:
		TransactionStatus status;
		dev::h256 transactionHash;
		cfx_cpp::Address contractCreatedAddr;
		std::shared_ptr<Contract> deployedContract;
	};
}
#endif
