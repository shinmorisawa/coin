#include <cstdlib>
#include <ctime>

int randint(int min, int max) {
	return rand() % (max - min + 1) + min;
}
