#include "number.h"

number::number(long long n) {
	if (n >= 0) {
		this->sign = 1;
	}
	else {
		this->sign = -1;
		n = -n;
	}
	do {
		this->digits.push_back(n % 1000);
		n = n / 1000;
	} while (n > 0);
}

inline unsigned int number::operator[](const size_t pos) const {
	return this->digits[pos];
}

unsigned int& number::operator[](const size_t pos) {
	return this->digits[pos];
}

std::string number::tostring() {
	std::string res;
	if (this->sign < 0) {
		res = "-";
	}
	for (int i = this->digits.size(); i >= 0; i--) {
		std::string tmp = std::to_string(this->digits[i]);
		res = res + std::string(3 - tmp.size(), '0') + tmp;
	}
	return res;
}

number mult(const number& n1, const number& n2) {
	number res;
	res.sign = n1.sign * n2.sign;
	res.digits.assign(n1.digits.size() + n2.digits.size(), 0);
	for (unsigned int i = 0; i < n1.digits.size(); i++) {
		for (unsigned int j = 0; j < n2.digits.size(); j++) {
			res[i + j] = res[i + j] + n1[i] * n2[j];
		}
	}

	res.normalize();
	return res;
}

void number::normalize() {
	number res;
	int carry = 0;
	for (unsigned int i = 0; i < this->digits.size(); i++) {
		auto sum = digits[i] + carry;
		if (sum > 0) {
			carry = sum / 1000;
			this->digits[i] = sum % 1000;
		}
		else {
			carry = (-sum) / 1000 + i;
			this->digits[i] = (-sum) % 1000;
			if (!digits[i]) {
				carry--;
			}
		}
	}
	if (!carry) {
		this->digits.push_back(carry);
	}
	while (!this->digits.back() and this->digits.size() > 1) {
		this->digits.pop_back();
	}
}
