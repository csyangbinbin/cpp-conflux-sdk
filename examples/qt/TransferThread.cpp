#include "TransferThread.h"

CfxTransferThread::CfxTransferThread(std::shared_ptr<cfx_cpp::JsonRpcClient> client, cfx_cpp::TransactionBase& trans) :
	m_transaction(trans),
	m_JsonRpcClient(client) {

}
CfxTransferThread::~CfxTransferThread() {

}

void CfxTransferThread::run() {
	std::string transHash;
	try {
		transHash = m_JsonRpcClient->sendTransaction(m_transaction);
	}
	catch (jsonrpc::JsonRpcException& e) {
		std::cout << e.what();
		emit transferFinished(  QString::fromStdString(transHash)  , -1 );
		return; 
	}

	cfx_cpp::TransactionByHashResult	res;
	for (;;) {
		res = m_JsonRpcClient->cfx_getTransactionByHash(transHash);
		if (res.status == cfx_cpp::TransactionStatus::SkippedOrNotPacked) {
			msleep(200);
		}
		else {
			break;
		}
	}
	if (res.IsOK()) {
		std::cout << "sendTransaction OK" << std::endl;
		std::cout << res.from << " ===> " << res.to << std::endl;
		std::cout << "transHash:" << dev::toJS(res.hash) << std::endl;
		std::cout << "blockHash:" << dev::toJS(res.blockHash) << std::endl;
		std::cout << "gas:" << dev::toJS(res.gas) << std::endl;
		std::cout << "value:" << dev::toJS(res.value) << std::endl;
	}
	else {
		std::cout << "sendTransaction ERROR:" << res.status << std::endl;
		emit transferFinished(QString::fromStdString(transHash), -1);
	}

	cfx_cpp::TransactionReceipt res1;

	for (;;) {
		try {
			for (;;) {
				res1 = m_JsonRpcClient->cfx_getTransactionReceipt(transHash);
				if (res1.outcomeStatus != 0) {
					msleep(200);
					continue;
				}
				else
					goto DONE;

			}
		}
		catch (const std::exception&) {
		}

	}
DONE:
	emit transferFinished(QString::fromStdString(transHash), 0);
	return;

}
