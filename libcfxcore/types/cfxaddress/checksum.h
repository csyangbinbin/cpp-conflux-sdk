// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CPP_CONFLUX_SDK_CHECKSUM_INCLUDE__
#define __CPP_CONFLUX_SDK_CHECKSUM_INCLUDE__
#include "network_type.h"
#include "body.h"

/*
Calculate checksum: We use the checksum algorithm of Bitcoin Cash, defined here.
uint64_t PolyMod(const data &v) {
    uint64_t c = 1;
    for (uint8_t d : v) {
        uint8_t c0 = c >> 35;
        c = ((c & 0x07ffffffff) << 5) ^ d;

        if (c0 & 0x01) c ^= 0x98f2bc8e61;
        if (c0 & 0x02) c ^= 0x79b76d99e2;
        if (c0 & 0x04) c ^= 0xf33e5fb3c4;
        if (c0 & 0x08) c ^= 0xae2eabe2a8;
        if (c0 & 0x10) c ^= 0x1e4f43e470;
    }

    return c ^ 1;
}
The checksum is calculated over the following data:

The lower 5 bits of each character of the prefix. - e.g. "bit..." becomes 2,9,20,...
A zero for the separator (5 zero bits).
The payload by chunks of 5 bits. If necessary, the payload is padded to the right with zero bits to complete any unfinished chunk at the end.
Eight zeros as a "template" for the checksum.
Importantly, optional fields (like address-type) are NOT part of the checksum computation.

The 40-bit number returned by PolyMod is split into eight 5-bit numbers (msb first). The payload and the checksum are then encoded according to the base32 character table.
*/

namespace cfx_cpp{
    byteArray CalcChecksum(NetworkType nt,  Body body) ;
}


#endif