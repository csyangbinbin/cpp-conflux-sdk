#ifndef __ASYNC_QUERY_ACCOUNT_INCLUDE__
#define __ASYNC_QUERY_ACCOUNT_INCLUDE__
#include <QThread>
#include <libcfxcore/conflux.h>
class AsyncQueryAccountThread :public QThread {
	Q_OBJECT
public:
	AsyncQueryAccountThread(cfx_cpp::Address addr );
	virtual ~AsyncQueryAccountThread();
public:
	void setAccountAddress(cfx_cpp::Address addr);
	void setJsonRpcClient(std::shared_ptr<cfx_cpp::JsonRpcClient> client);

	void run() Q_DECL_OVERRIDE; 


signals:
	void balanceChanged(double value);

private:
	std::shared_ptr<cfx_cpp::JsonRpcClient> m_JsonRpcClient;
	cfx_cpp::Address m_DefaultAddress;
	double m_balance;
};


#endif
