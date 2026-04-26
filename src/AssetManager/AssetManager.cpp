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

void AssetManager::loadAllFontsFromFile() {
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

void AssetManager::loadTexture(const std::filesystem::path& file_path) {
	sf::Texture texture;
	const std::string texture_name = file_path.stem().string();

	if (!texture.loadFromFile(file_path.string())) {
		std::cout << "Cannot load texture " << texture_name << "!\n";
		assert(0);
	}
	texture_container[texture_name] = std::move(texture);
}

void AssetManager::loadAllTexturesFromFile() {
	namespace fs = std::filesystem;
	std::string folder_path = std::string(ASSET_DIR) + "/textures";

	if (!fs::exists(folder_path)) {
		std::cout << "Folder " << folder_path << " does not exist!\n";
		return;
	}

	for (const auto& entry : fs::directory_iterator(folder_path)) {
		fs::path file_path = entry.path();
		loadTexture(file_path);
	}
}

const sf::Texture& AssetManager::getTexture(const std::string& texture_name) const {
	auto it = texture_container.find(texture_name);
	if (it == texture_container.end()) {
		std::cout << "Texture " << texture_name << " not found!\n";
		assert(0);
	}
	return it->second;
}

void AssetManager::loadSound(const std::filesystem::path& file_path) {
	sf::SoundBuffer sound;
	const std::string sound_name = file_path.stem().string();
	if (!sound.loadFromFile(file_path)) {
		std::cout << "Cannot load sound " << sound_name << "!\n";
		assert(0);
	}
	sound_container[sound_name] = std::move(sound);
}

void AssetManager::loadAllSoundsFromFile() {
	namespace fs = std::filesystem;
	std::string folder_path = std::string(ASSET_DIR) + "/sounds";

	for (const auto& entry : fs::directory_iterator(folder_path)) {
		fs::path file_path = entry.path();
		std::string sound_name = file_path.stem().string();
		loadSound(file_path);
	}
}

const sf::SoundBuffer& AssetManager::getSound(const std::string& sound_name) const {
	auto it = sound_container.find(sound_name);
	if (it == sound_container.end()) {
		std::cout << "sound " << sound_name << " not founded!\n";
		assert(0);
	}
	return it->second;
}