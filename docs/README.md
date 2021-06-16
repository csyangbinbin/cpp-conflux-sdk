# cpp-conflux-sdk
The Conflux C++ API allows any C++ client to interact with a local or remote Conflux node based on JSON-RPC 2.0 protocol. With Conflux C++ API, user can easily manage accounts, send transactions, deploy smart contracts and query blockchain information.



## Features
- Connect to Conflux node over HTTPS JSON-RPC

- Account utilities

- Transaction Call and Send

- ABI encode and decode

- Contract utilities

- Type converter, sign and hash utilities




## Supported Platforms

Requires a codebase and compiler compliant with the C++17 standard or newer.The SDK code is officially supported on the following platforms.

### Operating Systems
- Linux
- Windows

### Compilers
- GCC 5.4+
- MSVC 2015+

### How To Build

Linux:

```cmake
mkdir build && cd build
cmake ../
make
```

Windows:

```
mkdir build && cd build
cmake ../
Go to the build directory and open cpp_conflux_sdk.sln
```

##  Docs

* [SDK Manual](https://csyangbinbin.github.io/cpp-conflux-sdk/)
* [Conflux Official document](https://developer.conflux-chain.org/docs/introduction/en/conflux_overview)
* [Conflux RPC endpoints](https://github.com/conflux-fans/conflux-rpc-endpoints)

