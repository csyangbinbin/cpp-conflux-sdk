## Contract ABI Specification

The Contract Application Binary Interface (ABI) is the standard way to interact with contracts in the Conflux ecosystem, both from outside the blockchain and for contract-to-contract interaction. [Solidity Official Documentation](https://docs.soliditylang.org/en/v0.8.4/abi-spec.html)

### Solidity - C++ Type Mapping

|                Solidity Type                |                C++ Type                 |
| :-----------------------------------------: | :-------------------------------------: |
|                   address                   |              ParamAddress               |
|         uint8 , uint16 .... uint256         | ParamUInt8 , ParamUInt16 , ParamUInt<N> |
|           int8 , int16 ... int256           |  ParamInt8 , ParamInt16 , ParamInt<N>   |
|                    bool                     |                ParamBool                |
|                    bytes                    |             ParamByteArray              |
|                  bytes<N>                   |            ParamByteArrayFix            |
|                   string                    |               ParamString               |
| Dynamic array of the same types, "<type>[]" |               ParamArray                |
|  Fix array of the same types, "<type>[N]"   |              ParamArrayFix              |

###  Function Selector

The first four bytes of the call data for a function call specifies the function to be called. It is the first (left, high-order in big-endian) four bytes of the Keccak-256 hash of the signature of the function. The signature is defined as the canonical expression of the basic prototype without data location specifier, i.e. the function name with the parenthesised list of parameter types. Parameter types are split by a single comma - no spaces are used.

```c++
class Function {
public:
    Function(std::string name);
    Function(std::string name,const std::vector<std::shared_ptr<ParamBase>>& inParams);
    
    // Add an input parameter. Returns the index of the parameter.
    int addInParam(std::shared_ptr<ParamBase> param) ;
    
    // Add an output parameter. Returns the index of the parameter.
    int addOutParam(std::shared_ptr<ParamBase> param) ;
  
    // Add an input or output parameter. Returns the index of the parameter.
    int addParam(std::shared_ptr<ParamBase> param, bool isOutput = false);
    
    // Get an input parameter.
    bool getInParam(int paramIndex, std::shared_ptr<ParamBase>& param_out);
   
    // Get an output parameter.
    bool getOutParam(int paramIndex, std::shared_ptr<ParamBase>& param_out);
 
    // Get an input or output parameter.
    bool getParam(int paramIndex, std::shared_ptr<ParamBase>& param_out, bool isOutput = false);
    
    // Return the function type signature, of the form "baz(int32,uint256)"
    std::string getType() const;

    // Return the 4-byte function signature
    Data getSignature() const;

    //encode input parameters
    virtual void encode(Data& data) const;
    
    // Decode binary, fill output parameters
    bool decodeOutput(const Data& encoded, size_t& offset_inout);
};
```
### example

```c++
/*
	//A two dimensional array, an array of variable length of dynamic type
	function : g(uint[][],string[]) 
	input parameters : ([[1, 2], [3]], ["one", "two", "three"])
*/
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

func.addInParam(wrapArray);	//add parameters
func.addInParam(stringArray);

std::string type = func.getType(); // g(uint[][],string[])

TW::Data payload;
func.encode(payload);	//encode
```
