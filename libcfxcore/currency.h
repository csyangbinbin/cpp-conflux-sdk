// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_CURRENCY_INCLUDE__
#define __CPP_CONFLUX_SDK_CURRENCY_INCLUDE__
#include <string>
#include <libdevcore/Common.h>
#include <boost/multiprecision/cpp_dec_float.hpp>
namespace cfx_cpp {

	/*
	* To incentivize the maintenance of the Conflux network and charge users for consumption of resources,
	Conflux has a native currency called the Conflux coin, simply denoted by CFX for short.
	The smallest subdenomination is denoted by Drip, in which all values processed in Conflux are integers.
	One Conflux is defined as 10^18 Drip. Frequently used subdenominations of Conflux are listed below:

	Multiplier(in Drip)	        Name
	10^0	                    Drip
	10^9	                    GDrip
	10^12	                    uCFX
	10^18	                    Conflux(CFX)
	*
	*/
	using DecFloat50 = boost::multiprecision::cpp_dec_float_50;

	inline  dev::u256 Drip(dev::u256 value) {
		return dev::u256(value);
	}
	inline  dev::u256 Drip(const std::string& value) {
		return dev::u256(value);
	}

	inline   dev::u256 GDrip(DecFloat50 value) {
		DecFloat50 ratio_(1e9);
		return dev::u256(ratio_ * value);
	}

	inline   dev::u256 GDrip(const std::string& value) {
		return GDrip(DecFloat50(value));
	}

	inline   dev::u256 uCFX(boost::multiprecision::cpp_dec_float_50 value) {
		DecFloat50 ratio_(1e12);
		return dev::u256(ratio_ * value);
	}

	inline   dev::u256 uCFX(const std::string& value) {
		return uCFX(DecFloat50(value));
	}

	inline   dev::u256 CFX(DecFloat50 value) {
		DecFloat50 ratio_(1e18);
		return dev::u256(ratio_ * value);
	}

	inline   dev::u256 CFX(const std::string& value) {
		return CFX(DecFloat50(value));
	}

	inline double Drip2CFXDouble(dev::u256 value) {
		dev::u256  ratio_cfx2drip = dev::exp10<18>();
		cfx_cpp::DecFloat50 cfx = cfx_cpp::DecFloat50(value) / cfx_cpp::DecFloat50(ratio_cfx2drip);
		return cfx.convert_to<double>(); 
	}

}
dev::u256 operator"" _cfx(const char* v);
dev::u256 operator"" _ucfx(const char* v);
dev::u256 operator"" _gdrip(const char* v);
dev::u256 operator"" _drip(const char* v);
#endif