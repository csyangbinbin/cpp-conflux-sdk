#include <libcfxcore/conflux.h>
#include <gtest/gtest.h>

using namespace TW::Ethereum::ABI;

TEST(ConfluxCppSdkTest, Address_Base) {
	std::string base32AddrStr = "cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w";
	std::string hex40AddrStr = "0x1a3d17aab67a48abb1d39e059b5391b64a91d530";
	cfx_cpp::Address Addr(base32AddrStr);
	cfx_cpp::Address Addr_hex_format(hex40AddrStr, cfx_cpp::NetworkType::NetowrkTypeMainnetID);
	EXPECT_EQ(Addr, Addr_hex_format);
	EXPECT_STREQ(Addr.GetBase32Address().c_str(), base32AddrStr.c_str());
	EXPECT_STREQ(Addr.GetVerboseBase32Address().c_str(), "cfx:type.user:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w");
	EXPECT_TRUE(Addr.IsValid());
	EXPECT_STREQ(Addr.GetNetworkType().GetNetWorkTypeString().c_str(), "cfx");
	EXPECT_EQ(Addr.GetNetworkType().ToNetworkID(), 1029);
	EXPECT_STREQ(Addr.GetAddressType().Type().c_str(), "user");
	EXPECT_STREQ(Addr.GetHexAddress().c_str(), "0x1a3d17aab67a48abb1d39e059b5391b64a91d530");
	EXPECT_STREQ(dev::toJS(Addr.toEthCommonAddr()).c_str(), "0x1a3d17aab67a48abb1d39e059b5391b64a91d530");
	cfx_cpp::Address contract_addr("cfxtest:acf1tv2wj90bpxrssfp748zh5wmrp88wjevx9bb35p");
	EXPECT_STREQ(contract_addr.GetAddressType().Type().c_str(), "contract");
}

TEST(ConfluxCppSdkTest, Address_FromPK) {
	dev::Secret privateKey("9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547");
	cfx_cpp::Address  cfx_addr_mainent = cfx_cpp::Secret2CfxAddr(privateKey, cfx_cpp::NetworkType::NetowrkTypeMainnetID); //映射为主网地址
	EXPECT_STREQ(cfx_addr_mainent.GetVerboseBase32Address().c_str(), "cfx:type.user:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w");

	cfx_cpp::Address  cfx_addr_testnet = cfx_cpp::Secret2CfxAddr(privateKey, cfx_cpp::NetworkType::NetworkTypeTestnetID);
	EXPECT_STREQ(cfx_addr_testnet.GetVerboseBase32Address().c_str(), "cfxtest:type.user:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42");
}

TEST(ConfluxCppSdkTest, Address_NULL) {

	EXPECT_STREQ(cfx_cpp::Address::TestnetNullAddress().GetVerboseBase32Address().c_str(), "cfxtest:type.null:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa6f0vrcsw");
	EXPECT_STREQ(cfx_cpp::Address::MainnetNullAddress().GetVerboseBase32Address().c_str(), "cfx:type.null:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0sfbnjm2");
}

TEST(ConfluxCppSdkTest, ABI_1) {
	/*
	function baz(uint32 x, bool y) public pure returns(bool r) { r = x > 32 || y; } //普通值参数
	*/
	std::string expect = "cdcd77c"
		"00000000000000000000000000000000000000000000000000000000000000045"
		"0000000000000000000000000000000000000000000000000000000000000001";

	uint32_t x = 69;
	bool y = true;
	auto func = TW::Ethereum::ABI::Function("baz");
	func.addInParam(std::make_shared<ParamUInt32>(x));
	func.addInParam(std::make_shared<ParamBool>(y));
	TW::Data payload;
	func.encode(payload);
	std::string str = TW::dumpHex(payload);
	EXPECT_STREQ(str.c_str(), expect.c_str());
}

