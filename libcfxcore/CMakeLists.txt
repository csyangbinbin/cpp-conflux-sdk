include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(EncodingSrc
common/encoding/hex.h
common/encoding/hex.cpp

)
source_group(encoding_src FILES  ${EncodingSrc})

set(UtilSrc
common/util/string_util.h
common/util/string_util.cpp
common/util/json_convert.h
)
source_group(util_src FILES  ${UtilSrc})

set(TypesSrc
        types/cfxaddress/lib.cpp
        types/cfxaddress/lib.h
        types/cfxaddress/network_type.cpp
        types/cfxaddress/network_type.h
        types/cfxaddress/version_byte.h
        types/cfxaddress/version_byte.cpp
        types/cfxaddress/body.cpp
        types/cfxaddress/body.h
        types/cfxaddress/checksum.cpp
        types/cfxaddress/checksum.h
        types/cfxaddress/address_type.h
        types/cfxaddress/address_type.cpp
        types/cfxaddress/address.h
        types/cfxaddress/address.cpp   
        types/epoch_number.h
        types/epoch_number.cpp
        types/log_enrty.h
        types/log_enrty.cpp
        types/transaction_receipt.h
        types/transaction_receipt.cpp
        types/log_filter.h
        types/log_filter.cpp
)
source_group(types_src FILES  ${TypesSrc})
 

set(CfxSrc
rpc/stubclient.h
rpc/httpclient.h
rpc/httpclient.cpp
rpc/iclientconnector.h
rpc/client.h
rpc/client.cpp
rpc/rpc_parameters.h
rpc/rpc_parameters.cpp
rpc/https_rpc_client.h
rpc/https_rpc_client.cpp
TransactionBase.h
TransactionBase.cpp
currency.cpp
currency.h
conflux.h
conflux.cpp
account.h
account.cpp
bloom_filter.h
bloom_filter.cpp
contract.h
contract.cpp
abi.h
)
source_group(cfx_src FILES  ${CfxSrc})

set(ABISrc
abi/Data.h
abi/Data.cpp
abi/ParamBase.h
abi/uint256.h
abi/ValueEncoder.h
abi/ValueEncoder.cpp
abi/ValueDecoder.h
abi/ValueDecoder.cpp
abi/ParamNumber.h
abi/ParamNumber.cpp
abi/ParamFactory.h
abi/ParamFactory.cpp
abi/Parameters.h
abi/Parameters.cpp
abi/ParamAddress.h
abi/Function.h
abi/Function.cpp
abi/Bytes.h
abi/Bytes.cpp
abi/Array.h
abi/Array.cpp
abi/Hash.h
abi/Hash.cpp
abi/HexCoding
)
source_group(abi_src FILES  ${ABISrc})

add_library(cpp_conflux_sdk
        ${EncodingSrc}
        ${TypesSrc}
        ${UtilSrc}
        ${CfxSrc}
        ${ABISrc}
        )

target_link_libraries( cpp_conflux_sdk 
devcrypto
libjson-rpc-cpp::client
CURL::libcurl)

target_include_directories(cpp_conflux_sdk
PRIVATE 
${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/common
${Boost_INCLUDE_DIR}
)

