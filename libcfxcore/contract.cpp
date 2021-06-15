// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "contract.h"
#include "rpc/client.h"

namespace cfx_cpp {

	Contract::Contract(std::shared_ptr<JsonRpcClient> clientPtr, cfx_cpp::Address contractAddr) :
		client(clientPtr),
		address(contractAddr) {
	}
	Contract::~Contract() {
	}

	dev::bytes Contract::Call(const dev::bytes& args) {
		CallRequest callRequest;
		callRequest.to = address;
		callRequest.data = args;
		return client->cfx_call(callRequest);
	}

	dev::bytes Contract::Call(TW::Ethereum::ABI::Function& func) {
		TW::Data param;
		func.encode(param);
		dev::bytes retBytes = Call(param); //call 

		size_t offset_inout = 0;
		func.decodeOutput(retBytes, offset_inout); //decoed return value 
		return retBytes;
	}

	std::string Contract::SendTransaction(const dev::bytes& args, Secret const& secret, dev::u256 const& value) {
		TransactionBase trans(address, value, secret);
		trans.withData(args);
		return client->sendTransaction(trans);
	}

	std::string Contract::SendTransaction(TW::Ethereum::ABI::Function& func, Secret const& secret, dev::u256 const& value) {
		dev::bytes param;
		func.encode(param);
		return SendTransaction(param, secret, value);
	}

}