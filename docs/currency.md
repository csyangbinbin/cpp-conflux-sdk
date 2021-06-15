# Conflux Currency 

To incentivize the maintenance of the Conflux network and charge users for consumption of resources, Conflux has a native currency called the Conflux coin, simply denoted by CFX for short. The smallest subdenomination is denoted by Drip, in which all values processed in Conflux are integers. One Conflux is defined as 10^18 Drip. Frequently used subdenominations of Conflux are listed below:

| Multiplier (in Drip) | Name         |
| -------------------- | ------------ |
| 10^0                 | Drip         |
| 10^9                 | GDrip        |
| 10^12                | uCFX         |
| 10^18                | Conflux(CFX) |

As a native token of the Conflux network, CFX plays a very important role in the stability of the system.

First of all, CFX is used to pay transaction fees and to store collateral. Transactions are packaged and executed by miners. In order to incentivize miners to keep participating in the bookkeeping and provide sufficient security for the network, each transaction is charged a (gas) fee calculated as gasPrice *max(gasLimit* 3/4, gasUsed), which is paid by CFX. In addition, if the execution of a transaction takes up new storage space, a portion of the CFX is pledged for the use of the storage space (the pledged CFX can be released under certain condition).



## sdk function
```cpp
/*
Using typedef cpp_dec_float_50 hides the complexity of multiprecision to allow us to define variables with 50 decimal digit precision just like built-in double.
*/
using DecFloat50 = boost::multiprecision::cpp_dec_float_50;

//High precision integer with 256 Bits
using dev::u256 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;

dev::u256 Drip(dev::u256 value); //build Drip with big integer
dev::u256 Drip(const std::string& value) ;//build Drip with string (e.g. "12345")

dev::u256 GDrip(DecFloat50 value);//build GDrip with big integer
dev::u256 GDrip(const std::string& value);//build GDrip with string 

dev::u256 uCFX(boost::multiprecision::cpp_dec_float_50 value);//build uCFX with big integer
dev::u256 uCFX(const std::string& value) ;//build uCFX with string 

dev::u256 CFX(DecFloat50 value) ;//build CFX with big integer
dev::u256 CFX(const std::string& value) ;//build CFX with string 

double Drip2CFXDouble(dev::u256 value);//get a double number, may be loss of accuracy

dev::u256 operator"" _cfx(const char* v);//C++11 string literal operator
dev::u256 operator"" _ucfx(const char* v);
dev::u256 operator"" _gdrip(const char* v);
dev::u256 operator"" _drip(const char* v);
```

## example
```cpp
dev::u256 cfx_1 = CFX("1.23456789"); //build CFX with string 
dev::u256 cfx_1s = 1.23456789_cfx; //build CFX with string literal

bool equal = (cfx_1 == cfx_1s);  //true

std::cout.precision(18);
//Converting a high precision floating-point number to a double results in a loss of precision
std::cout << "value:" << Drip2CFXDouble(cfx_1) << " cfx" << std::endl; 

bool equa2 = (Drip("1") * dev::exp10<18>() == cfx_1);  //true
bool equa3 = (GDrip("1") * dev::exp10<9>() == cfx_1);  //true
bool equa4 = (uCFX("1") * dev::exp10<6>() == cfx_1);  //true

dev::u256 cfx_flaot = CFX("1.123456789123456789");
dev::u256 ucfx_flaot = uCFX("1123456.789123456789");
dev::u256 gdrip_flaot = GDrip("1123456789.123456789");
dev::u256 drip_flaot = Drip("1123456789123456789");

bool equa5 = (cfx_flaot == ucfx_flaot);  //true
bool equa6 = (cfx_flaot == gdrip_flaot);//true
bool equa7 = (cfx_flaot == drip_flaot);//true

std::cout << cfx_cpp::DecFloat50(cfx_flaot) << std::endl; //get Drip count
std::cout << cfx_flaot << std::endl;
```
