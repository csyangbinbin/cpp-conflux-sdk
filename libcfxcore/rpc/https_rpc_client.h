
// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#ifndef __CPP_CONFLUX_SDK_HTTPS_RPC_CLIENT_INCLUDE__
#define __CPP_CONFLUX_SDK_HTTPS_RPC_CLIENT_INCLUDE__
#include "client.h"
#include <memory>
#if defined(_WIN32)
#include <shlobj.h>
#else
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#endif
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

namespace cfx_cpp {

	class HttpsRpcClient : public JsonRpcClient {
	public:
		HttpsRpcClient(const std::string& remote_addr,
			uint32_t  networkID = NetworkType::NetworkTypeTestnetID,
			const std::string& ca_file_path = "");
		virtual ~HttpsRpcClient();
	public:
		/*
		默认CA文件存放与C:\Users\用户名\AppData\Roaming\cfx_cpp_ca\cacert.pem
		Linux系统位于HOME\.cfx_cpp_ca\cacert.pem
		*/
		static	fs::path GetDefaultCAFile();
	protected:
		std::shared_ptr<jsonrpc::IClientConnector> IClientConnectorPtr;
	};



	std::shared_ptr<JsonRpcClient> NewHttpsRpcClient(const std::string& remote_addr,
		uint32_t  networkID = NetworkType::NetworkTypeTestnetID,
		const std::string& ca_file_path = "");
}

#endif