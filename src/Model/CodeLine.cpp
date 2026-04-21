#include <Model/CodeLine.h>

CodeLine::CodeLine(const std::string& label, const sf::Font& font, unsigned int char_size) : text(font, label, char_size)
{
	sf::Vector2f text_size = text.getLocalBounds().size;
	background.setSize(sf::Vector2f(text_size.x, text_size.y));
	background.setFillColor(DEFAULT_BG_COLOR);
	text.setFillColor(DEFAULT_TEXT_COLOR);
}

void CodeLine::setPosition(const sf::Vector2f& pos) {
	sf::Vector2i int_pos(pos);
	background.setPosition(pos);
	//text.setPosition(sf::Vector2f(int_pos));
	text.setPosition(sf::Vector2f(int_pos.x + X_PADDING, int_pos.y));
}

void CodeLine::setSize(const sf::Vector2f& size) {
	sf::Vector2f text_size = text.getLocalBounds().size;
	sf::Vector2f new_size = sf::Vector2f(std::max(size.x, text_size.x + 2 * X_PADDING), std::max(size.y, text_size.y));
	background.setSize(new_size);

}

void CodeLine::setBackgroundColor(const sf::Color& color) {
	background.setFillColor(color);
}

void CodeLine::setTextColor(const sf::Color& color) {
	text.setFillColor(color);
}

void CodeLine::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(background, states);
	target.draw(text, states);
}