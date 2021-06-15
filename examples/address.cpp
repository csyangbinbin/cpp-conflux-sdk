#include <libcfxcore/conflux.h>
#include <libdevcrypto/Common.h>
#include <iostream>

int main() {
	/*
   ����ʹ�õ��˺�
   ������ַ: cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w
   ��������ַ: cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42
   ��Կ: 9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547
   ��Ӧ��CFX��ʽ��Hex��ַ_160bit: 0x1a3d17aab67a48abb1d39e059b5391b64a91d530
   ��Ӧ��CETH��ʽ��Hex��ַ_160bit: 0x9a3d17aab67a48abb1d39e059b5391b64a91d530
   **��ע:ETH->CFX��ַ�ı仯����:((eth_addr[0]&0x0F)|0x10)
   */

	//����CIP-37��ʽ�ĵ�ַ����ʽ��ַ���ڲ�ͨ���Զ�ʶ�����ָ�ʽ��ַ
	cfx_cpp::Address Addr("cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w");
	cfx_cpp::Address Addr_hex_format("0x1a3d17aab67a48abb1d39e059b5391b64a91d530" ,cfx_cpp::NetworkType::NetowrkTypeMainnetID); //��ʽ��ַ��Ҫ�ṩ�������Ͳ���

	//���ָ�ʽ��ַ���
	bool isEqual = (Addr == Addr_hex_format); //true
	std::cout << "Addr Equal : " << (isEqual ? "true" : "false") << std::endl; 

	std::cout << "Base32Address : " << Addr.GetBase32Address() << std::endl;//��ʾ����ʽ��ַ
	std::cout << "VerboseBase32Address : " << Addr.GetVerboseBase32Address() <<std::endl;//��ʾ��ϸ��ַ��������ַ����
	std::cout << "IsValid : " << Addr.IsValid() << std::endl;  //��ַ�Ƿ���ȷ
	std::cout << "NetWorkTypeString : " << Addr.GetNetworkType().GetNetWorkTypeString() << std::endl; //��ʾ���������ַ���
	std::cout << "NetworkID : " << Addr.GetNetworkType().ToNetworkID() <<std::endl; //��ʾ�����������ִ���
	std::cout << "AddressType : " << Addr.GetAddressType().Type() << std::endl; //��ʾ��ַ�����ַ�����ʾ
	cfx_cpp::byteArray checksum = Addr.GetChecksum(); //��ȡ��ַ��checksum����
	std::string hexAddr = Addr.GetHexAddress();//ת��Ϊ��ʽ��20�ֽڵ�ַ��ʽ
	std::cout << "conflux-hex-address : " << hexAddr << std::endl;

	dev::Address ethAddr = Addr.toEthCommonAddr();//ת��Ϊ��̫����ʽ���͵ĵ�ַ
	std::cout << "ETH Addr : " << dev::toJS(ethAddr) <<std::endl ;

	//��˽Կ�����ַ
	dev::Secret privateKey("9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547");
	cfx_cpp::Address  cfx_addr_mainent =  cfx_cpp::Secret2CfxAddr(privateKey ,  cfx_cpp::NetworkType::NetowrkTypeMainnetID); //ӳ��Ϊ������ַ
	std::cout << "privateKey -> mainnet cfx_addr : " << cfx_addr_mainent.GetVerboseBase32Address() << std::endl;

	cfx_cpp::Address  cfx_addr_testnet = cfx_cpp::Secret2CfxAddr(privateKey, cfx_cpp::NetworkType::NetworkTypeTestnetID); //ӳ��Ϊ��������ַ
	std::cout << "privateKey -> testnent cfx_addr : " << cfx_addr_testnet.GetVerboseBase32Address() << std::endl;

	std::cout << cfx_cpp::Address::TestnetNullAddress().GetVerboseBase32Address() <<std::endl; //��̬����,���ز�����null��ַ
	std::cout << cfx_cpp::Address::MainnetNullAddress().GetVerboseBase32Address() << std::endl; //��̬����,��������null��ַ

	cfx_cpp::Address contract_addr("cfxtest:acf1tv2wj90bpxrssfp748zh5wmrp88wjevx9bb35p"); 
	std::cout << "Contract AddressType : " << contract_addr.GetAddressType().Type() << std::endl; //��ʾ��ַ�����ַ�����ʾ

	getchar();
	return 0;
}