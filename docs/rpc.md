## JSON RPC

The Conflux JSON-RPC API is a collection of interfaces that allow you to interact with a local or remote Conflux node.

The following rpc api have been implemented:

- [x]     cfx_getTransactionByHash
- [x]     cfx_getBlockByHash
- [x]     cfx_getBlockByEpochNumber
- [x]     cfx_getBestBlockHash
- [x]     cfx_epochNumber
- [x]     cfx_gasPrice
- [x]     cfx_getBlocksByEpoch
- [x]     cfx_getBalance
- [x]     cfx_getStakingBalance
- [x]     cfx_getCollateralForStorage
- [x]     cfx_getAdmin
- [x]     cfx_getCode
- [x]     cfx_getStorageAt
- [x]     cfx_getStorageRoot
- [x]     cfx_getSponsorInfo
- [x]     cfx_getNextNonce
- [x]     cfx_sendRawTransaction
- [x]     cfx_call
- [x]     cfx_estimateGasAndCollateral
- [x]     cfx_getLogs
- [x]     cfx_getTransactionReceipt
- [x]     cfx_getAccount
- [x]     cfx_getInterestRate
- [x]     cfx_getAccumulateInterestRate
- [x]     cfx_checkBalanceAgainstTransaction
- [x]     cfx_getSkippedBlocksByEpoch
- [x]     cfx_getConfirmationRiskByHash
- [x]     cfx_getStatus
- [x]     cfx_clientVersion
- [x]     cfx_getBlockRewardInfo
- [x]     cfx_getBlockByHashWithPivotAssumption
- [x]     cfx_getDepositList
- [x]     cfx_getVoteList

### example

```cpp
//creat an HTTPS RPC Client instance
auto client = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.com");

//cfx_clientVersion
std::string version = client->cfx_clientVersion();

//cfx_getBlockByHash
cfx_cpp::cfx_getBlockByHashResult block = client->cfx_getBlockByHash("0x4185472402e6851f3c393e54d54aebdf46b8171a9a32fcf15622db7230d0b314");

//cfx_getBlockByEpochNumber
cfx_cpp::cfx_getBlockByHashResult epoch_block = client->cfx_getBlockByEpochNumber(dev::u256("0x1C3E9FF"));
dev::h256 best_block_hash = client->cfx_getBestBlockHash(); 

//cfx_getBlocksByEpoch
dev::h256s  blocks =  client->cfx_getBlocksByEpoch(dev::u256("0x1C3EAD6"));

//cfx_getStakingBalance
dev::u256 stacking_balance = client->cfx_getStakingBalance(cfx_cpp::Address("cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42") );

//cfx_getCollateralForStorage
dev::u256 CollateralForStorage = client->cfx_getCollateralForStorage(cfx_cpp::Address("cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42"));
std::cout << "CollateralForStorage:" << dev::toJS(CollateralForStorage) << std::endl;
	
```

