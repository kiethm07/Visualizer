#include <UI/Setting.h>

Setting::Setting(const AssetManager& a_manager) :
	a_manager(a_manager),
	background(a_manager.getTexture("BlurredMincraftBackground")),
	icon_music(a_manager, "Music", { 0.f, 0.f }),
	slider_music({ 0.f, 0.f }, { 0.f, 0.f }, 0.f, 100.f, 50.f),
	icon_sfx(a_manager, "Speaker", { 0.f, 0.f }),
	slider_sfx({ 0.f, 0.f }, { 0.f, 0.f }, 0.f, 100.f, 50.f),
	btn_choose_music(a_manager, "Choose Music", {}, {}, 20),
	btn_back(a_manager, "Back", {}, {}, 20)
{
	subbackground.setFillColor(sf::Color(80, 80, 80, 220));
	subbackground.setCornerRadius(10);
	btn_choose_music.setFont("Silkscreen-Regular");
	btn_back.setFont("Silkscreen-Regular");

	sound_controller->setMusicVolume(50.f);
	sound_controller->setSfxVolume(50.f);
}

void Setting::update(const sf::RenderWindow& window, const sf::View& view) {
	updateButtonState(window, view);
	updateWindowSize(window, view);

	sf::Vector2u windowSize = window.getSize();

	float scaleX = (float)windowSize.x / background.getTexture().getSize().x;
	float scaleY = (float)windowSize.y / background.getTexture().getSize().y;

	background.setScale({ scaleX, scaleY });
}

void Setting::updateWindowSize(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2f winSize = sf::Vector2f(window.getSize());

	subbackground.setSize(winSize / 1.75f);
	subbackground.setOrigin(subbackground.getSize() / 2.f);
	subbackground.setPosition(winSize / 2.f);

	auto setupIcon = [&winSize](TextureButton& icon, float xRatio, float yRatio) {
		icon.setButtonSize({ winSize.y * 0.08f, winSize.y * 0.08f });
		icon.setOrigin({ icon.getGlobalBounds().size.x / 2.f, icon.getGlobalBounds().size.y / 2.f });
		icon.setPosition({ winSize.x * xRatio, winSize.y * yRatio });
		};

	auto setupSlider = [&winSize](Slider& slider, float xRatio, float yRatio) {
		slider.setSliderSize({ winSize.x * 0.25f, winSize.y * 0.02f });
		slider.setOrigin({ slider.getSize().x / 2.f, slider.getSize().y / 2.f });
		slider.setPosition({ winSize.x * xRatio, winSize.y * yRatio });
		};

	auto setupButton = [&winSize](Button& button, float yPosRatio) {
		float width = 0.25f * winSize.x;
		float height = 0.08f * winSize.y;
		button.setButtonSize({ width, height });
		button.setOrigin({ width / 2.f, height / 2.f });
		button.setPosition({ winSize.x / 2.f, winSize.y * yPosRatio });
		button.setCharacterSize(static_cast<unsigned int>(winSize.y * 0.035f));
		};

	setupIcon(icon_music, 0.35f, 0.33f);
	setupIcon(icon_sfx, 0.35f, 0.45f);

	setupSlider(slider_music, 0.55f, 0.33f);
	setupSlider(slider_sfx, 0.55f, 0.45f);

	setupButton(btn_choose_music, 0.58f);
	setupButton(btn_back, 0.68f);
}

void Setting::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	icon_music.update(window, view);
	icon_sfx.update(window, view);
	btn_choose_music.update(window, view);
	btn_back.update(window, view);
}

std::optional<MenuState> Setting::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	slider_music.handleEvent(window, view, ev);
	slider_sfx.handleEvent(window, view, ev);

	if (slider_music.getIsDragging()) {
		sound_controller->setMusicVolume(slider_music.getValue());
	}

	if (slider_sfx.getIsDragging()) {
		sound_controller->setSfxVolume(slider_sfx.getValue());
		if (ev.getIf<sf::Event::MouseButtonPressed>()) {
			sound_controller->playSFX("button_click");
		}
	}

	if (icon_music.handleEvent(window, view, ev)) {
		music_mute = !music_mute;
		icon_music.setTexture(music_mute ? "MusicMute" : "Music");
		sound_controller->setMuteMusic(music_mute);
	}

	if (icon_sfx.handleEvent(window, view, ev)) {
		sfx_mute = !sfx_mute;
		icon_sfx.setTexture(sfx_mute ? "SpeakerMute" : "Speaker");
		sound_controller->setMuteSfx(sfx_mute);
	}

	if (btn_back.handleEvent(window, view, ev)) {
		return previous_state;
	}

	if (btn_choose_music.handleEvent(window, view, ev)) {
		std::string filePath = cr::utils::SimpleFileDialog::dialog();
		if (!filePath.empty() && sound_controller) {
			sound_controller->playMusic(filePath);
		}
	}

	return std::nullopt;
}