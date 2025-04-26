#include <ctime>
#include <iostream>
#include "rand.hpp"
#include "save.hpp"
#include "big.hpp"

int main() {
	srand(time(NULL));
	for (int i = 0; i <= 5000; i++) {
		int n = randint(1, 100);
		std::cout << "\033[31m" << n << "\033[0m" << std::endl;
	}
	return 0;
}

