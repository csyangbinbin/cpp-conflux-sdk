#ifndef __DEPLOY_CONTRACT_INCLUDE__
#define __DEPLOY_CONTRACT_INCLUDE__
#include <QThread>
#include <libcfxcore/conflux.h>

class DeployContractThread : public QThread {
	Q_OBJECT
public:
	DeployContractThread(const dev::bytes& contractBytes, const dev::bytes& constructor_abi, const dev::Secret& secret);
	virtual ~DeployContractThread();
protected:
	void run() Q_DECL_OVERRIDE; 
signals:
	void deployFinished(int status ,QString contractAddr);
private:
	dev::bytes m_contractBytes; 
	dev::bytes m_constructor_abi; 
	dev::Secret m_secret;
};

#endif
