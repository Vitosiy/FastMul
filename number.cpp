#include "number.h"

unsigned int number::min_size = 64;

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

number::number(std::string n) {	
	n[0] == '-' ? this->sign = -1 : this->sign = 1;

	//<-

	const int border = this->sign == -1 ? 1 : 0;
	int i = n.size() - 3;
	for (i; i >= border; i = i - 3) {
		this->digits.push_back(std::stoi(n.substr(i, 3)));
	}

	if (i + 3 - border > 0) {
		this->digits.push_back(std::stoi(n.substr(border, i + 3 - border)));
	}
}

number::number(const std::vector<long long>& n) {
	this->digits = n;
	this->sign = 1;
}

number::number(std::vector<long long>&& n) {
	this->digits = n;
}

inline long long number::operator[](const size_t pos) const {
	return this->digits[pos];
}

long long& number::operator[](const size_t pos) {
	return this->digits[pos];
}

number number::operator*(const number& num) {
	auto m = std::max(this->digits.size(), num.digits.size());

	size_t k = 0;

	while (m / 2 >= min_size) {
		m = m / 2;
		k++;
	}
	m++;
	m <<= k;

	number fm1, fm2;
	fm1.digits.resize(m);
	fm2.digits.resize(m);

	std::copy(this->digits.begin(), this->digits.end(), fm1.digits.begin());
	std::copy(num.digits.begin(), num.digits.end(), fm2.digits.begin());

	auto res = fm1._fastmult(fm2);
	res.sign = this->sign * num.sign;
	res.normalize();

	return res;
}

std::string number::tostring() const {
	std::string res;
	if (this->sign < 0) {
		res = "-";
	}
	res = res + std::to_string(this->digits.back());
	for (int i = this->digits.size() - 1; i > 0; i--) {
		std::string tmp = std::to_string(this->digits[i - 1]);
		res = res + std::string(3 - tmp.size(), '0') + tmp;
	}
	return res;
}

number number::_plus(const number& n) const {
	number res;
	res.digits.resize(n.digits.size());
	std::transform(this->digits.begin(), this->digits.end(), n.digits.begin(), res.digits.begin(), std::plus<long long>());

	return res;
}

number number::_minus(const number& n) const {
	number res;
	res.digits.resize(n.digits.size());
	std::transform(this->digits.begin(), this->digits.end(), n.digits.begin(), res.digits.begin(), std::minus<long long>());

	return res;
}

number number::_mult(const number& n) const {
	number res;
	res.sign = this->sign * n.sign;
	res.digits.assign(this->digits.size() + n.digits.size(), 0);
	for (unsigned int i = 0; i < this->digits.size(); i++) {
		for (unsigned int j = 0; j < n.digits.size(); j++) {
			res[i + j] = res[i + j] + this->digits[i] * n[j];
		}
	}

	return res;
}

number number::_fastmult(const number& num) const {
	number res;

	if (num.digits.size() <= min_size) {
		res = _mult(num);
	}
	else{
		res.digits.resize(2 * num.digits.size());

		unsigned int half_size = num.digits.size() / 2;

		std::vector<long long> xl{this->digits.begin(), this->digits.begin() + half_size};
		std::vector<long long> xr{this->digits.begin() + half_size, this->digits.end()};
		std::vector<long long> yl{num.digits.begin(), num.digits.begin() + half_size};
		std::vector<long long> yr{num.digits.begin() + half_size, num.digits.end()};

		number xl_num(std::move(xl));
		number xr_num(std::move(xr));
		number yl_num(std::move(yl));
		number yr_num(std::move(yr));

		auto xyl_res = xl_num._fastmult(yl_num);
		auto xyr_res = xr_num._fastmult(yr_num);

		auto x_plus_yl = xl_num._plus(xr_num);
		auto x_plus_yr = yl_num._plus(yr_num);

		auto mid = ((x_plus_yl._fastmult(x_plus_yr))._minus(xyl_res))._minus(xyr_res); 

		std::copy(xyr_res.digits.begin(), xyr_res.digits.end(), res.digits.begin() + 2 * half_size);
		std::copy(xyl_res.digits.begin(), xyl_res.digits.end(), res.digits.begin());
		std::transform(mid.digits.begin(), mid.digits.end(), res.digits.begin() + half_size, res.digits.begin() + half_size, std::plus<long long>());
	}

	return res;
}