TEST(ConfluxCppSdkTest, ABI_2) {
	/*
	 function storestr(string[2] memory num) public   动态类型T的定长数组
	 参数:["012345678901234567890123456789012345","0123"]
	*/

	std::string expect = "74292993"
		"0000000000000000000000000000000000000000000000000000000000000020"
		"0000000000000000000000000000000000000000000000000000000000000040"
		"00000000000000000000000000000000000000000000000000000000000000a0"
		"0000000000000000000000000000000000000000000000000000000000000024"
		"3031323334353637383930313233343536373839303132333435363738393031"
		"3233343500000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000004"
		"3031323300000000000000000000000000000000000000000000000000000000";


	auto func = TW::Ethereum::ABI::Function("storestr");
	std::shared_ptr<ParamArrayFix> strArrayFix = std::make_shared<ParamArrayFix>();
	strArrayFix->addParam(std::make_shared<ParamString>("012345678901234567890123456789012345"));
	strArrayFix->addParam(std::make_shared<ParamString>("0123"));
	func.addInParam(strArrayFix);
	TW::Data payload;
	func.encode(payload);
	std::string str = TW::dumpHex(payload);
	EXPECT_STREQ(str.c_str(), expect.c_str());
}

TEST(ConfluxCppSdkTest, ABI_3) {
	/*
		  function bar(bytes3[2]) public pure {} //静态类型T的定长数组
		  参数:["abc","def"]
		*/
	std::string expect = "fce353f6"
		"6162630000000000000000000000000000000000000000000000000000000000"
		"6465660000000000000000000000000000000000000000000000000000000000";

	auto func = TW::Ethereum::ABI::Function("bar");
	std::shared_ptr<ParamArrayFix> strArrayFix = std::make_shared<ParamArrayFix>();
	std::string p1 = "abc";
	std::string p2 = "def";
	strArrayFix->addParam(std::make_shared<ParamByteArrayFix>(3, TW::data(p1)));
	strArrayFix->addParam(std::make_shared<ParamByteArrayFix>(3, TW::data(p2)));
	func.addInParam(strArrayFix);

	std::string type = func.getType();
	TW::Data payload;
	func.encode(payload);
	std::string str = TW::dumpHex(payload);
	EXPECT_STREQ(str.c_str(), expect.c_str());
}

TEST(ConfluxCppSdkTest, ABI_4) {
	/*
		sam(bytes,bool,uint256[]) //变长数组
	   参数:"dave"、true 和 [1,2,3]
		*/

	std::string expect = "a5643bf2"
		"0000000000000000000000000000000000000000000000000000000000000060"
		"0000000000000000000000000000000000000000000000000000000000000001"
		"00000000000000000000000000000000000000000000000000000000000000a0"
		"0000000000000000000000000000000000000000000000000000000000000004"
		"6461766100000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"0000000000000000000000000000000000000000000000000000000000000001"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"0000000000000000000000000000000000000000000000000000000000000003";

	auto func = TW::Ethereum::ABI::Function("sam");
	func.addInParam(std::make_shared<ParamByteArray>(TW::data("dava")));
	func.addInParam(std::make_shared<ParamBool>(true));

	auto arr = std::make_shared<ParamArray>();
	arr->addParam(std::make_shared<ParamUInt256>(1));
	arr->addParam(std::make_shared<ParamUInt256>(2));
	arr->addParam(std::make_shared<ParamUInt256>(3));
	func.addInParam(arr);

	std::string type = func.getType();
	TW::Data payload;
	func.encode(payload);
	std::string str = TW::dumpHex(payload);
	EXPECT_STREQ(str.c_str(), expect.c_str());
}

TEST(ConfluxCppSdkTest, ABI_5) {
	/*
	   f(uint256,uint32[],bytes10,bytes)  //变长数组，动态类型
	   参数:(0x123, [0x456, 0x789], "1234567890", "Hello, world!")
		*/

	std::string expect = "8be65246"
		"0000000000000000000000000000000000000000000000000000000000000123"
		"0000000000000000000000000000000000000000000000000000000000000080"
		"3132333435363738393000000000000000000000000000000000000000000000"
		"00000000000000000000000000000000000000000000000000000000000000e0"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"0000000000000000000000000000000000000000000000000000000000000456"
		"0000000000000000000000000000000000000000000000000000000000000789"
		"000000000000000000000000000000000000000000000000000000000000000d"
		"48656c6c6f2c20776f726c642100000000000000000000000000000000000000";

	auto func = TW::Ethereum::ABI::Function("f");
	func.addInParam(std::make_shared<ParamUInt>(0x123));

	auto param2 = std::make_shared<ParamArray>();
	param2->addParam(std::make_shared<ParamUInt32>(0x456));
	param2->addParam(std::make_shared<ParamUInt32>(0x789));
	func.addInParam(param2);

	func.addInParam(std::make_shared<ParamByteArrayFix>(10, TW::data("1234567890")));
	func.addInParam(std::make_shared<ParamByteArray>(TW::data("Hello, world!")));

	std::string type = func.getType();
	TW::Data payload;
	func.encode(payload);
	std::string str = TW::dumpHex(payload);
	EXPECT_STREQ(str.c_str(), expect.c_str());
}

