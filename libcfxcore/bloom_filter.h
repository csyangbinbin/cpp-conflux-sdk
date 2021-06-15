// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_BLOOM_FILTER_INCLUDE__
#define __CPP_CONFLUX_SDK_BLOOM_FILTER_INCLUDE__
#include <libdevcore/Address.h>
#include <libdevcore/FixedHash.h>
#include <libcfxcore/types/cfxaddress/address.h>
#include <libdevcore/SHA3.h>
namespace cfx_cpp
{
	using LogBloom = dev::h2048;
	using LogBlooms = std::vector<LogBloom>;

	class BloomFilter {
	public:
		BloomFilter() {}
		BloomFilter(const LogBloom& v) :bloom(v) {}
		virtual ~BloomFilter() {}
	public:
		inline void Add(const cfx_cpp::Address& addr) {
			bloom.shiftBloom<3>(dev::sha3(addr.toEthCommonAddr()));
		}
		template<unsigned M> void Add(const dev::FixedHash<M>& hash) {
			bloom.shiftBloom<3>(dev::sha3(hash));
		}

		inline void Add(const std::string& str) {
			bloom.shiftBloom<3>(dev::sha3(str));
		}

		inline bool Contains(const cfx_cpp::Address& addr) {
			return bloom.containsBloom<3>(dev::sha3(addr.toEthCommonAddr()));
		}
	
		template<unsigned M> bool Contains(const dev::FixedHash<M>& hash) {
			return bloom.containsBloom<3>(dev::sha3(hash));
		}

		inline bool Contains(const std::string& str) {
			return bloom.containsBloom<3>(dev::sha3(str));
		}

		inline bool Contains(const LogBloom& v) { return bloom.contains(v);  }
		inline bool Contains(const BloomFilter& v) { return bloom.contains(v.Value()); }

		inline const LogBloom& Value() const { return bloom;  }
		inline void SetValue(const LogBloom& v) { bloom = v;  }
	private:
		LogBloom bloom; 
	};

}
#endif