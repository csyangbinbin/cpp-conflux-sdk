## Manage Accounts

use **AccountManager** manage accounts at local machine.

- Create an new account with passphras protection
- Import an account from Keystore or PrivateKey 
- Update an account passphras
- Delete an account 
- List all accounts.

### AccountManager Class

```cpp
class AccountManager {
public:
    /*
    constructor with the specified networkID
    With parameters path , the account will be saved in the local file system( as keystore file).
    without parameters path ,the account will in memory ,it will lost after app close
    */
	AccountManager(uint32_t _networkID);
	AccountManager(boost::filesystem::path const& _path, uint32_t _networkID);
	virtual ~AccountManager();
public:
    //create an account and protect it with a password
	cfx_cpp::Address Create(const std::string& passphrase);
    
    //list all account
	std::vector<cfx_cpp::Address> List() const;
    
    //import an account from keystore file
	cfx_cpp::Address ImportKeystore(const std::string& keystore_file, bool* ok = NULL);
     //import an account from keystore file (overload)
	cfx_cpp::Address ImportKeystore(boost::filesystem::path keystore_file_path, bool* ok = NULL);
    
    //import an account from private key
	cfx_cpp::Address ImportPrivateKey(dev::Secret secret_key, const std::string& passphrase);
    
    //update passphras
	bool UpdatePassphrase(cfx_cpp::Address addr, const std::string& passphrase, const std::string& newPassphrase);
    
    //get the privateKey of specified address
	dev::Secret GetSecret(cfx_cpp::Address addr, const std::string& passphrase);
    
    //delete an account
	bool Delete(cfx_cpp::Address addr, const std::string& passphrase);
    
    //get the fist account
    cfx_cpp::Address GetDefault() const;
};
```

### example

```cpp
std::string keystoreDir = "C:/Keystores"; 
std::string SecretKeyString = "9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547";
dev::Secret SecretKey(SecretKeyString);

//new an AccountManager instance
cfx_cpp::AccountManager account(keystoreDir, cfx_cpp::NetworkType::NetworkTypeTestnetID);

//import from private with passphrase
account.ImportPrivateKey(dev::Secret(SecretKeyString), "123456"); 

//print account list
auto acclist = account.List();
for (auto const& c : acclist)
   std::cout << "Account:" << c << std::endl;

//get the privateKey of specified address
std::string mainnetAddr = "cfxtest:aard4f7m037evm7v4stang4xwg5ezeszgaxgumub42";
std::string passphrase = "123456";
dev::Secret key = account.GetSecret(cfx_cpp::Address(mainnetAddr), passphrase);
if (key) {
  std::cout << "account.GetSecret Success";
else {
  std::cout << "account.GetSecret Error";

//update account passphrase
ok = account.UpdatePassphrase(cfx_cpp::Address(mainnetAddr), "123456", "12345678");

//delete an account
ok = account.Delete(cfx_cpp::Address(mainnetAddr), "12345678");
```