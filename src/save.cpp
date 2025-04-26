#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include "save.hpp"
#include "big.hpp"

namespace fs = std::filesystem;

void saveFile(save save) {
	char* home = getenv("HOME");

	fs::path savePath = fs::path(home) / ".coin" / "game.save";
	fs::create_directories(savePath.parent_path());
	std::ofstream saveFile(savePath, std::ios::binary);
	
	if (saveFile.is_open()) {
		saveFile.write(save.magic.data(), save.magic.size());
		saveFile.write(save.version.data(), save.version.size());
		saveFile.write(reinterpret_cast<char*>(&save.coins.base), sizeof(save.coins.base));
		saveFile.write(reinterpret_cast<char*>(&save.coins.mult), sizeof(save.coins.mult));

		size_t pets_size = save.pets.size();
		saveFile.write(reinterpret_cast<const char*>(&pets_size), sizeof(pets_size));

		for (int pet: save.pets) {
			saveFile.write(reinterpret_cast<const char*>(&pet), sizeof(pet));
		}
		
		saveFile.close();
	}
}
