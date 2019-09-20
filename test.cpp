#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>

#include "iosfrstor.h"

int main()
{
	std::cout << std::setfill('f') << std::setw(18) << std::setprecision(2) << std::fixed;
	std::ios::fmtflags flags = std::cout.flags();

	iosfmt fmt(std::cout);
	iosfmt fmt_cpy{ std::cout };

	assert(fmt.fill() == 'f');
	assert(fmt.width() == 18);
	assert(fmt.precision() == 2);
	assert(fmt.flags() == flags);

	assert(fmt_cpy.fill() == 'f');
	assert(fmt_cpy.width() == 18);
	assert(fmt_cpy.precision() == 2);
	assert(fmt_cpy.flags() == flags);

	{
		assert(std::cout.fill() == 'f');
		assert(std::cout.width() == 18);
		assert(std::cout.precision() == 2);
		assert(std::cout.flags() == flags);

		iosfrstor _(std::cout);
		assert(std::cout.fill() == 'f');
		assert(std::cout.width() == 18);
		assert(std::cout.precision() == 2);
		assert(std::cout.flags() == flags);

		std::cout << std::setfill(' ') << std::setw(12) << std::setprecision(3) << std::scientific;
		std::cout >> fmt_cpy;

		assert(std::cout.fill() == ' ');
		assert(std::cout.width() == 12);
		assert(std::cout.precision() == 3);
		assert(std::cout.flags() != flags);

		assert(fmt.fill() == 'f');
		assert(fmt.width() == 18);
		assert(fmt.precision() == 2);
		assert(fmt.flags() == flags);
		
		std::cout >> fmt;

		assert(fmt.fill() == ' ');
		assert(fmt.width() == 12);
		assert(fmt.precision() == 3);
		assert(fmt.flags() != flags);
	}
	assert(std::cout.fill() == 'f');
	assert(std::cout.width() == 18);
	assert(std::cout.precision() == 2);
	assert(std::cout.flags() == flags);
	std::cout >> fmt;
	assert(fmt.fill() == 'f');
	assert(fmt.width() == 18);
	assert(fmt.precision() == 2);
	assert(fmt.flags() == flags);

	std::cout << fmt_cpy;
	assert(std::cout.fill() == ' ');
	assert(std::cout.width() == 12);
	assert(std::cout.precision() == 3);
	assert(std::cout.flags() != flags);
	{
		iosfrstor _(std::cout);
		assert(std::cout.fill() == ' ');
		assert(std::cout.width() == 12);
		assert(std::cout.precision() == 3);
		assert(std::cout.flags() != flags);
	}
	assert(std::cout.fill() == ' ');
	assert(std::cout.width() == 12);
	assert(std::cout.precision() == 3);
	assert(std::cout.flags() != flags);
	{
		iosfrstor _(std::cout);
		assert(std::cout.fill() == ' ');
		assert(std::cout.width() == 12);
		assert(std::cout.precision() == 3);
		assert(std::cout.flags() != flags);
		std::ios::fmtflags _flags = std::cout.flags();

		std::cout << iosfmt(std::cout).fill('I').width(7).precision(8);
		assert(std::cout.fill() == 'I');
		assert(std::cout.width() == 7);
		assert(std::cout.precision() == 8);
		assert(std::cout.flags() == _flags);

		_.fmt() = fmt;
	}
	assert(std::cout.fill() == 'f');
	assert(std::cout.width() == 18);
	assert(std::cout.precision() == 2);
	assert(std::cout.flags() == flags);

	iosfrstor a(std::cout);
	iosfrstor b{ std::cin };
	
	wiosfrstor c{ std::wcout };
	wiosfrstor d(std::wcin);
	
	iosfmt fmt_a(std::cout);
	iosfmt fmt_b{ std::cin };

	wiosfmt fmt_c{ std::wcout };
	wiosfmt fmt_d(std::wcin);

	std::cout >> fmt_a;
	std::cout << fmt_a;

	std::cin >> fmt_a;
	std::cin << fmt_a;

	std::wcout >> fmt_c;
	std::wcout << fmt_c;

	std::wcin >> fmt_c;
	std::wcin << fmt_c;

	std::cout << iosfmt().fill('f').precision(3).width(5).flags(std::ios::fixed);
	std::wcout << wiosfmt().fill(L'f').precision(3).width(5).flags(std::ios::fixed);
	
	{
		std::vector<iosfmt> vec;
		vec.push_back(fmt_a);
		vec.push_back(fmt_b);
	}

	{
		iosfmt _f;
		std::cout >> _f;
	}

	return 0;
}
