#include "AsyncQueryAccountThread.h"
AsyncQueryAccountThread::AsyncQueryAccountThread(cfx_cpp::Address addr):
	m_DefaultAddress(addr),
	m_balance(0)
{
	m_JsonRpcClient = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.com");
}

AsyncQueryAccountThread::~AsyncQueryAccountThread() {

}

void AsyncQueryAccountThread::setAccountAddress(cfx_cpp::Address addr) {
	m_DefaultAddress = addr;
}
void AsyncQueryAccountThread::setJsonRpcClient(std::shared_ptr<cfx_cpp::JsonRpcClient> client) {
	m_JsonRpcClient = client; 
}


void AsyncQueryAccountThread::run() {
	for (;;) {
		try
		{
			dev::u256 balance = m_JsonRpcClient->cfx_getBalance(m_DefaultAddress);
			double balanceDouble = cfx_cpp::Drip2CFXDouble(balance);

			if (balanceDouble != m_balance)
				emit balanceChanged(balanceDouble);
			m_balance = balanceDouble;
			msleep(2000);
		}
		catch (const std::exception&)	{

		}
	
	}

}