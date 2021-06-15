#include <stdlib.h>
#include <stdio.h>
#include <libdevcore/CommonJS.h> //dev::jsToXXX
#include <libcfxcore/conflux.h>

int main() {
        /*
        * 真实交易的某log条目
        * 
            "address": "CFXTEST:TYPE.CONTRACT:ACBKW7V7K0AS0J766RBRUNK5CYZTPMCFYYHARWS4M9",
            "data": "0x0000000000000000000000000000000000000000000000000000000000000001",
            "topics": [
                "0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef",
                "0x0000000000000000000000001a3d17aab67a48abb1d39e059b5391b64a91d530",
                "0x0000000000000000000000001a3d17aab67a48abb1d39e059b5391b64a91d530" ]
        */

    //交易对应的logsBloom
    std::string bloomString = "0x00000000010000000000000000000000000000004000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000008008000000000000000000000000000000000000000040000"
        "0000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000040000"
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000020"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    cfx_cpp::LogBloom logsBloom = dev::jsToFixed<256>(bloomString);


    cfx_cpp::Address cfx_address("CFXTEST:TYPE.CONTRACT:ACBKW7V7K0AS0J766RBRUNK5CYZTPMCFYYHARWS4M9");
  
    auto topic_1 = dev::jsToFixed<32>("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef");
    auto topic_2 = dev::jsToFixed<32>("0x0000000000000000000000001a3d17aab67a48abb1d39e059b5391b64a91d530");
    auto topic_3 = dev::jsToFixed<32>("0x0000000000000000000000001a3d17aab67a48abb1d39e059b5391b64a91d530");
    auto topic_error = dev::jsToFixed<32>("0x1100000000000000000000001a3d17aab67a48abb1d39e059b5391b64a91d511");

    cfx_cpp::BloomFilter filter(logsBloom);
    bool is_contains_address = filter.Contains(cfx_address); //true
    bool is_contains_topic_1 = filter.Contains(topic_1);//true
    bool is_contains_topic_2 = filter.Contains(topic_2);//true
    bool is_contains_topic_3 = filter.Contains(topic_3);//true
    bool is_contains_topic_error = filter.Contains(topic_error);//false



    cfx_cpp::BloomFilter another_filter_1;
    another_filter_1.Add(topic_1);
    another_filter_1.Add(topic_2);
    bool is_contains_another_filter_1 = filter.Contains(another_filter_1); //true


    cfx_cpp::BloomFilter another_filter_2;
    another_filter_2.Add(topic_1);
    another_filter_2.Add(topic_2);
    another_filter_2.Add(topic_error);
    bool is_contains_another_filter_2 = filter.Contains(another_filter_2); //false


    cfx_cpp::BloomFilter another_filter_str;
    another_filter_str.Add("123456");
    another_filter_str.Add("ABCDEF");
    another_filter_str.Add("+-*012");

    bool is_contains_str_1 = another_filter_str.Contains("123456"); //true
    bool is_contains_str_2 = another_filter_str.Contains("ABCDEF"); //true
    bool is_contains_str_3 = another_filter_str.Contains("+-*012"); //true
    bool is_contains_str_4 = another_filter_str.Contains("123450"); //false

	return 0;
}
