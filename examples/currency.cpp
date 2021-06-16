#include <libcfxcore/conflux.h>
#include <iostream>
using namespace cfx_cpp;

int main() {

	dev::u256 cfx_1 = CFX("1.23456789");
	dev::u256 cfx_1s = 1.23456789_cfx;

	bool equal = (cfx_1 == cfx_1s);  //true

	std::cout.precision(18);
	std::cout << "value:" << Drip2CFXDouble(cfx_1) << " cfx" << std::endl; //boost高精度浮点数转换为double会有精度损失

	dev::u256 one_cfx = cfx_cpp::CFX("1.0");
	bool equa2 = (Drip("1") * dev::exp10<18>() == one_cfx);  //true
	bool equa3 = (GDrip("1") * dev::exp10<9>() == one_cfx);  //true
	bool equa4 = (uCFX("1") * dev::exp10<6>() == one_cfx);  //true

	dev::u256 cfx_flaot = CFX("1.123456789123456789");
	dev::u256 ucfx_flaot = uCFX("1123456.789123456789");
	dev::u256 gdrip_flaot = GDrip("1123456789.123456789");
	dev::u256 drip_flaot = Drip("1123456789123456789");

	bool equa5 = (cfx_flaot == ucfx_flaot);  //true
	bool equa6 = (cfx_flaot == gdrip_flaot);//true
	bool equa7 = (cfx_flaot == drip_flaot);//true


	
	std::cout << cfx_cpp::DecFloat50(cfx_flaot) << std::endl; //输出Drip数量
	std::cout << cfx_flaot << std::endl;


	getchar();

}