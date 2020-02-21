#pragma once

#include <vector>
#include <string>

class number {
public:
	number(long long n = 0);
	number(const std::string& n); //
	number(const std::vector<int>& n); //
	inline unsigned int operator[](const size_t pos) const; //
	unsigned int& operator[](const size_t pos); //
	number operator*(const number& num);
	friend number mult(const number& n1, const number& n2); //
	std::string tostring(); //
	friend std::ostream& operator << (std::ostream& ps, const number& n); //

private:
	number _plus(const number& n) const; // aligned and unsigned
	number _minus(const number& n) const; // aligned and unsigned
	number _mult(const number& n) const; // casual mult
	number _fastmult(const number& n) const; //karatsuba
	void normalize();


	int sign;
	std::vector<unsigned int> digits;
};

