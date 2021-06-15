add_library(
    devcore
    Address.cpp
    Address.h
    Common.cpp
    Common.h
    CommonData.cpp
    CommonData.h
    CommonIO.cpp
    CommonIO.h
    Exceptions.h
    FileSystem.cpp
    FileSystem.h
    FixedHash.cpp
    FixedHash.h
    Guards.cpp
    Guards.h  
    Log.cpp
    Log.h
    RLP.cpp
    RLP.h
    SHA3.cpp
    SHA3.h
    Terminal.h
    vector_ref.h
    CommonJS.h
    CommonJS.cpp
    KeyManager.h
    KeyManager.cpp
)

# Needed to prevent including system-level boost headers:
target_include_directories(devcore SYSTEM PUBLIC ${Boost_INCLUDE_DIR} PRIVATE ../utils)
target_link_libraries(devcore PUBLIC  Boost::filesystem Boost::system Boost::log Boost::thread Threads::Threads PRIVATE ethash::ethash)