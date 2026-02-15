#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <string>
#include <filesystem>


class AssetManager {
public:
	void loadFont(const std::filesystem::path& file_path);
	void loadAllFontFromFile();
	const sf::Font& getFont(const std::string& font_name);
	///Debug purpose
	int getFontSize() { return font_container.size(); }
	int getTextureSize() { return texture_container.size(); }
	void printAllFont() { for (const auto it : font_container) std::cout << it.first << "\n"; }
private:
	std::unordered_map<std::string, sf::Font> font_container;
	std::unordered_map<std::string, sf::Texture> texture_container;
};