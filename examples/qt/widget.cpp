#include "widget.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QFile>
#include <QDesktopServices>
#include <QMessageBox> 
#include "ui_widget.h"
#include "AsyncQueryAccountThread.h"
#include "TransferThread.h"
#include "DeployContractThread.h"

using namespace TW::Ethereum::ABI;

Widget::Widget(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::Widget),
	m_asyncQueryAccountThread(NULL)
{
	ui->setupUi(this);
	connect(ui->pushButton_ImportPK, SIGNAL(clicked()), this, SLOT(onBtnClicked_ImportPK()));
	connect(ui->pushButton_Transfer, SIGNAL(clicked()), this, SLOT(onBtnClicked_Transfer()));
	connect(ui->pushButton_TransferScan, SIGNAL(clicked()), this, SLOT(onBtnClicked_TransferScan()));
	connect(ui->pushButton_DeplayContract, SIGNAL(clicked()), this, SLOT(onBtnDeplayContract()));
	connect(ui->pushButton_ScanContract, SIGNAL(clicked()), this, SLOT(onBtnScanContract()));
	connect(ui->pushButton_TokenSend, SIGNAL(clicked()), this, SLOT(onBtnSendToken()));
	connect(ui->pushButton_TokenSendScan, SIGNAL(clicked()), this, SLOT(ontBtnSendTokenScan()));


	m_KeystoreDir = "C://KeyStore_CFX_QT";
	m_DefaultPasswd = "12345678";

	m_accountManagerPtr = std::make_shared<cfx_cpp::AccountManager>(m_KeystoreDir.toStdString(), cfx_cpp::NetworkType::NetworkTypeTestnetID);
	m_JsonRpcClient = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.com");

	loadSolidityFile();
	loadBytesCode();
}

Widget::~Widget() {
	delete ui;
}
void Widget::onBtnClicked_ImportPK() {
	QString  PK_str = ui->lineEdit_PrivateKey->text();
	dev::Secret PK(PK_str.toStdString());
	m_DefaultAccount = m_accountManagerPtr->ImportPrivateKey(PK, m_DefaultPasswd.toStdString()); //从私钥导入账号
	std::cout << "Import Account:" << m_DefaultAccount.GetVerboseBase32Address() << std::endl;

	ui->lineEdit_DefaultAddress->setText(QString::fromStdString(m_DefaultAccount.GetBase32Address()));

	if (m_asyncQueryAccountThread) {
		m_asyncQueryAccountThread->setAccountAddress(m_DefaultAccount);
	}
	else {
		m_asyncQueryAccountThread = new AsyncQueryAccountThread(m_DefaultAccount);
		connect(m_asyncQueryAccountThread, SIGNAL(balanceChanged(double)),
			this, SLOT(onBalanceChanged(double)));
		m_asyncQueryAccountThread->start();
	}

}

void Widget::onBalanceChanged(double value) {
	std::cout << "balanceDouble:" << value << std::endl;
	QString str = QString("%1").arg(value, 0, 'f', 5);
	ui->lineEdit_Balance->setText(str);
}
void Widget::onBtnClicked_Transfer() {
	//查询发送账户密钥，用于交易签名
	auto privateKey = m_accountManagerPtr->GetSecret(m_DefaultAccount, m_DefaultPasswd.toStdString());
	if (!privateKey) {
		std::cout << "Account Manager GetSecret Error!" << std::endl;
		return;
	}
	else {
		std::cout << "GetSecret OK" << std::endl;
	}

	dev::u256 value = cfx_cpp::CFX(ui->lineEdit_RecvCount->text().toStdString()); //发送1cfx
	cfx_cpp::Address to(ui->lineEdit_Recv->text().toStdString());
	cfx_cpp::TransactionBase trans(to, value, privateKey);

	m_transferThread = new CfxTransferThread(m_JsonRpcClient, trans);
	connect(m_transferThread, SIGNAL(transferFinished(QString, int)),
		this, SLOT(onTransferFinished(QString, int)));
	m_transferThread->start();
	ui->pushButton_Transfer->setEnabled(false);
}

