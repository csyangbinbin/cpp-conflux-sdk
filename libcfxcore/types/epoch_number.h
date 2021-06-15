// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_EPOCH_NUMBER_TYPE_INCLUDE__
#define __CPP_CONFLUX_SDK_EPOCH_NUMBER_TYPE_INCLUDE__
#include <libdevcore/Common.h>
#include <iostream>
#include <string.h>
/*
HEX String - an integer epoch number. For example, 0x3e8 is epoch 1000.
String "earliest" for the epoch of the genesis block.
String "latest_checkpoint" for the earliest epoch stored in memory.
String "latest_confirmed" - for the latest confirmed epoch (using the confirmation meter's estimate).
String "latest_state" - for the latest epoch that has been executed.
String "latest_mined" - for the latest known epoch.
*/

namespace cfx_cpp {
	class EpochNumber {
	public:
		friend std::ostream& operator<<(std::ostream& os, const EpochNumber& en); 
		EpochNumber(const std::string& sn = "latest_state");
		EpochNumber(dev::u256 n);
		operator std::string() const { return epoch_number_js_str;  }

	public:
		static EpochNumber Earliest() { return EpochNumber("earliest");  }
		static EpochNumber LatestCheckpoint() { return EpochNumber("latest_checkpoint"); }
		static EpochNumber LatestConfirmed() { return EpochNumber("latest_confirmed"); }
		static EpochNumber LatestState() { return EpochNumber("latest_state"); }
		static EpochNumber LatestMined() { return EpochNumber("latest_mined"); }

	public:
		const static std::string	 earliest;
		const static std::string	 latest_checkpoint;
		const static std::string	 latest_confirmed;
		const static std::string	 latest_state;
		const static std::string	 latest_mined;
	private:
		std::string	epoch_number_js_str;
	};
	std::ostream& operator<<(std::ostream& os, const EpochNumber& en);
}
#endif