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
	std::ofstream saveFile(savePath);
	
	if (saveFile.is_open()) {
		saveFile.write(reinterpret_cast<char*>(&save.coins.base), sizeof(save.coins.base));
		saveFile.write(reinterpret_cast<char*>(&save.coins.mult), sizeof(save.coins.mult));
		saveFile.close();
	}
}