TEST(ConfluxCppSdkTest, ABI_6) {
	/*
		g(uint[][],string[])       //二维数组 ,动态类型的不定长数组
		参数:([[1, 2], [3]], ["one", "two", "three"])
		*/
	std::string expect = "2289b18c"
		"0000000000000000000000000000000000000000000000000000000000000040"
		"0000000000000000000000000000000000000000000000000000000000000140"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"0000000000000000000000000000000000000000000000000000000000000040"
		"00000000000000000000000000000000000000000000000000000000000000a0"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"0000000000000000000000000000000000000000000000000000000000000001"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"0000000000000000000000000000000000000000000000000000000000000001"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"0000000000000000000000000000000000000000000000000000000000000060"
		"00000000000000000000000000000000000000000000000000000000000000a0"
		"00000000000000000000000000000000000000000000000000000000000000e0"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"6f6e650000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"74776f0000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000005"
		"7468726565000000000000000000000000000000000000000000000000000000";

	auto func = TW::Ethereum::ABI::Function("g");

	std::shared_ptr<ParamArray> array1 = std::make_shared<ParamArray>(
		std::vector<std::shared_ptr<ParamBase> >{
		std::make_shared<ParamUInt256>(1),
			std::make_shared<ParamUInt256>(2) });

	std::shared_ptr<ParamArray> array2 = std::make_shared<ParamArray>(
		std::vector<std::shared_ptr<ParamBase> > {
		std::make_shared<ParamUInt256>(3) });

	std::shared_ptr<ParamArray> wrapArray = std::make_shared<ParamArray>();
	wrapArray->addParam(array1);
	wrapArray->addParam(array2);

	std::shared_ptr<ParamArray> stringArray = std::make_shared<ParamArray>();
	stringArray->addParam(std::make_shared<ParamString>("one"));
	stringArray->addParam(std::make_shared<ParamString>("two"));
	stringArray->addParam(std::make_shared<ParamString>("three"));

	func.addInParam(wrapArray);
	func.addInParam(stringArray);
	std::string type = func.getType();
	TW::Data payload;
	func.encode(payload);
	std::string str = TW::dumpHex(payload);
	EXPECT_STREQ(str.c_str(), expect.c_str());
}

TEST(ConfluxCppSdkTest, ABI_7) {
	/*
	function storestr(string[2] memory num ,uint256[2] memory a1 ,string[2] memory num2) 混合动态类型定长数组与动态类型的定长数组
	参数:["123","456"],[1,2],["345","678"]
	*/


	std::string expect = "05eca0ce"
		"0000000000000000000000000000000000000000000000000000000000000080"
		"0000000000000000000000000000000000000000000000000000000000000001"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"0000000000000000000000000000000000000000000000000000000000000140"
		"0000000000000000000000000000000000000000000000000000000000000040"
		"0000000000000000000000000000000000000000000000000000000000000080"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"3132330000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"3435360000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000040"
		"0000000000000000000000000000000000000000000000000000000000000080"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"3334350000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"3637380000000000000000000000000000000000000000000000000000000000";

	auto func = TW::Ethereum::ABI::Function("storestr");
	std::shared_ptr<ParamArrayFix> strArrayFix_1 = std::make_shared<ParamArrayFix>();
	strArrayFix_1->addParam(std::make_shared<ParamString>("123"));
	strArrayFix_1->addParam(std::make_shared<ParamString>("456"));

	std::shared_ptr<ParamArrayFix> strArrayFix_2 = std::make_shared<ParamArrayFix>();
	strArrayFix_2->addParam(std::make_shared<ParamUInt256>(1));
	strArrayFix_2->addParam(std::make_shared<ParamUInt256>(2));

	std::shared_ptr<ParamArrayFix> strArrayFix_3 = std::make_shared<ParamArrayFix>();
	strArrayFix_3->addParam(std::make_shared<ParamString>("345"));
	strArrayFix_3->addParam(std::make_shared<ParamString>("678"));
	func.addInParam(strArrayFix_1);
	func.addInParam(strArrayFix_2);
	func.addInParam(strArrayFix_3);
	auto type = func.getType();
	TW::Data payload;
	func.encode(payload);
	std::string str = TW::dumpHex(payload);
	EXPECT_STREQ(str.c_str(), expect.c_str());
}


