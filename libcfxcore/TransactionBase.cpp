// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionBase.h"
#include <libdevcore/SHA3.h>
#include <libdevcore/Exceptions.h>
namespace cfx_cpp {
	dev::h256 TransactionBase::sha3(IncludeSignature _sig) const {
		if (_sig == WithSignature && m_hashWith)
			return m_hashWith;

		RLPStream s;
		streamRLP(s, _sig);

		auto ret = dev::sha3(s.out());
		if (_sig == WithSignature)
			m_hashWith = ret;
		return ret;
	}
	void TransactionBase::streamRLP(RLPStream& _s, IncludeSignature _sig) const {
		if (_sig == WithSignature) {
			_s.appendList(4);
		}
		{
			_s.appendList(9);
			_s << m_nonce << m_gasPrice << m_gas;
			if (m_type == MessageCall)
				_s << m_receiveAddress;
			else
				_s << "";
			_s << m_value;
			_s << m_storageLimit << m_epochHeight << m_chainId;
			if (m_data.empty())
				_s << "";
			else
				_s << m_data;
		}
		if (_sig == WithSignature) {
			_s << (m_vrs->v);
			_s << (u256)m_vrs->r << (u256)m_vrs->s;
		}
	}

	dev::bytes TransactionBase::SignTransactionWithSecret(dev::Secret secret) {
		m_secret = secret;
		return SignTransaction();
	}

	dev::bytes TransactionBase::SignTransaction() {
		if (!m_secret) {
			throw("secret is empty!");
		}
		sign(m_secret);
		dev::RLPStream rlp;
		streamRLP(rlp, WithSignature);
		return  rlp.out();
	}

	void TransactionBase::sign(Secret const& _priv) {
		auto sig = dev::sign(_priv, sha3(WithoutSignature));
		SignatureStruct sigStruct = *(SignatureStruct const*)&sig;
		if (sigStruct.isValid())
			m_vrs = sigStruct;
	}

	cfx_cpp::Address  TransactionBase::cfx_sender(uint32_t  networkID) {
		dev::Address ethAddr = sender();
		return  cfx_cpp::getCfxUserAddress(ethAddr, networkID);
	}

	dev::Address const& TransactionBase::sender() const {
		if (!m_sender.is_initialized()) {
			if (hasZeroSignature())
				m_sender = MaxAddress;
			else {
				if (!m_vrs)
					throw ("TransactionIsUnsigned");
				auto p = recover(*m_vrs, sha3(WithoutSignature));
				if (!p)
					throw("InvalidSignature");
				m_sender = right160(dev::sha3(bytesConstRef(p.data(), sizeof(p))));
			}
		}
		return *m_sender;
	}

}
