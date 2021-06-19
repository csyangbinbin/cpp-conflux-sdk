// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "account.h"
#include <iostream>
#include <vector>
#include <libdevcore/SHA3.h>
using namespace dev;
using namespace dev::eth;

namespace cfx_cpp {

	AccountManager::AccountManager(boost::filesystem::path const& _path, uint32_t _networkID) :
		m_path(_path),
		m_networkID(_networkID) {
		ks = new dev::SecretStore(_path);
		initialize();
	}
	AccountManager::AccountManager(uint32_t _networkID) :
		m_networkID(_networkID) {
		ks = new dev::SecretStore();
		initialize();
	}
	AccountManager::~AccountManager() {
	}

	void AccountManager::initialize() {
		std::vector<h128> keys = ks->keys();
		for (auto const& k : keys) {
			dev::Address  ethHexAddress = ks->address(k);
			cfx_cpp::Address  cfxaddr = getCfxUserAddress(ethHexAddress, m_networkID);
			cfxAddressDic[cfxaddr] = k;
		}
	}
	cfx_cpp::Address AccountManager::Create(const std::string& passphrase) {
		std::lock_guard<std::mutex> lock(mtx);
		bool icap = true;
		KeyPair k(Secret::random());
		k = KeyPair(Secret(dev::sha3(k.secret().ref())));
		auto uuid = ks->importSecret(k.secret().asBytesSec(), passphrase);
		cfx_cpp::Address cfxaddr = getCfxUserAddress(k.address(), m_networkID);
		cfxAddressDic[cfxaddr] = uuid;
		return cfxaddr;
	}
	std::vector<cfx_cpp::Address> AccountManager::List() const {
		std::lock_guard<std::mutex> lock(mtx);
		std::vector<cfx_cpp::Address> value;
		for (auto const& v : cfxAddressDic) {
			value.push_back(v.first);
		}
		return value;
	}
	cfx_cpp::Address AccountManager::ImportKeystore(boost::filesystem::path keystore_file_path, bool* ok) {
		if (!boost::filesystem::is_regular_file(keystore_file_path))
			return cfx_cpp::Address();
		return ImportKeystore(keystore_file_path.string(), ok);
	}

	cfx_cpp::Address AccountManager::ImportKeystore(const std::string& keystore_file, bool* ok) {
		std::lock_guard<std::mutex> lock(mtx);
		auto uuid = ks->importKey(keystore_file);
		if (ok)
			*ok = uuid ? true : false;
		if (uuid) {
			auto cfxaddr = getCfxUserAddress(ks->address(uuid), m_networkID);
			cfxAddressDic[cfxaddr] = uuid;
			return  cfxaddr;
		}
		else {
			return cfx_cpp::Address();
		}
	}
	dev::Secret AccountManager::GetSecret(cfx_cpp::Address addr, const std::string& passphrase) {
		std::lock_guard<std::mutex> lock(mtx);
		dev::Secret secret;
		auto iter = cfxAddressDic.find(addr);
		if (iter == cfxAddressDic.end())
			return secret;

		bytesSec  secbytes = ks->secret(iter->second, [&]() { return passphrase; });
		return dev::Secret(secbytes);
	}

	cfx_cpp::Address AccountManager::ImportPrivateKey(dev::Secret secret_key, const std::string& passphrase) {
		std::lock_guard<std::mutex> lock(mtx);
		cfx_cpp::Address addr = Secret2CfxAddr(secret_key ,m_networkID);
		auto iter = cfxAddressDic.find(addr);
		if (iter != cfxAddressDic.end())
			return addr;
		h128 uuid = ks->importSecret(secret_key.asBytesSec(), passphrase);
		cfx_cpp::Address cfxaddr = Secret2CfxAddr(secret_key, m_networkID);
		cfxAddressDic[cfxaddr] = uuid;
		return cfxaddr;
	}

	bool AccountManager::UpdatePassphrase(cfx_cpp::Address addr, const std::string& passphrase, const std::string& newPassphrase) {
		std::lock_guard<std::mutex> lock(mtx);
		auto iter = cfxAddressDic.find(addr);
		if (iter == cfxAddressDic.end())
			return false;
		auto uuid = iter->second;
		return ks->recode(uuid, newPassphrase, [&]() { return passphrase; });
	}

	bool AccountManager::Delete(cfx_cpp::Address addr, const std::string& passphrase) {
		std::lock_guard<std::mutex> lock(mtx);
		auto iter = cfxAddressDic.find(addr);
		if (iter == cfxAddressDic.end())
			return false;
		dev::Secret secret(ks->secret(iter->second, [passphrase]() { return passphrase; },false));
		if (!secret)
			return false;
		ks->kill(iter->second);
		cfxAddressDic.erase(addr);
		return true;
	}
	cfx_cpp::Address AccountManager::GetDefault() const {
		{
			//MUTEX
			std::lock_guard<std::mutex> lock(mtx);
			if (cfxAddressDic.empty())
				return cfx_cpp::Address();
		}//MUTEX

		return List()[0];
	}
	//----------------
	cfx_cpp::Address Secret2CfxAddr(dev::Secret key, uint32_t  networkID) {
		dev::Address addr = dev::toAddress(dev::toPublic(key));
		addr[0] = (addr[0] & 0x0F) | 0x10;
		return  cfx_cpp::Address(("0x" + addr.hex()), networkID);
	}

	cfx_cpp::Address getCfxUserAddress(dev::Address addr, uint32_t  networkID) {
		addr[0] = (addr[0] & 0x0F) | 0x10;
		return  cfx_cpp::Address(("0x" + addr.hex()), networkID);
	}

}

