#include "number.h"
#include <cassert>
#include <iostream>


int main() {
	number n1("31234567543567865434567897652");
	number n2("2541768186237129385173126538617239658127637819361532");
	
	auto res = n1 * n2;

	std::cout << res << std::endl;






	return 0;
}