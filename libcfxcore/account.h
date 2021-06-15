// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_ACCOUNT_INCLUDE__
#define __CPP_CONFLUX_SDK_ACCOUNT_INCLUDE__
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <libdevcrypto/SecretStore.h>
#include <libdevcore/KeyManager.h>
#include "types/cfxaddress/address.h"
namespace cfx_cpp {

	cfx_cpp::Address Secret2CfxAddr(dev::Secret key, uint32_t  networkID);
	cfx_cpp::Address getCfxUserAddress(dev::Address addr, uint32_t  networkID);

	class AccountManager {
	public:
		AccountManager(uint32_t _networkID);
		AccountManager(boost::filesystem::path const& _path, uint32_t _networkID);
		virtual ~AccountManager();

	public:
		cfx_cpp::Address Create(const std::string& passphrase);
		std::vector<cfx_cpp::Address> List() const;
		cfx_cpp::Address ImportKeystore(const std::string& keystore_file, bool* ok = NULL);
		cfx_cpp::Address ImportKeystore(boost::filesystem::path keystore_file_path, bool* ok = NULL);
		cfx_cpp::Address ImportPrivateKey(dev::Secret secret_key, const std::string& passphrase);
		bool UpdatePassphrase(cfx_cpp::Address addr, const std::string& passphrase, const std::string& newPassphrase);
		dev::Secret GetSecret(cfx_cpp::Address addr, const std::string& passphrase);
		bool Delete(cfx_cpp::Address addr, const std::string& passphrase);
		cfx_cpp::Address GetDefault() const;

	private:
		void initialize();
	private:
		boost::filesystem::path m_path;
		uint32_t m_networkID;
		dev::SecretStore* ks;
		std::map<cfx_cpp::Address, dev::h128> cfxAddressDic;
		mutable std::mutex	mtx;
	};

}
#endif