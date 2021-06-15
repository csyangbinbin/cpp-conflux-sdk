// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "currency.h"
namespace cfx_cpp {
}
dev::u256 operator"" _cfx(const char* v) {
	return cfx_cpp::CFX(std::string(v));
}
dev::u256 operator"" _ucfx(const char* v) {
	return cfx_cpp::uCFX(std::string(v));
}
dev::u256 operator"" _gdrip(const char* v) {
	return cfx_cpp::GDrip(std::string(v));
}
dev::u256 operator"" _drip(const char* v) {
	return cfx_cpp::Drip(std::string(v));
}