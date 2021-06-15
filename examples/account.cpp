#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <libcfxcore/conflux.h>

using namespace std;

/*
   *测试使用的账号
   主网地址: cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w
   测试网地址: cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42
   密钥: 9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547
   对应的CFX格式的Hex地址_160bit: 0x1a3d17aab67a48abb1d39e059b5391b64a91d530
   对应的CETH格式的Hex地址_160bit: 0x9a3d17aab67a48abb1d39e059b5391b64a91d530
   **备注:ETH->CFX地址的变化规则:((eth_addr[0]&0x0F)|0x10)
   *
   */
int main() {
    std::string keystoreDir = "c:/ks";
    std::string SecretKeyString = "9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547";
    dev::Secret SecretKey(SecretKeyString);

    cfx_cpp::AccountManager account(keystoreDir, cfx_cpp::NetworkType::NetworkTypeTestnetID);
    bool ok = false;
    account.ImportPrivateKey(dev::Secret(SecretKeyString), "123456"); //从私钥导入账号

    //打印所有的账号
    auto acclist = account.List();
    for (auto const& c : acclist) {
        std::cout << " LIST:" << c << std::endl;
    }

    //获取账号对应的私钥
    std::string mainnetAddr = "cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42";
    std::string passphrase = "123456";
    dev::Secret key = account.GetSecret(cfx_cpp::Address(mainnetAddr), passphrase);
    if (key) {
        std::cout << "account.GetSecret Success";
    }
    else {
        std::cout << "account.GetSecret Error";
    }

    //更新账号对应的密码
    ok = account.UpdatePassphrase(cfx_cpp::Address(mainnetAddr), "123456", "12345678");
    assert(ok && "UpdatePassphrase");

    //删除账号
    ok = account.Delete(cfx_cpp::Address(mainnetAddr), "12345678");
    assert(ok && "Delete");

    getchar();
    return 0;
}

