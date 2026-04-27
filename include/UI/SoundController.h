#pragma once
#include <SFML/Audio.hpp>
#include <AssetManager/AssetManager.h>
#include <string>
#include <map>

class SoundController {
public:
	SoundController(const AssetManager& a_manager);

	void playMusic(const std::string& fileName, bool loop = true);
	void stopMusic();
	void playSFX(const std::string& name);

	void setMusicVolume(float volume);
	void setSfxVolume(float volume);
	void setMuteMusic(bool f);
	void setMuteSfx(bool f);

	float getMusicVolume() const { return music_mute ? 0 : music_volume; }
	float getSFXVolume() const { return sfx_mute ? 0 : sfx_volume; }

private:
	const AssetManager& a_manager;
	sf::Music bgm;
	sf::Sound sfx;

	float music_volume = 100.f;
	float sfx_volume = 100.f;

	float music_mute = 0;
	float sfx_mute = 0;
};