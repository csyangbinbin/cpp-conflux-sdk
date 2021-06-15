#include <memory>           //shard_ptr
#include <thread>         // std::thread
#include <chrono>         // std::chrono::seconds
#include <libcfxcore/conflux.h>
#include <libdevcore/CommonJS.h>


using namespace std;

   /*
   ����ʹ�õ��˺�
   ������ַ: cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w
   ��������ַ: cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42
   ��Կ: 9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547
   ��Ӧ��CFX��ʽ��Hex��ַ_160bit: 0x1a3d17aab67a48abb1d39e059b5391b64a91d530
   ��Ӧ��CETH��ʽ��Hex��ַ_160bit: 0x9a3d17aab67a48abb1d39e059b5391b64a91d530
   **��ע:ETH->CFX��ַ�ı仯����:((eth_addr[0]&0x0F)|0x10)
   */
int main() {
    std::string keystoreDir = "c:/ks";
    dev::Secret SecretKey("9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547");

    //�����˻�������
    cfx_cpp::AccountManager account(keystoreDir, cfx_cpp::NetworkType::NetworkTypeTestnetID);
  
    std::string passphrase = "12345678";
    account.ImportPrivateKey(SecretKey, passphrase); //��˽Կ�����˺�
  
   auto client = cfx_cpp::NewHttpsRpcClient("https://test.confluxrpc.com");

    cfx_cpp::Address from("cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42");
    cfx_cpp::Address to("cfxtest:aakd0d2x8zuu7zr3vfm3rwnjyv5rxxuvyj96acpdx8");

    //��ȡ����ǰ���˻����
    double BalanceBefore = cfx_cpp::Drip2CFXDouble(client->cfx_getBalance(from));
    std::cout << "=====> BalanceBefore:" << BalanceBefore << std::endl; 
  

   auto privateKey =  account.GetSecret(from , passphrase); //��ѯ�����˻���Կ�����ڽ���ǩ��
   if (!privateKey) {
       std::cout << "GetSecret ERROR!";
       return 0;
   }
   dev::u256 value = cfx_cpp::CFX("1"); //����1cfx

   cfx_cpp::TransactionBase trans(to , value  , privateKey);

   std::string transHash;
   try {
       transHash = client->sendTransaction(trans);
   }
   catch (jsonrpc::JsonRpcException&e ) {
   std::cout << e.what();
   }
   

  cfx_cpp::TransactionByHashResult	res;
   for (;;) {
	   res = client->cfx_getTransactionByHash(transHash);
	   if (res.status == cfx_cpp::TransactionStatus::SkippedOrNotPacked) {
           this_thread::sleep_for(chrono::seconds(1));
	   }
	   else {
		   break;
	   }
   }
   if (res.IsOK()) {
       std::cout << "sendTransaction OK" << std::endl; 
       std::cout << res.from << " ===> " << res.to << std::endl;
       std::cout << "transHash:" << dev::toJS(res.hash) << std::endl; 
       std::cout << "blockHash:" << dev::toJS(res.blockHash) << std::endl;
       std::cout << "gas:" << dev::toJS(res.gas) << std::endl; 
       std::cout << "value:" << dev::toJS(res.value) << std::endl;
   }
   else {
	   std::cout << "sendTransaction ERROR:" << res.status << std::endl;
   }

   cfx_cpp::TransactionReceipt res1;

   for (;;) {
       try {
           for (;;) {
               res1 = client->cfx_getTransactionReceipt(transHash);
               if (res1.outcomeStatus != 0) {
                   this_thread::sleep_for(chrono::seconds(1));
                   continue;
               }
               else
                   goto DONE;

           }
       }
       catch (const std::exception& e) {
           //std::cout << "exception:" << e.what();
       }
   
   }
 DONE:
  

   //this_thread::sleep_for(chrono::seconds(2));
   double BalanceAfter = cfx_cpp::Drip2CFXDouble(client->cfx_getBalance(from /*,  cfx_cpp::EpochNumber::LatestConfirmed()*/));
   std::cout << "=====> BalanceAfter:" << BalanceAfter << std::endl;

  
   getchar();
}

