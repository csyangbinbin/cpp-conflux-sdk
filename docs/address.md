# C++ sdk support for CIP37 address

Currently, Conflux and Ethereum addresses are very similar, in many cases they are cross-compatible, i.e. some valid Ethereum addresses are valid Conflux addresses and vice versa. This has led many users to lose their funds when accidentally transferring them to a Conflux address on Ethereum, particularly during cross-chain operations. A new address type that is incompatible with Ethereum would allow tools like Metamask to prevent such erroneous transactions from being submitted to the network.

Apart from the regular type-prefixed hex addresses Conflux uses today, we introduce a new base32-encoded address format. Addresses of the new format are derived from the hex-address directly. They contain an easy-to-recognize prefix (e.g. `"cfx"`), an optional address type, and a checksum. Using base32check encoding helps us avoid and detect typos and makes it easy to disambiguate between Conflux addresses and addresses on other blockchains, thus reducing the risk of loss of funds.

An example of the new address format: `cfx:aarc9abycue0hhzgyrr53m6cxedgccrmmyybjgh4xg`.

------

### CIP37 Address
In order to solve the problems of mistakenly using wrong addresses, we introduces a brand new base32 checksum address format in [CIP37](https://github.com/Conflux-Chain/CIPs/blob/master/CIPs/cip-37.md). Besides checksum, the new addresses also include information such as network, type.

Old address vs new address:

* hex40 address:  0x1386b4185a223ef49592233b69291bbe5a80c527
* base32 address: cfx:aak2rra2njvd77ezwjvx04kkds9fzagfe6ku8scz91

The new addresses use customized base32 code address. Currently applied characters are: `abcdefghjkmnprstuvwxyz0123456789` (i, l, o, q removed).

In new format addresses, network types are included. Up to now there are three types: cfx，cfxtest，net[n]

* cfx:aak2rra2njvd77ezwjvx04kkds9fzagfe6ku8scz91
* cfxtest:aak2rra2njvd77ezwjvx04kkds9fzagfe6d5r8e957
* net1921:aak2rra2njvd77ezwjvx04kkds9fzagfe65k87kwdf

Meanwhile, new addresses also include address type information, currently four types (types are usually in upper case):

* user: CFX:TYPE.USER:AAK2RRA2NJVD77EZWJVX04KKDS9FZAGFE6KU8SCZ91
* contract: CFX:TYPE.CONTRACT:ACB2RRA2NJVD77EZWJVX04KKDS9FZAGFE640XW9UAE
* builtin: CFX:TYPE.BUILTIN:AAEJUAAAAAAAAAAAAAAAAAAAAAAAAAAAAJRWUC9JNB
* null: CFX:TYPE.NULL:AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA0SFBNJM2

The two address formats (hex40 and base32) are convertible to each other. They are the same if converted to byte arrays. However, when converting hex40 addresses (starting with 0x) into base32check addresses, the network ID information is also required.

### Address

 compatible with CIP-37 address and old address, internal automatic recognition of both formats address.
```C++
//CIP-37 address
cfx_cpp::Address Addr("cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w");

//hex40 address(The network type parameter needs to be supplied)
cfx_cpp::Address Addr_hex_format("0x1a3d17aab67a48abb1d39e059b5391b64a91d530",cfx_cpp::NetworkType::NetowrkTypeMainnetID); 
```

##### Instance method

```C++
bool isEqual = (Addr == Addr_hex_format); //Test whether the two addresses are the same in hex40 and base32

//cfx:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w		
Addr.GetBase32Address() //not contain address type.*
    
//cfx:type.user:aard4f7m037evm7v4stang4xwg5ezeszga3rd40n8w
Addr.GetVerboseBase32Address() ;//contain address type.*

Addr.IsValid();  //Is the address available

//cfx
Addr.GetNetworkType().GetNetWorkTypeString() ;//get the network type string

//1029
Addr.GetNetworkType().ToNetworkID(); //get the network type numeric code

//user
Addr.GetAddressType().Type()//get a string representation of the address type
    
cfx_cpp::byteArray checksum = Addr.GetChecksum(); //get checksum

//0x1a3d17aab67a48abb1d39e059b5391b64a91d530
std::string hex40Addr = Addr.GetHexAddress();//convert hex40 address format

//0x1a3d17aab67a48abb1d39e059b5391b64a91d530
dev::Address ethAddr = Addr.toEthCommonAddr();//convert Ethereum address

dev::Secret privateKey("9B9A28DD9D7E89B82FC1E95D9EEFDD3AB7258EF30A1CE3393F56BC555D4FA547"); //address private key

//from private key to conflux mainnet address
cfx_cpp::Address  cfx_addr_mainent =  cfx_cpp::Secret2CfxAddr(privateKey ,  cfx_cpp::NetworkType::NetowrkTypeMainnetID); 

//from private key to conflux mainnet address
cfx_cpp::Address  cfx_addr_testnet = cfx_cpp::Secret2CfxAddr(privateKey, cfx_cpp::NetworkType::NetworkTypeTestnetID);

//type.null:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa6f0vrcsw
cfx_cpp::Address::TestnetNullAddress().GetVerboseBase32Address(); //class static function,return testnet null address

//cfx:type.null:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0sfbnjm2
cfx_cpp::Address::MainnetNullAddress().GetVerboseBase32Address() ; //class static function,return mainnet null address

cfx_cpp::Address contract_addr("cfxtest:acf1tv2wj90bpxrssfp748zh5wmrp88wjevx9bb35p"); // a contract address
contract_addr.GetAddressType().Type() ;//return contract
```

### networkId

When converting hex40 addresses to base32 addresses, networkId is required. The current networkId is the same with the chainId value, 1029 for the mainnet and 1 for the testnet.

```C++
class NetworkType {
public:
    //constructor with string networktype( for example 'cfx','cfxtest','net10')
	NetworkType(const std::string& type_str = "cfxtest"); 
    
    //constructor with integer networktype( for example 1 ,1029)
	NetworkType(uint32_t networkID = 1);
public:
	uint32_t  ToNetworkID() const;//get integer network type
	static uint32_t  getIDWhenBeginWithNet(const std::string& net_type); //Convert from string type to numeric type
	std::string GetNetWorkTypeString() const { return net_work_type_str; } //get string network type
public:
	std::string net_work_type_str;
public:
	static   std::string   NetworkTypeMainnetPrefix;//"cfx"
	static   std::string   NetworkTypeTestNetPrefix;//"cfxtest"
	static   uint32_t NetowrkTypeMainnetID;//1029
	static   uint32_t NetworkTypeTestnetID;//1
};
```

### addressType

Address-type: For the null address (`0x0000000000000000000000000000000000000000`), address-type must be `"type.null"`. Otherwise,

```C++
match addr[0] & 0xf0
        case b00000000: "type.builtin"
        case b00010000: "type.user"
        case b10000000: "type.contract"
```

```C++
class AddressType {
	public:
    	//constructor function with type string(null,builtin,usercontract)
		AddressType(const std::string addr_type_str);
		virtual ~AddressType();
		std::string Type() const;//the original type string does not contain prefix "type".   
		//returns byte represents of address type according to CIP-37, see https://github.com/Conflux-			Chain/CIPs/blob/master/CIPs/cip-37.md
		uint8_t ToByte() const;
		std::string String() const;//get address type string with "type." prefix.
	public:
		static const std::string	AddressTypeBuiltin; //contract
		static const std::string	AddressTypeUser; 	//user
		static const std::string	AddressTypeContract; //builtin
		static const std::string	AddressTypeNull;	//null
		static const std::string	AddressTypeInvalid;	//User-defined, indicating an invalid value
	};

	//gloabl function ,  calculate address type of hexAddress
	AddressType CalcAddressType(const cfx_cpp::byteArray& hexAddress);
```