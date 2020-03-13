#!/usr/bin/python3
from random import getrandbits


def get_test_mult(bits):
	a = getrandbits(bits)
	b = getrandbits(bits)
	answ = str(a * b)
	return f'{a}\n{b}\n{answ}\n'


def main():
	file = open('tests.txt', 'w')
	for i in range(10):
		file.write(get_test_mult(500000))


if __name__ == '__main__':
	main()