TEST(ConfluxCppSdkTest, ABI_8) {
	/*
	// function storestr(string[2] memory num ,string[] memory a3 ,uint256[2] memory a1 ,string[2] memory num2) public
	//["123","456"],["01","02","03"],[1,2],["a","b"]
	*/

	std::string expect = "ba901134"
		"00000000000000000000000000000000000000000000000000000000000000a0"
		"0000000000000000000000000000000000000000000000000000000000000160"
		"0000000000000000000000000000000000000000000000000000000000000001"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"00000000000000000000000000000000000000000000000000000000000002a0"
		"0000000000000000000000000000000000000000000000000000000000000040"
		"0000000000000000000000000000000000000000000000000000000000000080"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"3132330000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"3435360000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000003"
		"0000000000000000000000000000000000000000000000000000000000000060"
		"00000000000000000000000000000000000000000000000000000000000000a0"
		"00000000000000000000000000000000000000000000000000000000000000e0"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"3031000000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"3032000000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000002"
		"3033000000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000040"
		"0000000000000000000000000000000000000000000000000000000000000080"
		"0000000000000000000000000000000000000000000000000000000000000001"
		"6100000000000000000000000000000000000000000000000000000000000000"
		"0000000000000000000000000000000000000000000000000000000000000001"
		"6200000000000000000000000000000000000000000000000000000000000000";


	auto func = TW::Ethereum::ABI::Function("storestr");
	std::shared_ptr<ParamArrayFix> strArrayFix_1 = std::make_shared<ParamArrayFix>();
	strArrayFix_1->addParam(std::make_shared<ParamString>("123"));
	strArrayFix_1->addParam(std::make_shared<ParamString>("456"));


	std::shared_ptr<ParamArray> strArrayFix_2 = std::make_shared<ParamArray>();
	strArrayFix_2->addParam(std::make_shared<ParamString>("01"));
	strArrayFix_2->addParam(std::make_shared<ParamString>("02"));
	strArrayFix_2->addParam(std::make_shared<ParamString>("03"));

	std::shared_ptr<ParamArrayFix> strArrayFix_3 = std::make_shared<ParamArrayFix>();
	strArrayFix_3->addParam(std::make_shared<ParamUInt256>(1));
	strArrayFix_3->addParam(std::make_shared<ParamUInt256>(2));

	std::shared_ptr<ParamArrayFix> strArrayFix_4 = std::make_shared<ParamArrayFix>();
	strArrayFix_4->addParam(std::make_shared<ParamString>("a"));
	strArrayFix_4->addParam(std::make_shared<ParamString>("b"));
	func.addInParam(strArrayFix_1);
	func.addInParam(strArrayFix_2);
	func.addInParam(strArrayFix_3);
	func.addInParam(strArrayFix_4);
	auto type = func.getType();
	TW::Data payload;
	func.encode(payload);
	std::string str = TW::dumpHex(payload);
	EXPECT_STREQ(str.c_str(), expect.c_str());
}


