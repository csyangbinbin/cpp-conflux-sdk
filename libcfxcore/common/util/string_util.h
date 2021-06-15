// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef __CFX_STRING_UTIL_INCLUDE__
#define __CFX_STRING_UTIL_INCLUDE__
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

namespace cfx_cpp{
    namespace string_util{
        template <typename charT>
        inline bool starts_with(const std::basic_string<charT>& big, const std::basic_string<charT>& smallstr) {
            if (&big == &smallstr) return true;
            const typename std::basic_string<charT>::size_type big_size = big.size();
            const typename std::basic_string<charT>::size_type small_size = smallstr.size();
            const bool valid_ = (big_size >= small_size);
            const bool starts_with_ = (big.compare(0, small_size, smallstr) == 0);
            return valid_ && starts_with_;
        }

        template <typename charT>
        inline bool ends_with(const std::basic_string<charT>& big, const std::basic_string<charT>& smallstr) {
            if (&big == &smallstr) return true;
            const typename std::basic_string<charT>::size_type big_size = big.size();
            const typename std::basic_string<charT>::size_type small_size = smallstr.size();
            const bool valid_ = (big_size >= small_size);
            const bool ends_with_ = (big.compare(big_size - small_size, small_size, smallstr) == 0);
            return valid_ && ends_with_;
        }
        std::vector<std::string> split_string(const std::string& strtem, char a);
        std::string random_string(size_t len);
        std::string tolower_string(const std::string& sourceString) ;
        std::string toupper_string(const std::string& sourceString);
        bool contains(const std::string& src , const std::string& substring) ;
    }
}

#endif