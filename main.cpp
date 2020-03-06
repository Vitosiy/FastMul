#include "number.h"
#include <cassert>
#include <iostream>


int main() {
	number n1("5172673896851738917253765876198726354126387385154971235410234");
	number n2("2541768186237129385173126538617239658127637819361532");
	
	auto res = n1 * n2;

	std::cout << res << std::endl;






	return 0;
}