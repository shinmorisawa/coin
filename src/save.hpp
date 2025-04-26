#ifndef SAVE_HPP
#define SAVE_HPP

#include <array>
#include <vector>
#include "big.hpp"

struct save {
	std::array<char, 8> magic;
	std::array<char, 4> version;
	big coins;	
	std::vector<int> pets;
};

void saveFile(save save);

#endif
