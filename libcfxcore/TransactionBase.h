// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __TRANSACTION_BASE_INCLUDE__
#define __TRANSACTION_BASE_INCLUDE__

#include <iostream>
#include <libdevcore/RLP.h>
#include <libdevcore/Address.h>
#include <libdevcrypto/Common.h>
#include <boost/optional.hpp>
#include "types/cfxaddress/address.h"	
#include "account.h"

using namespace dev; 


namespace cfx_cpp {

	enum IncludeSignature
	{
		WithoutSignature = 0,
		WithSignature = 1,
	};

	class TransactionBase {
	public:
		TransactionBase() :
			m_nonce(dev::Invalid256),
			m_chainId(dev::Invalid256),
			m_gasPrice(dev::Invalid256),
			m_epochHeight(dev::Invalid256),
			m_gas(dev::Invalid256),
			m_storageLimit(dev::Invalid256),
			m_type(NullTransaction) {}


		//签名的消息调用交易构造
		TransactionBase(cfx_cpp::Address const& _dest, u256 const& _value, Secret const& _secret) :
			m_nonce(dev::Invalid256),
			m_chainId(dev::Invalid256),
			m_gasPrice(dev::Invalid256),
			m_epochHeight(dev::Invalid256),
			m_gas(dev::Invalid256),
			m_storageLimit(dev::Invalid256),
			m_type(MessageCall),
			m_value(_value),
			m_receiveAddress(_dest.toEthCommonAddr()),
			m_cfx_receiveAddress(_dest),
			m_secret(_secret) {
		}
		TransactionBase(cfx_cpp::Address const& _dest, u256 const& _value) :TransactionBase(_dest , _value , dev::Secret()) {}
		
		//签名的合约创建交易构造
		TransactionBase(dev::bytes const& _data, Secret const& _secret) :
			m_nonce(dev::Invalid256),
			m_chainId(dev::Invalid256),
			m_gasPrice(dev::Invalid256),
			m_epochHeight(dev::Invalid256),
			m_gas(dev::Invalid256),
			m_storageLimit(dev::Invalid256),
			m_type(ContractCreation),
			m_value(0),
			m_data(_data),
			m_secret(_secret) {}

		TransactionBase(dev::bytes const& _data) :TransactionBase(_data, dev::Secret()) {}
	public:
		cfx_cpp::Address receive_cfx() const { return m_cfx_receiveAddress; }
		TransactionBase& withReceive(const cfx_cpp::Address& addr) { m_cfx_receiveAddress = addr; return *this; }

		cfx_cpp::Address to_cfx() const { return m_cfx_receiveAddress; }
		TransactionBase& withTo(const cfx_cpp::Address& addr) { m_cfx_receiveAddress = addr; return *this; }

		dev::Address receive() const { return m_receiveAddress; }
		dev::Address to() const { return m_receiveAddress; }

		dev::u256 nonce() const { return m_nonce; }
		TransactionBase& withNonce(dev::u256 value) { m_nonce = value; return *this; }

		dev::u256 chainId() const { return m_chainId; }
		TransactionBase& withChainId(dev::u256 value) { m_chainId = value; return *this; }

		dev::u256 gasPrice() const { return m_gasPrice; }
		TransactionBase& withGasPrice(dev::u256 value) { m_gasPrice = value; return *this; }

		dev::u256 epochHeight() const { return m_epochHeight; }
		TransactionBase& withEpochHeight(dev::u256 value) { m_epochHeight = value; return *this; }

		dev::u256 gas() const { return m_gas; }
		TransactionBase& withGas(dev::u256 value) { m_gas = value; return *this; }

		dev::u256 storageLimit() const { return m_storageLimit; }
		TransactionBase& withStorageLimit(dev::u256 value) { m_storageLimit = value; return *this; }

		bool isCreation() const { return m_type == ContractCreation; }

		dev::u256 value() const { return m_value; }
		TransactionBase& withValue(dev::u256 value) { m_value = value; return *this; }

		const bytes& data() const { return m_data; }
		TransactionBase& withData(const bytes& data) { m_data = data; return *this; }

		dev::Secret  secret() const { return m_secret; }
		TransactionBase& withSecret(dev::Secret value) { m_secret = value; return *this; }

	public:
		dev::bytes SignTransaction();
		dev::bytes SignTransactionWithSecret(dev::Secret secret);

		bool hasZeroSignature() const { return m_vrs && isZeroSignature(m_vrs->r, m_vrs->s); }

		void streamRLP(dev::RLPStream& _s, IncludeSignature _sig) const;
		dev::h256 sha3(IncludeSignature _sig = WithSignature) const;
		void sign(dev::Secret const& _priv);
		dev::Address const& sender() const;
		cfx_cpp::Address cfx_sender(uint32_t  networkID);

	public:
		static bool isZeroSignature(dev::u256 const& _r, dev::u256 const& _s) { return !_r && !_s; }

		enum Type {
			NullTransaction,//空交易		
			ContractCreation,//合约创建,	receiveAddress被忽略	
			MessageCall	//消息调用,	receiveAddress被使用	
		};
	public:
		dev::Address m_from;
		dev::u256 m_storageLimit;						///< the storage limit specified by the sender
		dev::u256 m_epochHeight;
		dev::u256 m_chainId;
		boost::optional<dev::SignatureStruct> m_vrs;	///< The signature of the transaction. Encodes the sender.
		dev::u256 m_nonce;								///< The transaction-count of the sender.
		dev::u256 m_value;								///< The amount of ETH to be transferred by this transaction. Called 'endowment' for contract-creation transactions.
		dev::Address m_receiveAddress;					///< The receiving address of the transaction.  
		cfx_cpp::Address m_cfx_receiveAddress;
		dev::u256 m_gasPrice;							///< The base fee and thus the implied exchange rate of ETH to GAS.
		dev::u256 m_gas;								///< The total gas to convert, paid for from sender's account. Any unused gas gets refunded once the contract is ended.
		dev::bytes m_data;								///< The data associated with the transaction, or the initialiser if it's a creation transaction.
		mutable boost::optional<dev::Address>		m_sender;  ///< 通过签名计算出的sender缓存
		mutable dev::h256 m_hashWith;					///< 带签名的交易hash缓存
		Type m_type;									///<消息调用或者合约创建
		Secret  m_secret;
	};

	//std::ostream& operator<<(std::ostream& _out, TransactionBase const& _t);
	//TransactionBase TransactionBuilder(cfx_cpp::Address const& _dest, u256 const& _value);
	//TransactionBase TransactionBuilder(cfx_cpp::Address const& _dest, u256 const& _value, Secret const& _secret);	
}


#endif