// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "https_rpc_client.h"
#include "rpc/httpclient.h"
using namespace std;
using namespace dev;

namespace cfx_cpp {

	HttpsRpcClient::HttpsRpcClient(const std::string& remote_addr, uint32_t  networkID, const std::string& ca_file_path) :
		JsonRpcClient(remote_addr, networkID) {

		std::string ca_file = ca_file_path;
		if (ca_file.empty()) 
			ca_file = HttpsRpcClient::GetDefaultCAFile().string();

		IClientConnectorPtr = std::make_shared<jsonrpc::HttpClient>(remote_addr, ca_file.c_str());
		stub_clientPtr = std::make_shared<StubClient>(*IClientConnectorPtr, jsonrpc::JSONRPC_CLIENT_V2);

		assert(IClientConnectorPtr && stub_clientPtr);
	}

	HttpsRpcClient::~HttpsRpcClient() {

	}

	fs::path HttpsRpcClient::GetDefaultCAFile() {
		std::string _prefix = "cfx_cpp_ca";
		fs::path ca_file = fs::path("cacert.pem");
#if defined(_WIN32)
		char path[1024] = "";
		if (SHGetSpecialFolderPathA(NULL, path, CSIDL_APPDATA, true))
			return fs::path(path) / _prefix / ca_file;
		else {
			BOOST_THROW_EXCEPTION(std::runtime_error("getDataDir() - SHGetSpecialFolderPathA() failed."));
		}
#else
		fs::path dataDirPath;
		char const* homeDir = getenv("HOME");
		if (!homeDir || strlen(homeDir) == 0)
		{
			struct passwd* pwd = getpwuid(getuid());
			if (pwd)
				homeDir = pwd->pw_dir;
		}

		if (!homeDir || strlen(homeDir) == 0)
			dataDirPath = fs::path("/");
		else
			dataDirPath = fs::path(homeDir);

		return dataDirPath / ("." + _prefix) / ca_file;
#endif

	}

	std::shared_ptr<JsonRpcClient> NewHttpsRpcClient(const std::string& remote_addr, uint32_t  networkID, const std::string& ca_file_path) {
		std::shared_ptr<JsonRpcClient> client = std::make_shared<HttpsRpcClient>(remote_addr, networkID, ca_file_path);
		return client;
	}
}
