#include <ctime>
#include <iostream>
#include "rand.hpp"
#include "game.hpp"

int main() {
	srand(time(NULL));
	
	startGame();

	return 0;
}

