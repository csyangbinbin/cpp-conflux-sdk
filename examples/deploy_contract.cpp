#include <fstream>
#include <libcfxcore/conflux.h>
#include <libdevcore/CommonJS.h>

using namespace TW::Ethereum::ABI;
using namespace std;

dev::bytes loadContractBytes(const std::string& contractFile);

int main() {
	//读取合约字节码
	std::string miniERC20 = "C:/contract/miniERC20.json"; 
	dev::bytes contract_bytes = loadContractBytes(miniERC20);
	if (contract_bytes.empty()) {
		cout << "reade bytecode error!" << std::endl;
		return 0;
	}

	cfx_cpp::Address from("cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42"); //创建合约账户
	dev::Secret SecretKey("9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547");//创建合约账户的秘钥

	auto client = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.org/v2"); //new RPC client

	//构造合约创建的构造函数的参数
	auto constroctorParams = std::make_shared<ParamSet>();
	constroctorParams->addParam(std::make_shared<ParamString>("TigerCoin")); //创建ERC20代码(名字叫---老虎币-_-!)
	constroctorParams->addParam(std::make_shared<ParamUInt8>(18));
	constroctorParams->addParam(std::make_shared<ParamString>("TGC"));
	constroctorParams->addParam(std::make_shared<ParamUInt256>(cfx_cpp::CFX("10000000000"))); //发行100亿
	TW::Data ParamsABI;
	constroctorParams->encode(ParamsABI); //构造函数进行ABI编码

	cfx_cpp::ContractDeployResult contractRes;
	try {
		contractRes = client->DeployContract(contract_bytes , ParamsABI, SecretKey);
		cout << "transaction_hash:" << dev::toJS(contractRes.transactionHash) << std::endl;;
	}
	catch (jsonrpc::JsonRpcException& e) {
		std::cout << "DeployContract failed:" << e.what() <<std::endl;
		return 0;
	}

	cfx_cpp::Address TigerCoinAddr = contractRes.contractCreatedAddr; //新创建的合约的地址
	std::shared_ptr<cfx_cpp::Contract> deployedContract = std::make_shared<cfx_cpp::Contract>(client,TigerCoinAddr);

	{
		//查询代码符号
		Function function("symbol");
		auto index = function.addOutParam(std::make_shared<ParamString>());
		deployedContract->Call(function); //进行合约的CALL调用
		std::shared_ptr<ParamBase> param_out;
		function.getOutParam(index, param_out);
		std::string symbol = std::dynamic_pointer_cast<ParamString>(param_out)->getVal();
		std::cout << "symbol:" << symbol << std::endl;
	}

	{
		//向其他账户发送代币
		cfx_cpp::Address transferto("cfxtest:aakd0d2x8zuu7zr3vfm3rwnjyv5rxxuvyj96acpdx8");
		Function transfer("transfer");
		transfer.addInParam(std::make_shared<ParamAddress>(TW::data(transferto.toEthCommonAddr().data(), 20))); //接收地址
		transfer.addInParam(std::make_shared<ParamUInt256>(cfx_cpp::CFX("1"))); //发送的代币数量
		transfer.addOutParam(std::make_shared<ParamBool>()); //返回参数
		try {
			std::string transHash = deployedContract->SendTransaction(transfer, SecretKey, 0);
			std::cout << "Transfer_TransHash:" << transHash << std::endl;
		}
		catch (const std::exception& e) {
			cout << "transfer error:" << e.what() << std::endl;
		}
	}

	{
		//检查我们自己账户的余额，应该会减少1
		Function balanceOf("balanceOf");
		balanceOf.addInParam(std::make_shared<ParamAddress>(TW::data(from.toEthCommonAddr().data(), 20)));
		balanceOf.addOutParam(std::make_shared<ParamUInt256>());

		TW::Data outBalanceOfP = deployedContract->Call(balanceOf);//进行Call调用

		std::shared_ptr<ParamBase> param_out_0;
		balanceOf.getOutParam(0, param_out_0);
		dev::u256 balance = std::dynamic_pointer_cast<ParamUInt256>(param_out_0)->getVal();
		std::cout << "balance:" << dev::toJS(balance);
	}

	getchar();
	int a = 0;
}

dev::bytes loadContractBytes(const std::string& contractFile) {
	ifstream ifs;
	ifs.open(contractFile);
	assert(ifs.is_open());

	Json::Reader reader;
	Json::Value root;
	dev::bytes contract_bytes;

	if (!reader.parse(ifs, root, false)) {
		cout << "reader parse error: " << strerror(errno) << endl;
		return contract_bytes;
	}

	if (!root.isObject()) {
		return contract_bytes;
	}
	if (root.isMember("bytecode") && root["bytecode"].isString()) {
		std::string bytecode = root["bytecode"].asString();
		contract_bytes = cfx_cpp::encoding::hex::DecodeString(bytecode.substr(2));
	}
	return contract_bytes;
}