void Widget::onTransferFinished(QString transacationHash, int status) {
	qDebug() << "onTransferFinished:" << transacationHash << " : " << status;
	ui->pushButton_Transfer->setEnabled(true);
	if (status >= 0) {
		QMessageBox::information(this, "Information", "The transaction is completed.");
	}
	else {
		QMessageBox::critical(this, "Error", "transaction failed!");
		return;
	}

	ui->lineEdit_TransactionHash->setText(transacationHash);
}

void Widget::onBtnClicked_TransferScan() {
	QString hash = ui->lineEdit_TransactionHash->text();
	QString URL("https://testnet.confluxscan.io/transaction/" + hash);
	QDesktopServices::openUrl(URL);
}

void Widget::loadSolidityFile() {
	QString app_dir_path = QCoreApplication::applicationDirPath();
	const static QString solidityFileName = "miniERC20.sol";
	QString fileName = app_dir_path + "/" + solidityFileName;
	QFile file(fileName);
	bool isok = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!isok) {
		QMessageBox::critical(this, "ERROR", "cant open solidity file");
		return;
	}
	QByteArray t = file.readAll();
	ui->textEdit_Solidity->setText(QString(t));
}
void Widget::loadBytesCode() {
	QString app_dir_path = QCoreApplication::applicationDirPath();
	const static QString solidityJsonFileName = "miniERC20.json";
	QString fileName = app_dir_path + "/" + solidityJsonFileName;
	QFile file(fileName);
	bool isok = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!isok) {
		QMessageBox::critical(this, "ERROR", "cant open solidity json file");
		return;
	}
	QByteArray t = file.readAll();

	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(t, &jsonError);
	if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {
		if (doucment.isObject()) {
			QJsonObject object = doucment.object();
			if (object.contains("bytecode")) {
				minierc20_bytecode = object.value("bytecode").toString();
				ui->textEdit_BytesCode->setText(minierc20_bytecode);
			}
		}
	}
}
void Widget::onBtnDeplayContract() {
	QString Name = ui->lineEdit_Name->text();
	QString Symbol = ui->lineEdit_Symbol->text();
	int Decimals = ui->lineEdit_Decimals->text().toInt();
	int TotalSupply = ui->lineEdit_TotalSupply->text().toInt();

	auto constroctorParams = std::make_shared<ParamSet>();
	constroctorParams->addParam(std::make_shared<ParamString>(Name.toStdString()));
	constroctorParams->addParam(std::make_shared<ParamUInt8>(Decimals));
	constroctorParams->addParam(std::make_shared<ParamString>(Symbol.toStdString()));
	constroctorParams->addParam(std::make_shared<ParamUInt256>(cfx_cpp::CFX(TotalSupply)));
	TW::Data ParamsABI;
	constroctorParams->encode(ParamsABI); //构造函数进行ABI编码

	dev::bytes contract_bytes = cfx_cpp::encoding::hex::DecodeString(minierc20_bytecode.toStdString().substr(2));


	auto privateKey = m_accountManagerPtr->GetSecret(m_DefaultAccount, m_DefaultPasswd.toStdString());
	if (!privateKey) {
		std::cout << "Account Manager GetSecret Error!" << std::endl;
		return;
	}
	else {
		std::cout << "GetSecret OK" << std::endl;
	}

	m_deployContractThread = new DeployContractThread(contract_bytes, ParamsABI, privateKey);
	connect(m_deployContractThread, SIGNAL(deployFinished(int, QString)), this, SLOT(onDeployFinished(int, QString)));
	m_deployContractThread->start();
	ui->pushButton_DeplayContract->setEnabled(false);
}
void Widget::onBtnScanContract() {

	QString contractAddr = ui->lineEdit_ContractAddress->text();
	if (contractAddr.size() > 0) {
		QString addr = "https://testnet.confluxscan.io/address/" + contractAddr;
		QString URL(addr);
		QDesktopServices::openUrl(URL);
	}
}

