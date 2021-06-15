// Copyright  2021 csyangbinbin. All rights reserved. 
// Licensed under the GNU General Public License, Version 3.
//
// This file is part of Conflux C++ SDK. The full copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "string_util.h"

#include <boost/algorithm/string.hpp>
std::vector<std::string> cfx_cpp::string_util::split_string(const std::string& strtem, char a){
    std::vector<std::string> strvec;
    std::string::size_type pos1, pos2;
    pos2 = strtem.find(a);
    pos1 = 0;
    while (std::string::npos != pos2){
        strvec.push_back(strtem.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
        pos2 = strtem.find(a, pos1);
    }
    strvec.push_back(strtem.substr(pos1));
    return strvec;
}


std::string cfx_cpp::string_util::random_string(size_t len) {
    srand( static_cast<unsigned int>( time(NULL) ));
    std::string str;
    str.resize(len + 1);
    size_t i = 0;
    for (i = 0; i < len; ++i) {
        switch ((rand() % 3)){
            case 1:
                str[i] = 'A' + rand() % 26;
                break;
            case 2:
                str[i] = 'a' + rand() % 26;
                break;
            default:
                str[i] = '0' + rand() % 10;
                break;
        }
    }
    str[++i] = '\0';
    return str;
}



std::string cfx_cpp::string_util::tolower_string(const std::string& sourceString){
    std::string destinationString;
    destinationString.resize(sourceString.size());
    std::transform(sourceString.begin(),
                   sourceString.end(),
                   destinationString.begin(),
                   ::tolower);
    return destinationString ;
}

std::string cfx_cpp::string_util::toupper_string(const std::string& sourceString){
    std::string destinationString ;
    destinationString.resize(sourceString.size());
    std::transform(sourceString.begin(),
                   sourceString.end(),
                   destinationString.begin(),
                   ::toupper);
    return destinationString ;
}

bool cfx_cpp::string_util::contains(const std::string& src , const std::string& substring){

    return  boost::algorithm::contains(src, substring); 
  /*  auto it = std::search(src.begin(), src.end(),
                          std::boyer_moore_searcher(
                                  substring.begin(), substring.end()));
    return (it != src.end()) ;*/
}
