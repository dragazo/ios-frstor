#include <iostream>
#include <iomanip>
#include <vector>

#include "iosfrstor.h"

int main()
{
	iosfrstor a(std::cout);
	iosfrstor b(std::cin);
	
	std::vector<iosfrstor> arr, arr2;

	arr.emplace_back(std::cout);
	arr.emplace_back(std::cin);

	sizeof(iosfrstor);
	
	return 0;
}
