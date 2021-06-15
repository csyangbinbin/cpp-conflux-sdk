#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <libcfxcore/conflux.h>

using namespace std;

/*
   *����ʹ�õ��˺�
   ������ַ: cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w
   ��������ַ: cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42
   ��Կ: 9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547
   ��Ӧ��CFX��ʽ��Hex��ַ_160bit: 0x1a3d17aab67a48abb1d39e059b5391b64a91d530
   ��Ӧ��CETH��ʽ��Hex��ַ_160bit: 0x9a3d17aab67a48abb1d39e059b5391b64a91d530
   **��ע:ETH->CFX��ַ�ı仯����:((eth_addr[0]&0x0F)|0x10)
   *
   */
int main() {
    std::string keystoreDir = "c:/ks";
    std::string SecretKeyString = "9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547";
    dev::Secret SecretKey(SecretKeyString);

    cfx_cpp::AccountManager account(keystoreDir, cfx_cpp::NetworkType::NetworkTypeTestnetID);
    bool ok = false;
    account.ImportPrivateKey(dev::Secret(SecretKeyString), "123456"); //��˽Կ�����˺�

    //��ӡ���е��˺�
    auto acclist = account.List();
    for (auto const& c : acclist) {
        std::cout << " LIST:" << c << std::endl;
    }

    //��ȡ�˺Ŷ�Ӧ��˽Կ
    std::string mainnetAddr = "cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42";
    std::string passphrase = "123456";
    dev::Secret key = account.GetSecret(cfx_cpp::Address(mainnetAddr), passphrase);
    if (key) {
        std::cout << "account.GetSecret Success";
    }
    else {
        std::cout << "account.GetSecret Error";
    }

    //�����˺Ŷ�Ӧ������
    ok = account.UpdatePassphrase(cfx_cpp::Address(mainnetAddr), "123456", "12345678");
    assert(ok && "UpdatePassphrase");

    //ɾ���˺�
    ok = account.Delete(cfx_cpp::Address(mainnetAddr), "12345678");
    assert(ok && "Delete");

    getchar();
    return 0;
}

