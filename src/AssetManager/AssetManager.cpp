#include <AssetManager/AssetManager.h>
#include <iostream>

void AssetManager::loadFont(const std::filesystem::path& file_path) {
	sf::Font font;
	const std::string font_name = file_path.stem().string();
	if (!font.openFromFile(file_path)) {
		std::cout << "Cannot load font " << font_name << "!\n";
		assert(0);
	}
	font_container[font_name] = std::move(font);
}


void AssetManager::loadAllFontFromFile() {
	namespace fs = std::filesystem;
	std::string folder_path = std::string(ASSET_DIR) + "/fonts";
	for (const auto& entry : fs::directory_iterator(folder_path)) {
		fs::path file_path = entry.path();
		std::string font_name = file_path.stem().string();
		loadFont(file_path);
	}
}

const sf::Font& AssetManager::getFont(const std::string& font_name) const {
	auto it = font_container.find(font_name);
	if (it == font_container.end()) {
		std::cout << "font " << font_name << " not founded!\n";
		assert(0);
	}
	return it->second;
}