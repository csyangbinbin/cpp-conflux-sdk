#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <memory>
#include <libcfxcore/conflux.h>

class AsyncQueryAccountThread;
class CfxTransferThread; 
class DeployContractThread;

namespace Ui {
class Widget;
}

class Widget : public QWidget{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void onBtnClicked_ImportPK();
    void onBtnClicked_Transfer();
    void onBtnClicked_TransferScan();
    void onBalanceChanged(double value);
    void onTransferFinished(QString transacationHash, int status);
    void onDeployFinished(int status, QString contractAddr);
    void onBtnDeplayContract();
    void onBtnScanContract();
    void onBtnSendToken();
    void ontBtnSendTokenScan();
private:
    void loadSolidityFile();
    void loadBytesCode(); 
private:
    Ui::Widget *ui;
    QString m_KeystoreDir; 
    QString m_DefaultPasswd;
    cfx_cpp::Address m_DefaultAccount;
    std::shared_ptr<cfx_cpp::AccountManager>  m_accountManagerPtr;
    std::shared_ptr<cfx_cpp::JsonRpcClient>  m_JsonRpcClient; 
    AsyncQueryAccountThread* m_asyncQueryAccountThread; 
    CfxTransferThread* m_transferThread; 
    DeployContractThread* m_deployContractThread; 
    QString minierc20_bytecode; 
    std::string m_lastTokenTransHash;
     
};

#endif // WIDGET_H
