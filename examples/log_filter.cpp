#include <libdevcore/CommonJS.h>
#include <libcfxcore/conflux.h>

int main() {
	cfx_cpp::LogFilterParam filter;

	//指定从如下两个block中进行log查询
	dev::h256 blockHash_1 = dev::jsToFixed<32>("0xabbe186ad47a14a60f1d85a7f6cc3cc3f2d79fdd8ed8c6e8e855904ab4384dbe");
	dev::h256 blockHash_2 = dev::jsToFixed<32>("0xb67e86a333fefff451da92dbce59ca6a2de8c8481f841f5bf3af7ee39d2c6277");

	//智能合约地址
	cfx_cpp::Address addr_1("CFX:TYPE.CONTRACT:ACAUCWUZA1NM7WFJ1BWKJTTZ7B0EH4AK7UR7FUE1DY");

	//指定查询的event签名
	dev::h256 topic_0_0 = dev::jsToFixed<32>("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef");

	//第一个主题为 topic_1_0  OR  topic_1_1
	dev::h256 topic_1_0 = dev::jsToFixed<32>("0x0000000000000000000000001607ae8ec007e079c1ed222c9bdb2199f4606f4c");
	dev::h256 topic_1_1 = dev::jsToFixed<32>("0x00000000000000000000000017ecbd854ce7bf7bccfc5e289feb649e1da508f9");

	//查询的epoch的区间
	cfx_cpp::EpochNumber from(dev::jsToU256("0xf8d520"));
	cfx_cpp::EpochNumber to(dev::jsToU256("0xf8d530"));

	filter.SetFromEpoch(from);
	filter.SetToEpoch(to);


	filter.AddBlockHash(blockHash_1)
		.AddBlockHash(blockHash_2);

	filter.AddAddress(addr_1);

	filter.AddTopic(0, topic_0_0)
		.AddTopic(1, topic_1_0)
		.AddTopic(1, topic_1_1);

	filter.SetLimit(10); //设置最多返回的logs条目

	auto mainnetClientPtr = cfx_cpp::NewHttpsRpcClient("https://main.confluxrpc.com");

	cfx_cpp::LocalisedLogEntries entries = mainnetClientPtr->cfx_getLogs(filter);
	for (auto& entry : entries) {
		std::cout << entry << std::endl;
	}

	getchar();
}