#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class number {
public:
	number(long long n = 0);
	number(std::string n);
	number(const std::vector<long long>& n);
	number(std::vector<long long>&& n);
	inline long long operator[](const size_t pos) const;
	long long& operator[](const size_t pos);
	number operator*(const number& num);
	friend number mult(const number& n1, const number& n2);
	std::string tostring() const;
	friend std::ostream& operator << (std::ostream& ps, const number& n);

private:
	number _plus(const number& n) const; // aligned and unsigned
	number _minus(const number& n) const; // aligned and unsigned
	number _mult(const number& n) const; // casual mult
	number _fastmult(const number& n) const; //karatsuba
	void normalize();


	int sign;
	static unsigned int min_size;
	std::vector<long long> digits;
};

