// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "version_byte.h"

using namespace cfx_cpp;

std::map<uint32_t,uint8_t> VersionByte::hashSizeToBits{
         {160,0}
        ,{192,1}
        ,{224,2}
        ,{256,3}
        ,{320,4}
        ,{384,5}
        ,{448,6}
        ,{512,7}};