void Widget::onDeployFinished(int status, QString contractAddr) {
	if (status < 0) {
		QMessageBox::critical(this, "ERROR", "Deploy Contract Error!");
		return;
	}
	QMessageBox::information(this, "Information", "Deploy Contract Success!");
	ui->pushButton_DeplayContract->setEnabled(true);
	ui->lineEdit_ContractAddress->setText(contractAddr);


	cfx_cpp::Address Addr(ui->lineEdit_ContractAddress->text().toStdString());
	auto client = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.com");
	std::shared_ptr<cfx_cpp::Contract> deployedContract = std::make_shared<cfx_cpp::Contract>(client, Addr);
	//query balance
	cfx_cpp::Address from(ui->lineEdit_DefaultAddress->text().toStdString());
	Function balanceOf("balanceOf");
	balanceOf.addInParam(std::make_shared<ParamAddress>(TW::data(from.toEthCommonAddr().data(), 20)));
	balanceOf.addOutParam(std::make_shared<ParamUInt256>());

	TW::Data outBalanceOfP = deployedContract->Call(balanceOf);//进行Call调用

	std::shared_ptr<ParamBase> param_out_0;
	balanceOf.getOutParam(0, param_out_0);
	dev::u256 balance = std::dynamic_pointer_cast<ParamUInt256>(param_out_0)->getVal();

	double balance_double = cfx_cpp::Drip2CFXDouble(balance);

	QString str = QString("%1").arg(balance_double, 0, 'f', 5);
	ui->lineEdit_TokenBalance->setText(str);


}


void Widget::onBtnSendToken() {
	QString recvAddr = ui->lineEdit_RecvTokenAddr->text();
	int SendTokenCount = ui->lineEdit_SendTokenCount->text().toInt();

	cfx_cpp::Address contractAddr(ui->lineEdit_ContractAddress->text().toStdString());
	auto client = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.com");
	std::shared_ptr<cfx_cpp::Contract> deployedContract = std::make_shared<cfx_cpp::Contract>(client, contractAddr);

	auto privateKey = m_accountManagerPtr->GetSecret(m_DefaultAccount, m_DefaultPasswd.toStdString());
	if (!privateKey) {
		std::cout << "Account Manager GetSecret Error!" << std::endl;
		return;
	}
	else {
		std::cout << "GetSecret OK" << std::endl;
	}


	cfx_cpp::Address transferto(recvAddr.toStdString());
	Function transfer("transfer");
	transfer.addInParam(std::make_shared<ParamAddress>(TW::data(transferto.toEthCommonAddr().data(), 20))); //接收地址
	transfer.addInParam(std::make_shared<ParamUInt256>(cfx_cpp::CFX(SendTokenCount))); //发送的代币数量
	transfer.addOutParam(std::make_shared<ParamBool>()); //返回参数
	try {
		m_lastTokenTransHash = deployedContract->SendTransaction(transfer, privateKey, 0);
		std::cout << "Transfer_TransHash:" << m_lastTokenTransHash << std::endl;
		QMessageBox::information(this, "Information", "Send Token Success!");
	}
	catch (const std::exception& e) {
		std::cout << "transfer error:" << e.what() << std::endl;
		QMessageBox::critical(this, "ERROR", "Send Token Error!");
		return;
	}

	client->WaitTransactionFinished(m_lastTokenTransHash); //wait


	//query balance
	cfx_cpp::Address from(ui->lineEdit_DefaultAddress->text().toStdString());
	Function balanceOf("balanceOf");
	balanceOf.addInParam(std::make_shared<ParamAddress>(TW::data(from.toEthCommonAddr().data(), 20)));
	balanceOf.addOutParam(std::make_shared<ParamUInt256>());

	TW::Data outBalanceOfP = deployedContract->Call(balanceOf);//进行Call调用

	std::shared_ptr<ParamBase> param_out_0;
	balanceOf.getOutParam(0, param_out_0);
	dev::u256 balance = std::dynamic_pointer_cast<ParamUInt256>(param_out_0)->getVal();

	double balance_double = cfx_cpp::Drip2CFXDouble(balance);

	QString str = QString("%1").arg(balance_double, 0, 'f', 5);
	ui->lineEdit_TokenBalance->setText(str);
}
void Widget::ontBtnSendTokenScan() {

	QString URL("https://testnet.confluxscan.io/transaction/" + QString::fromStdString(m_lastTokenTransHash));
	QDesktopServices::openUrl(URL);
}
