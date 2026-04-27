#include <UI/Setting.h>

Setting::Setting(const AssetManager& a_manager) :
	a_manager(a_manager),
	background(a_manager.getTexture("BlurredMincraftBackground")),
	btn_back(a_manager, "Back", {}, {}, 20),
	btn_music(a_manager, "Music", {}, {}, 20),
	btn_sfx(a_manager, "Sfx", {}, {}, 20)
{
	subbackground.setFillColor(sf::Color(80, 80, 80, 220));
	subbackground.setCornerRadius(10);
	btn_back.setFont("Silkscreen-Regular");
	btn_music.setFont("Silkscreen-Regular");
	btn_sfx.setFont("Silkscreen-Regular");
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
	sf::Vector2u size = window.getSize();
	sf::Vector2f winSize = sf::Vector2f(size);

	auto centerOrigin = [](auto& object) {
		auto bounds = object.getLocalBounds();
		object.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
						   bounds.position.y + bounds.size.y / 2.f });
		};

	auto scaleToWidth = [&winSize](sf::Sprite& sprite, float percentage) {
		auto bounds = sprite.getLocalBounds();
		if (bounds.size.x > 0) {
			float scale = (winSize.x * percentage) / bounds.size.x;
			sprite.setScale({ scale, scale });
		}
		};

	auto setupButton = [&winSize](Button& button, float yPosRatio) {
		float width = 0.2f * winSize.x;
		float height = 0.1f * winSize.y;
		button.setButtonSize({ width, height });
		button.setOrigin({ width / 2.f, height / 2.f });
		button.setPosition({ winSize.x / 2.f, winSize.y * yPosRatio });
		button.setCharacterSize(static_cast<unsigned int>(winSize.y * 0.04f));
		};

	setupButton(btn_music, 0.4f);
	setupButton(btn_sfx, 0.55f);
	setupButton(btn_back, 0.7f);

	subbackground.setSize(winSize / 1.75f);
	subbackground.setOrigin(subbackground.getSize() / 2.f);
	subbackground.setPosition(winSize / 2.f);
}

void Setting::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	btn_music.update(window, view);
	btn_sfx.update(window, view);
	btn_back.update(window, view);
}

std::optional<MenuState> Setting::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	if (btn_back.handleEvent(window, view, ev)) {
		//std::cout << "hello\n";
		return previous_state;
	}
	if (btn_music.handleEvent(window, view, ev)) {
		std::string filePath = cr::utils::SimpleFileDialog::dialog();
		if (!filePath.empty() && sound_controller) {
			sound_controller->playMusic(filePath);
		}
	}
	if (btn_sfx.handleEvent(window, view, ev)) {

	}

	return std::nullopt;
}