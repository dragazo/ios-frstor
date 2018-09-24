#include <iostream>
#include <iomanip>
#include <vector>

#include "iosfrstor.h"

int main()
{
	iosfrstor a(std::cout);
	iosfrstor b(std::cin);
	
	wiosfrstor c(std::wcout);
	wiosfrstor d(std::wcin);
	
	std::vector<iosfrstor> arr;
	std::vector<wiosfrstor> warr;

	arr.emplace_back(std::cout);
	arr.emplace_back(std::cin);

	warr.emplace_back(std::wcout);
	warr.emplace_back(std::wcin);

	sizeof(iosfrstor);
	sizeof(wiosfrstor);
	
	return 0;
}
