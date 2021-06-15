// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_CONTRACT_INCLUDE__
#define __CPP_CONFLUX_SDK_CONTRACT_INCLUDE__
#include <memory>
#include <libdevcore/Common.h>
#include <libdevcrypto/Common.h>
#include "rpc/stubclient.h"
#include "abi.h"
#include "types/cfxaddress/address.h"
#include "types/epoch_number.h"

namespace cfx_cpp {
	class JsonRpcClient;

	class Contract {
	public:
		Contract(std::shared_ptr<JsonRpcClient> clientPtr, cfx_cpp::Address contractAddr);
		virtual ~Contract();
	public:
		dev::bytes Call(const dev::bytes& args);
		dev::bytes Call(TW::Ethereum::ABI::Function& func);

		std::string SendTransaction(const dev::bytes& args, dev::Secret const& secret, dev::u256 const& value = 0);
		std::string SendTransaction(TW::Ethereum::ABI::Function& func, dev::Secret const& secret, dev::u256 const& value = 0);

		cfx_cpp::Address GetContractAddr() const { return address;  }
		std::shared_ptr<JsonRpcClient> GetConnector() const { return client;  }

	private:
		std::shared_ptr<JsonRpcClient>	client;
		cfx_cpp::Address address;
	};
}

#endif