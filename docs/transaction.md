## Send Transaction

To send a transaction, first you need to build a transaction and sign the transaction at local machine, then send the signed transaction to local or remote Conflux node.

In Conflux ,an transaction contains the following parts:

```cpp
/*
the base32 address of the receiver of the transaction, could be a personal account (e.g. CFXTEST:TYPE.USER:AARC9ABYCUE0HHZGYRR53M6CXEDGCCRMMY8M50BU1P) or a contract (e.g. CFXTEST:TYPE.CONTRACT:ACC7UAWF5UBTNMEZVHU9DHC6SGHEA0403YWJZ6WTPG). Leave a null here to deploy a contract.
*/
cfx_cpp::Address	to;

/*The sender account(with private key) to sign the transaction.*/
cfx_cpp::Address	from;

/*the value (in Drip) to be transferred.*/
dev::u256 m_value;	

/*
the max storage (in Byte) you would like to collateralize in the transaction. If missing, the result of cfx_estimateGasAndCollateral will be automatically filled in and it works for general senarios.transactions.
*/
dev::u256 m_storageLimit;		

/*
a transaction is can be verified only in epochs in the range [epochHeight - 100000, epochHeight + 100000], so it's a timeout mechanism. If missing, the result of cfx_epochNumber will be automatically filled in and it works for general scenarios.
*/
dev::u256 m_epochHeight;	

/*
it used for dealing with a hard fork or preventing a transaction replay attack. If missing, the SDK will get it from RPC. Currently mainnet chainId is 1029, testnet is 1.
*/
dev::u256 m_chainId;


/*
the nonce of a transaction to keep the order of your sending transactions, starting with some random large number and increase one by one. If missing, the result of cfx_getNextNonce will be automatically filled in and it works for general scenarios. Some cases, like sending a lot of transactions in a short period. It's recommended to maintain the nonce on your own.
*/
dev::u256 m_nonce;

/*
the price in Drip that you would like to pay for each gas consumed. If missing, the result of cfx_gasPrice will be automatically filled in, which is the median of recent transactions.
*/
dev::u256 m_gasPrice;	

/*
the max gas you would like to use in the transaction. After the end of transaction processing, the unused gas will be refunded if used_gas >= gas * 0.75. If missing, the result of cfx_estimateGasAndCollateral will be automatically filled in and it works for general scenarios.
*/
dev::u256 m_gas;

/*
it's either an attached message of a transaction or a function signature of a contract call. If missing, a null will be filled into it.
*/
dev::bytes m_data;	 
```

### Signature

signatures are a key part of the blockchain. They are used to prove ownership of an address without exposing its private key.

#### Signing and verifying using ECDSA

ECDSA signatures consist of two numbers (integers): `r` and `s`. Conflux also uses an additional `v` (recovery identifier) variable. The signature can be notated as `{r, s, v}`.

steps:

1. construct the following list : [nonce, gasPrice , gas , toAddress  ,  value , storageLimit  , epochHeight , chainI , data]  .if the transaction is an constract create type , the paramter " toAddress" is set to empty . if no data to send ,the paramter "data" is set to empty . 
2. Calculate the SHA hash values in the list above  , return { r,s,v} .

Signed transactions are [RLP](https://eth.wiki/en/fundamentals/rlp) encoded, and consist of all transaction parameters (nonce, gasPrice , gas , toAddress  ,  value , storageLimit  , epochHeight , chainI , data) and the signature (v, r, s). 

To get the final transmission data ,you need calc  **SHA3([ [nonce, gasPrice , gas , toAddress  ,  value , storageLimit  , epochHeight , chainI , data] ,v,r,s])** ; 

```cpp
dev::Secret SecretKey("9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547"); //private key
cfx_cpp::Address to("cfxtest:aakd0d2x8zuu7zr3vfm3rwnjyv5rxxuvyj96acpdx8"); //toAddress
dev::u256 value = cfx_cpp::CFX("1"); //value
cfx_cpp::TransactionBase trans(to , value  , privateKey); //create Transaction instance
trans.withGasPrice(..) //set new GasPrice
trans.withGas(..)//set new Gas
trans.withValue(..)//set new value
trans.withData(..)//set new data
std::string transHash = client->sendTransaction(trans); //send this transaction and return  transactionHash
    
```

### Deploy Contract

If you just want to deploy a contract, invoke function `DeployContract` (using sample contract code :miniERC20 ):

```cpp
auto constroctorParams = std::make_shared<ParamSet>();
constroctorParams->addParam(std::make_shared<ParamString>("TigerCoin")); //create ERC20 toke(the name is---tiger-_-!)
constroctorParams->addParam(std::make_shared<ParamUInt8>(18));
constroctorParams->addParam(std::make_shared<ParamString>("TGC"));
constroctorParams->addParam(std::make_shared<ParamUInt256>(cfx_cpp::CFX("10000000000")));
TW::Data ParamsABI;
constroctorParams->encode(ParamsABI); //constructor parameters abi encode

cfx_cpp::ContractDeployResult contractRes;
try {
	contractRes = client->DeployContract(contract_bytes , ParamsABI, SecretKey);
	cout << "transaction_hash:" << dev::toJS(contractRes.transactionHash) << std::endl;;
}
catch (jsonrpc::JsonRpcException& e) {
	std::cout << "DeployContract failed:" << e.what() <<std::endl;
	return 0;
}

cfx_cpp::Address TigerCoinAddr = contractRes.contractCreatedAddr; //created contract address

std::shared_ptr<cfx_cpp::Contract> deployedContract = std::make_shared<cfx_cpp::Contract>(client,TigerCoinAddr);

{
		//call symbol
		Function function("symbol");
		auto index = function.addOutParam(std::make_shared<ParamString>());
		deployedContract->Call(function); //call 
		std::shared_ptr<ParamBase> param_out;
		function.getOutParam(index, param_out);
		std::string symbol = std::dynamic_pointer_cast<ParamString>(param_out)->getVal();
		std::cout << "symbol:" << symbol << std::endl;
}

{
		//transfer
		cfx_cpp::Address transferto("cfxtest:aakd0d2x8zuu7zr3vfm3rwnjyv5rxxuvyj96acpdx8");
		Function transfer("transfer");
		transfer.addInParam(std::make_shared<ParamAddress>(TW::data(transferto.toEthCommonAddr().data(), 20))); 
		transfer.addInParam(std::make_shared<ParamUInt256>(cfx_cpp::CFX("1"))); 
		transfer.addOutParam(std::make_shared<ParamBool>()); 
		try {
			std::string transHash = deployedContract->SendTransaction(transfer, SecretKey, 0);
			std::cout << "Transfer_TransHash:" << transHash << std::endl;
		}
		catch (const std::exception& e) {
			cout << "transfer error:" << e.what() << std::endl;
		}
}
```

