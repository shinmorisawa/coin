#include <iostream>
#include "rand.hpp"
#include "big.hpp"
#include "save.hpp"

void clearScreen() {
	std::cout << "\033[2J";
}

void startGame() {
	clearScreen();

	std::cout << "\033[1mcoin\033[0m" << std::endl;
	std::cout << "a game about collecting coins";

	while (1) {}
}