number number::_fastmult_async(const number& num) const {
	number res;

	if (num.digits.size() <= min_size) {
		res = _mult(num);
	}
	else {
		res.digits.resize(2 * num.digits.size());

		unsigned int half_size = num.digits.size() / 2;

		std::vector<long long> xl{this->digits.begin(), this->digits.begin() + half_size};
		std::vector<long long> xr{this->digits.begin() + half_size, this->digits.end()};
		std::vector<long long> yl{num.digits.begin(), num.digits.begin() + half_size};
		std::vector<long long> yr{num.digits.begin() + half_size, num.digits.end()};

		number xl_num(std::move(xl));
		number xr_num(std::move(xr));
		number yl_num(std::move(yl));
		number yr_num(std::move(yr));

		auto future_xyr = std::async(std::launch::async, &number::_fastmult_async, xr_num, std::reference_wrapper<const number>(yr_num));
		auto xyl_res = xl_num._fastmult_async(yl_num);
		//auto xyr_res = xr_num._fastmult_async(yr_num);

		auto xyr_res = future_xyr.get();
		auto x_plus_yl = xl_num._plus(xr_num);
		auto x_plus_yr = yl_num._plus(yr_num);

		auto mid = ((x_plus_yl._fastmult_async(x_plus_yr))._minus(xyl_res))._minus(xyr_res);

		std::copy(xyr_res.digits.begin(), xyr_res.digits.end(), res.digits.begin() + 2 * half_size);
		std::copy(xyl_res.digits.begin(), xyl_res.digits.end(), res.digits.begin());
		std::transform(mid.digits.begin(), mid.digits.end(), res.digits.begin() + half_size, res.digits.begin() + half_size, std::plus<long long>());
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

number fast_mult_async(const number& n1, const number& n2) {
	auto m = std::max(n1.digits.size(), n2.digits.size());

	size_t k = 0;

	while (m / 2 >= number::min_size) {
		m = m / 2;
		k++;
	}
	m++;
	m <<= k;

	number fm1, fm2;
	fm1.digits.resize(m);
	fm2.digits.resize(m);

	std::copy(n1.digits.begin(), n1.digits.end(), fm1.digits.begin());
	std::copy(n2.digits.begin(), n2.digits.end(), fm2.digits.begin());

	auto res = fm1._fastmult_async(fm2);
	res.sign = n1.sign * n2.sign;
	res.normalize();

	return res;
}

std::ostream& operator<<(std::ostream& ps, const number& n) {
	if (n.sign < 0) {
		ps << '-';
	}
	ps << n.digits.back();
	for (int i = n.digits.size() - 2; i >= 0; i--) {
		std::string tmp = std::to_string(n.digits[i]);
		ps << std::string(3 - tmp.size(), '0') + tmp;
	}
	return ps;
}

void number::normalize() {
	number res;
	long long carry = 0;
	for (int i = 0; i < this->digits.size(); i++) {
		long long sum = digits[i] + carry;
		if (sum >= 0) {
			carry = sum / 1000;
			this->digits[i] = sum % 1000;
		}
		else {
			carry = (sum + 1)/ 1000 - 1;
			this->digits[i] = sum - carry * 1000;
		}
	}
	if (!carry) {
		this->digits.push_back(carry);
	}
	while (!this->digits.back() and this->digits.size() > 1) {
		this->digits.pop_back();
	}

	if (res.digits.size() == 0 and res.digits[0] == 0) {
		res.sign = 1;
	}
}
