#include <libcfxcore/conflux.h>
#include <libdevcrypto/Common.h>
#include <iostream>

int main() {
	/*
   测试使用的账号
   主网地址: cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w
   测试网地址: cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42
   密钥: 9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547
   对应的CFX格式的Hex地址_160bit: 0x1a3d17aab67a48abb1d39e059b5391b64a91d530
   对应的CETH格式的Hex地址_160bit: 0x9a3d17aab67a48abb1d39e059b5391b64a91d530
   **备注:ETH->CFX地址的变化规则:((eth_addr[0]&0x0F)|0x10)
   */

	//兼容CIP-37格式的地址与老式地址，内部通过自动识别两种格式地址
	cfx_cpp::Address Addr("cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w");
	cfx_cpp::Address Addr_hex_format("0x1a3d17aab67a48abb1d39e059b5391b64a91d530" ,cfx_cpp::NetworkType::NetowrkTypeMainnetID); //老式地址需要提供网络类型参数

	//两种格式地址相等
	bool isEqual = (Addr == Addr_hex_format); //true
	std::cout << "Addr Equal : " << (isEqual ? "true" : "false") << std::endl; 

	std::cout << "Base32Address : " << Addr.GetBase32Address() << std::endl;//显示简洁格式地址
	std::cout << "VerboseBase32Address : " << Addr.GetVerboseBase32Address() <<std::endl;//显示详细地址，包含地址类型
	std::cout << "IsValid : " << Addr.IsValid() << std::endl;  //地址是否正确
	std::cout << "NetWorkTypeString : " << Addr.GetNetworkType().GetNetWorkTypeString() << std::endl; //显示网络类型字符串
	std::cout << "NetworkID : " << Addr.GetNetworkType().ToNetworkID() <<std::endl; //显示网络类型数字代码
	std::cout << "AddressType : " << Addr.GetAddressType().Type() << std::endl; //显示地址类型字符串表示
	cfx_cpp::byteArray checksum = Addr.GetChecksum(); //获取地址的checksum数据
	std::string hexAddr = Addr.GetHexAddress();//转换为老式的20字节地址格式
	std::cout << "conflux-hex-address : " << hexAddr << std::endl;

	dev::Address ethAddr = Addr.toEthCommonAddr();//转换为以太坊格式类型的地址
	std::cout << "ETH Addr : " << dev::toJS(ethAddr) <<std::endl ;

	//由私钥计算地址
	dev::Secret privateKey("9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547");
	cfx_cpp::Address  cfx_addr_mainent =  cfx_cpp::Secret2CfxAddr(privateKey ,  cfx_cpp::NetworkType::NetowrkTypeMainnetID); //映射为主网地址
	std::cout << "privateKey -> mainnet cfx_addr : " << cfx_addr_mainent.GetVerboseBase32Address() << std::endl;

	cfx_cpp::Address  cfx_addr_testnet = cfx_cpp::Secret2CfxAddr(privateKey, cfx_cpp::NetworkType::NetworkTypeTestnetID); //映射为测试网地址
	std::cout << "privateKey -> testnent cfx_addr : " << cfx_addr_testnet.GetVerboseBase32Address() << std::endl;

	std::cout << cfx_cpp::Address::TestnetNullAddress().GetVerboseBase32Address() <<std::endl; //静态函数,返回测试网null地址
	std::cout << cfx_cpp::Address::MainnetNullAddress().GetVerboseBase32Address() << std::endl; //静态函数,返回主网null地址

	cfx_cpp::Address contract_addr("cfxtest:acf1tv2wj90bpxrssfp748zh5wmrp88wjevx9bb35p"); 
	std::cout << "Contract AddressType : " << contract_addr.GetAddressType().Type() << std::endl; //显示地址类型字符串表示

	getchar();
	return 0;
}