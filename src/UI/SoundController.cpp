#include <UI/SoundController.h>
#include <iostream>

SoundController::SoundController(const AssetManager& a_manager) : 
	a_manager(a_manager),
	sfx(a_manager.getSound("button_click"))
{
	std::string file_path = std::string(ASSET_DIR) + "/musics/Gymnopedie1.mp3";
	playMusic(file_path, 1);
}

void SoundController::playMusic(const std::string& file_path, bool loop) {
	if (!bgm.openFromFile(file_path)) {
		return;
	}
	std::cout << file_path << "\n";
	bgm.setLooping(loop);
	bgm.setVolume(music_volume);
	bgm.play();
}

void SoundController::stopMusic() {
	bgm.stop();
}

void SoundController::playSFX(const std::string& name) {
	sfx.setBuffer(a_manager.getSound(name));
	sfx.setVolume(sfx_volume);
	sfx.play();
}

void SoundController::setMusicVolume(float volume) {
	music_volume = volume;
	bgm.setVolume(music_volume);
}

void SoundController::setSfxVolume(float volume) {
	sfx_volume = volume;
	sfx.setVolume(sfx_volume);
}