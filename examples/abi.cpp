#include<libcfxcore/conflux.h>
#include <cassert>
using namespace TW::Ethereum::ABI;

void TestCase1();
void TestCase2();
void TestCase3();
void TestCase4();
void TestCase5();
void TestCase6();
void TestCase7();
void TestCase8();

int main() {
	/*
	* 测试用例来源：https://learnblockchain.cn/docs/solidity/abi-spec.html
	*/

	TestCase1();
	TestCase2();
	TestCase3();
	TestCase4();
	TestCase5();
	TestCase6();
	TestCase7();
	TestCase8();

	std::cout << "ALL PASS" << std::endl;
	getchar();
	return 0;
}

void TestCase1() {
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
	assert(str == expect && "baz(uint32,bool)");

}
void TestCase2() {
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
	assert(str == expect && " function storestr(string[2] memory num) public");
}
void TestCase3() {
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
	assert(str == expect && " function bar(bytes3[2]) public");
}

void TestCase4() {
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
	assert(expect == str && "sam(bytes,bool,uint256[])");
}

void TestCase5() {
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
	assert(expect == str && " f(uint256,uint32[],bytes10,bytes) ");
	int a = 0;
}

void TestCase6() {
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
	assert(str == expect && "g(uint[][],string[])");
}


void TestCase7() {
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
	assert(str == expect && "function storestr(string[2] memory num ,uint256[2] memory a1 ,string[2] memory num2)");
}

void TestCase8() {
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
	assert(str == expect && "function storestr(string[2] memory num ,string[] memory a3 ,uint256[2] memory a1 ,string[2] memory num2) public");
}