// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_TRANSACTION_RECEIPT_TYPE_INCLUDE__
#define __CPP_CONFLUX_SDK_TRANSACTION_RECEIPT_TYPE_INCLUDE__
#include <stdint.h>
#include <libdevcore/Common.h>
#include <boost/optional.hpp>
#include "cfxaddress/address.h"
#include "log_enrty.h"
#include <json/json.h>


namespace cfx_cpp {

	struct StorageChange {
		cfx_cpp::Address	address; 		
		dev::u256			collaterals;/// Number of storage collateral units to deposit / refund (absolute value).
	};

	class TransactionReceipt {
	public:
		TransactionReceipt() = default;

	public: 
		dev::h256	transactionHash; 
		uint64_t	index; 
		dev::h256	blockHash; 
		dev::u256	epochNumber; 
		cfx_cpp::Address	from;
		boost::optional<cfx_cpp::Address>	to; //null when it is a contract deployment transaction.
		dev::u256	gasUsed;
		dev::u256	gasFee;
		boost::optional<cfx_cpp::Address>	contractCreated; //null when it is not a contract deployment transaction
		LogEntries	logs;
		LogBloom	logsBloom;
		dev::h256	stateRoot;  //0 if the state root is not available.
		uint64_t	outcomeStatus; //the outcome status code. 0x0 means success
		boost::optional<std::string>	txExecErrorMsg; 
		bool	gasCoveredBySponsor;  // Whether gas costs were covered by the sponsor.
		bool	storageCoveredBySponsor;// Whether storage costs were covered by the sponsor.
		dev::u256	storageCollateralized; // The amount of storage collateralized by the sender.
		std::vector<StorageChange>	storageReleased; // Storage collaterals released during the execution of the transaction.
	};
	bool  Json2Struct(const Json::Value& js, TransactionReceipt& retValue);
	std::ostream& operator<<(std::ostream& _out, TransactionReceipt const& _r);
}



#endif
