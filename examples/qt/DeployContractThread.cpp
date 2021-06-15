#include "DeployContractThread.h"

DeployContractThread::DeployContractThread(const dev::bytes& contractBytes, const dev::bytes& constructor_abi, const dev::Secret& secret) :
	m_contractBytes(contractBytes),
	m_constructor_abi(constructor_abi),
	m_secret(secret) {
}
DeployContractThread::~DeployContractThread() {
}

void DeployContractThread::run() {
	auto JsonRpcClient = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.com");
	cfx_cpp::ContractDeployResult contractRes;
	try {
		contractRes = JsonRpcClient->DeployContract(m_contractBytes, m_constructor_abi, m_secret);
		std::cout << "transaction_hash:" << dev::toJS(contractRes.transactionHash) << std::endl;;
	}
	catch (jsonrpc::JsonRpcException& e) {
		std::cout << "DeployContract failed:" << e.what() << std::endl;
		emit deployFinished(-1, "");
		return ;
	}

	bool isok = JsonRpcClient->WaitTransactionFinished(contractRes.transactionHash);

	if (isok) {
		cfx_cpp::Address contractAddr = contractRes.contractCreatedAddr;
		emit deployFinished(0, QString::fromStdString(contractAddr.GetVerboseBase32Address()));
	}
	else {
		emit deployFinished(-1, "");
	}
	
}
