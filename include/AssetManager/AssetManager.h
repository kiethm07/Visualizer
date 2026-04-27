#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <unordered_map>
#include <string>
#include <filesystem>


class AssetManager {
public:
	void loadFont(const std::filesystem::path& file_path);
	void loadAllFontsFromFile();
	const sf::Font& getFont(const std::string& font_name) const;

	void loadTexture(const std::filesystem::path& file_path);
	void loadAllTexturesFromFile();
	const sf::Texture& getTexture(const std::string& texture_name) const;

	void loadSound(const std::filesystem::path& file_path);
	void loadAllSoundsFromFile();
	const sf::SoundBuffer& getSound(const std::string& sound_name) const;
	///Debug purpose
	int getSoundSize() const { return sound_container.size(); }
	int getFontSize() const { return font_container.size(); }
	int getTextureSize() const { return texture_container.size(); }
	void printAllFont() const { for (const auto& it : font_container) std::cout << it.first << "\n"; }
	void printAllSound() const { for (const auto& it : sound_container) std::cout << it.first << "\n"; }
private:
	std::unordered_map<std::string, sf::Font> font_container;
	std::unordered_map<std::string, sf::Texture> texture_container;
	std::unordered_map<std::string, sf::SoundBuffer> sound_container;
};