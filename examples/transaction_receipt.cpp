#include <libcfxcore/conflux.h>

int main() {

	try
	{
		auto mainnetClientPtr = cfx_cpp::NewHttpsRpcClient("https://main.confluxrpc.com");
		std::string transactionHash = "0x64c7af98a1739c12fc9add32f2359b91a6d97df88067546c2cf2f8a7f9bcdef9";
		cfx_cpp::TransactionReceipt receipt_1 = mainnetClientPtr->cfx_getTransactionReceipt(transactionHash);
		std::cout << receipt_1 << std::endl;

		//测试logsBloom，每个log都应在bloom中有对应的位
		for (size_t i = 0; i < receipt_1.logs.size(); i++) {
			cfx_cpp::LogBloom  logBloom = receipt_1.logs.at(i).bloom();
			cfx_cpp::BloomFilter filter(receipt_1.logsBloom);
			bool is_contains = filter.Contains(logBloom);
			assert(is_contains && "logsBloom is not contains the log!");
		}

		auto testnetClientPtr = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.com");
		std::string transactionHash2 = "0x4c767a55ed47c19790b3734b78a5c3ad94e6b838a643e830bd85c2ee732d0288";
		cfx_cpp::TransactionReceipt receipt_2 = testnetClientPtr->cfx_getTransactionReceipt(transactionHash2);
		std::cout << receipt_2 << std::endl;

	}
	catch (const jsonrpc::JsonRpcException& e)
	{
		std::cout << "JsonRpcException:" << e.what();
	}


	getchar();

}