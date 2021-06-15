// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "epoch_number.h"
#include "libdevcore/CommonJS.h"
#include "common/util/string_util.h"
namespace cfx_cpp {
	const  std::string	 EpochNumber::earliest = "earliest";
	const  std::string	 EpochNumber::latest_checkpoint = "latest_checkpoint";
	const  std::string	 EpochNumber::latest_confirmed = "latest_confirmed";
	const  std::string	 EpochNumber::latest_state = "latest_state";
	const  std::string	 EpochNumber::latest_mined = "latest_mined";



	EpochNumber::EpochNumber(const std::string& sn) :epoch_number_js_str(sn) {

	}
	EpochNumber::EpochNumber(dev::u256 n) {
		epoch_number_js_str = dev::toJS(n);
	}

	std::ostream& operator<<(std::ostream& os, const EpochNumber& en) {
		os << en.epoch_number_js_str; 
		return os;
	}
}

