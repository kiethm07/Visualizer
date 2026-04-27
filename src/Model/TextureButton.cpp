#include <Model/TextureButton.h>
#include <SFML/Graphics.hpp>

SoundController* TextureButton::sound_controller = nullptr;

TextureButton::TextureButton(const AssetManager& a_manager, const std::string& texture_name, const sf::Vector2f pos) :
	a_manager(a_manager),
	sprite(a_manager.getTexture(texture_name)),
	click_sound(a_manager.getSound("button_click"))
{
	sprite.setPosition(pos);

	sf::FloatRect bounds = sprite.getLocalBounds();
	sf::Vector2f size = bounds.size;
	sprite.setOrigin({ std::round(size.x / 2.f), std::round(size.y / 2.f) });

	click_sound.setVolume(80.f);
}

void TextureButton::setPosition(const sf::Vector2f& pos) {
	sprite.setPosition(pos);
}

sf::Vector2f TextureButton::getPosition() const {
	return sprite.getPosition();
}

void TextureButton::setOrigin(const sf::Vector2f& origin) {
	//sprite.setOrigin(origin);

	//Dummy call
	//Origin of texture have different logic compared to normal BUtton
	//So disbale this function, origin is calculated in the constructor
}

sf::Vector2f TextureButton::getOrigin() const {
	return sprite.getOrigin();
}

void TextureButton::setScale(const sf::Vector2f& scale) {
	baseScale = scale;
	sprite.setScale({ baseScale.x * currentScale, baseScale.y * currentScale });
}

void TextureButton::setButtonSize(const sf::Vector2f& size) {
	lastTargetSize = size;
	sf::FloatRect bounds = sprite.getLocalBounds();
	sf::Vector2f rec_size = bounds.size;
	if (rec_size.x != 0 && rec_size.y != 0) {
		baseScale.x = size.x / rec_size.x;
		baseScale.y = size.y / rec_size.y;
		sprite.setScale({ baseScale.x * currentScale, baseScale.y * currentScale });
	}
}

void TextureButton::setTexture(const std::string& texture_name) {
	sprite.setTexture(a_manager.getTexture(texture_name), true);

	sf::FloatRect bounds = sprite.getLocalBounds();
	sf::Vector2f rec_size = bounds.size;

	if (rec_size.x != 0 && rec_size.y != 0 && lastTargetSize.x != 0 && lastTargetSize.y != 0) {
		baseScale.x = lastTargetSize.x / rec_size.x;
		baseScale.y = lastTargetSize.y / rec_size.y;
		sprite.setScale({ baseScale.x * currentScale, baseScale.y * currentScale });
	}
}
sf::FloatRect TextureButton::getGlobalBounds() const {
	return sprite.getGlobalBounds();
}

bool TextureButton::contains(const sf::RenderWindow& window, const sf::View& view, const sf::Vector2f& position) {
	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	return sprite.getGlobalBounds().contains(mouse_world);
}

bool TextureButton::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	if (mousePressed(window, view, ev)) {
		click_sound.setVolume(SOUND_FACTOR * sound_controller->getSFXVolume());
		click_sound.play();
		return true;
	}
	return false;
}

void TextureButton::update(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	hovered = sprite.getGlobalBounds().contains(mouse_world);

	if (hovered) {
		targetScale = 1.05f;
		sprite.setColor(sf::Color(200, 200, 200));
	}
	else {
		targetScale = 1.0f;
		sprite.setColor(sf::Color::White);
	}

	currentScale += (targetScale - currentScale) * 0.2f;
	sprite.setScale({ baseScale.x * currentScale, baseScale.y * currentScale });
}

bool TextureButton::mousePressed(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {


	if (const auto* mouse_press = ev.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse_press->button == sf::Mouse::Button::Left) {
			sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
			return sprite.getGlobalBounds().contains(mouse_world);
		}
	}
	return 0;
}