TEST(ConfluxCppSdkTest, BloomFilter) {
	std::string bloomString = "0x00000000010000000000000000000000000000004000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000008008000000000000000000000000000000000000000040000"
		"0000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000040000"
		"00000000000000000000000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000020"
		"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	cfx_cpp::LogBloom logsBloom = dev::jsToFixed<256>(bloomString);


	cfx_cpp::Address cfx_address("CFXTEST:TYPE.CONTRACT:ACBKW7V7K0AS0J766RBRUNK5CYZTPMCFYYHARWS4M9");

	auto topic_1 = dev::jsToFixed<32>("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef");
	auto topic_2 = dev::jsToFixed<32>("0x0000000000000000000000001a3d17aab67a48abb1d39e059b5391b64a91d530");
	auto topic_3 = dev::jsToFixed<32>("0x0000000000000000000000001a3d17aab67a48abb1d39e059b5391b64a91d530");
	auto topic_error = dev::jsToFixed<32>("0x1100000000000000000000001a3d17aab67a48abb1d39e059b5391b64a91d511");

	cfx_cpp::BloomFilter filter(logsBloom);
	EXPECT_TRUE(filter.Contains(cfx_address)); //true
	EXPECT_TRUE(filter.Contains(topic_1));//true
	EXPECT_TRUE(filter.Contains(topic_2));//true
	EXPECT_TRUE(filter.Contains(topic_3));//true
	EXPECT_FALSE(filter.Contains(topic_error));//false


	cfx_cpp::BloomFilter another_filter_1;
	another_filter_1.Add(topic_1);
	another_filter_1.Add(topic_2);
	EXPECT_TRUE(filter.Contains(another_filter_1)); //true


	cfx_cpp::BloomFilter another_filter_2;
	another_filter_2.Add(topic_1);
	another_filter_2.Add(topic_2);
	another_filter_2.Add(topic_error);
	EXPECT_FALSE(filter.Contains(another_filter_2)); //false


	cfx_cpp::BloomFilter another_filter_str;
	another_filter_str.Add("123456");
	another_filter_str.Add("ABCDEF");
	another_filter_str.Add("+-*012");

	EXPECT_TRUE(another_filter_str.Contains("123456")); //true
	EXPECT_TRUE(another_filter_str.Contains("ABCDEF")); //true
	EXPECT_TRUE(another_filter_str.Contains("+-*012")); //true
	EXPECT_FALSE(another_filter_str.Contains("123450")); //false
}


TEST(ConfluxCppSdkTest, Account) {
	std::string SecretKeyString = "9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547";
	dev::Secret SecretKey(SecretKeyString);
	std::string base32Addr = "cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42";
	std::string passphrase = "12345678";

	cfx_cpp::AccountManager account(cfx_cpp::NetworkType::NetworkTypeTestnetID);

	cfx_cpp::Address addr = account.ImportPrivateKey(dev::Secret(SecretKeyString), passphrase);
	EXPECT_STREQ(addr.GetBase32Address().c_str(), base32Addr.c_str());
	dev::Secret key = account.GetSecret(cfx_cpp::Address(base32Addr), passphrase);
	EXPECT_EQ(SecretKey, key);

	bool  isOK = account.UpdatePassphrase(cfx_cpp::Address(base32Addr), passphrase, "12345678NEW");
	EXPECT_TRUE(isOK);

	dev::Secret keyNew = account.GetSecret(cfx_cpp::Address(base32Addr), "12345678NEW");
	EXPECT_EQ(SecretKey, keyNew);

	isOK = account.Delete(cfx_cpp::Address(base32Addr), "12345678NEW");
	EXPECT_TRUE(isOK);

	EXPECT_TRUE(account.List().size() == 0);
}

TEST(ConfluxCppSdkTest, Currency) {
	dev::u256 cfx_1 = cfx_cpp::CFX("1.23456789");
	dev::u256 cfx_1s = 1.23456789_cfx;

	EXPECT_EQ(cfx_1, cfx_1s);  //true

	dev::u256 one_cfx = cfx_cpp::CFX("1.0");
	EXPECT_TRUE(cfx_cpp::Drip("1") * dev::exp10<18>() == one_cfx);  //true
	EXPECT_TRUE(cfx_cpp::GDrip("1") * dev::exp10<9>() == one_cfx);  //true
	EXPECT_TRUE(cfx_cpp::uCFX("1") * dev::exp10<6>() == one_cfx);  //true

	dev::u256 cfx_flaot = cfx_cpp::CFX("1.123456789123456789");
	dev::u256 ucfx_flaot = cfx_cpp::uCFX("1123456.789123456789");
	dev::u256 gdrip_flaot = cfx_cpp::GDrip("1123456789.123456789");
	dev::u256 drip_flaot = cfx_cpp::Drip("1123456789123456789");

	EXPECT_EQ(cfx_flaot, ucfx_flaot);  //true
	EXPECT_EQ(cfx_flaot, gdrip_flaot);//true
	EXPECT_EQ(cfx_flaot, drip_flaot);//true
}



