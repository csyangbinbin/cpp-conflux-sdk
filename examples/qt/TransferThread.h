#ifndef __TRANSFER_INCLUDE__
#define __TRANSFER_INCLUDE__
#include <QThread>
#include <libcfxcore/conflux.h>
class CfxTransferThread:public QThread {
	Q_OBJECT
public:
	CfxTransferThread(std::shared_ptr<cfx_cpp::JsonRpcClient> client,cfx_cpp::TransactionBase& trans);
	virtual ~CfxTransferThread();
	
	void run() Q_DECL_OVERRIDE;
signals:
	void transferFinished(QString transacationHash , int status);
private:
	std::shared_ptr<cfx_cpp::JsonRpcClient> m_JsonRpcClient;
	cfx_cpp::TransactionBase m_transaction; 
};

#endif
