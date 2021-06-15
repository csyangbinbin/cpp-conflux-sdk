#include <iostream>
#include <libcfxcore/conflux.h>

int main() {
	auto client = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.com");

	std::string version = client->cfx_clientVersion();

	cfx_cpp::cfx_getBlockByHashResult block = client->cfx_getBlockByHash("0x4185472402e6851f3c393e54d54aebdf46b8171a9a32fcf15622db7230d0b314");
	std::cout << "block.miner:" << block.miner.GetVerboseBase32Address()<<std::endl;


	cfx_cpp::cfx_getBlockByHashResult epoch_block = client->cfx_getBlockByEpochNumber(dev::u256("0x1C3E9FF"));
	std::cout << "epoch_block.miner:" << epoch_block.miner.GetVerboseBase32Address() << std::endl;
	

	dev::h256 best_block_hash = client->cfx_getBestBlockHash(); 
	std::cout << "BestBlockHas:" << dev::toJS(best_block_hash) << std::endl;;



	dev::h256s  blocks =  client->cfx_getBlocksByEpoch(dev::u256("0x1C3EAD6"));
	std::cout << "cfx_getBlocksByEpoch(0x1C3EAD6):[" << std::endl;
	for (auto& b : blocks) {
		std::cout << dev::toJS(b) << std::endl; 
	}
	std::cout << "]" << std::endl;


	dev::u256 stacking_balance = client->cfx_getStakingBalance(cfx_cpp::Address("cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42") );
	std::cout <<"stacking_balance:" <<  cfx_cpp::Drip2CFXDouble(stacking_balance) <<std::endl ;

	dev::u256 CollateralForStorage = client->cfx_getCollateralForStorage(cfx_cpp::Address("cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42"));
	std::cout << "CollateralForStorage:" << dev::toJS(CollateralForStorage) << std::endl;
	

	boost::optional<cfx_cpp::Address> admin_addr = client->cfx_getAdmin(cfx_cpp::Address("cfxtest:acf1tv2wj90bpxrssfp748zh5wmrp88wjevx9bb35p"));
	if (admin_addr) {
		std::cout << "Contract Admin:" << admin_addr.get().GetVerboseBase32Address() <<std::endl;
	}
	else {
		std::cout << "contract does not exist!" <<std::endl ; 
	}

	dev::bytes contract_code = client->cfx_getCode(cfx_cpp::Address("cfxtest:acf1tv2wj90bpxrssfp748zh5wmrp88wjevx9bb35p"));
	if (contract_code.size()>0) {
		std::cout << "contract_code_size:" << contract_code.size() <<std::endl ;
	}
	else {
		std::cout << " the account has no code!" << std::endl;
	}
	
	cfx_cpp::cfx_getSponsorInfoResult sponsor_info = client->cfx_getSponsorInfo(cfx_cpp::Address("cfxtest:acf1tv2wj90bpxrssfp748zh5wmrp88wjevx9bb35p"));
	std::cout << "sponsorForCollateral:" << sponsor_info.sponsorForCollateral.GetVerboseBase32Address() << std::endl; ;


	getchar();
	return  0;
}