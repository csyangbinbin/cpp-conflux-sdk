cmake_minimum_required(VERSION 3.9.3)

include(cmake/cable/bootstrap.cmake)
include(CableBuildInfo)
include(CableBuildType)
include(CableToolchains)
include(GNUInstallDirs)

cable_configure_toolchain(DEFAULT cxx17-pic)

set(CFX_CPP_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${CFX_CPP_CMAKE_DIR})

set(HUNTER_CONFIGURATION_TYPES Release CACHE STRING "Hunter configuration types")
set(HUNTER_JOBS_NUMBER 4 CACHE STRING "Hunter jobs number")
set(HUNTER_CACHE_SERVERS "https://github.com/ethereum/hunter-cache" CACHE STRING "Hunter cache servers")
set(HUNTER_PASSWORDS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Hunter/passwords.cmake CACHE STRING "Hunter password path")

include(HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.211.tar.gz"
    SHA1 "c022f0c2480e835a6bafdd734035ec2071b51897"
    LOCAL
)
cable_set_build_type(DEFAULT RelWithDebInfo CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)

project(cpp_conflux_sdk)
set(PROJECT_VERSION 0.0.1)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
#hunter_add_package(Boost COMPONENTS program_options filesystem system thread context fiber log)
#find_package(Boost CONFIG REQUIRED program_options filesystem system thread context fiber log)
hunter_add_package(Boost COMPONENTS  filesystem system  thread log)
find_package(Boost CONFIG REQUIRED  filesystem system  thread log)

hunter_add_package(cryptopp)
find_package(cryptopp CONFIG REQUIRED)

hunter_add_package(jsoncpp)
find_package(jsoncpp CONFIG REQUIRED)

hunter_add_package(libjson-rpc-cpp)
find_package(libjson-rpc-cpp CONFIG REQUIRED)

hunter_add_package(libscrypt)
find_package(libscrypt CONFIG REQUIRED)

hunter_add_package(ethash)
find_package(ethash CONFIG REQUIRED)

hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

hunter_add_package(CURL)
find_package(CURL CONFIG REQUIRED)

include(ProjectSecp256k1)
include(ProjectLibFF)

#Global include path for all libs.
set(UTILS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/utils")
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${UTILS_INCLUDE_DIR})


add_subdirectory(libdevcore)
add_subdirectory(libdevcrypto)
add_subdirectory(libcfxcore)
add_subdirectory(examples)