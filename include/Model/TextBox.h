#pragma once
#include <Model/Button.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <optional>

class TextBox : public Button {
public:
    explicit TextBox(
        const sf::Font& font,
        const std::string& placeholder,
        const sf::Vector2f& pos,
        const sf::Vector2f& size,
        unsigned int char_size,
        bool outside_click_reset = true
    );

    void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
    void update(const sf::RenderWindow& window, const sf::View& view);
    void reset();
    void setValue(const std::string& value);
    void setPlaceholder(const std::string& placeholder);
    void setFocused(bool focused);
    void setMaxLength(std::size_t max_length);

    std::optional<int> getValueAsInt() const;
    const std::string& getValue() const;
    const std::string& getPlaceholder() const;
    bool isFocused() const;
    bool isEmpty() const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override{
        Button::draw(target, states);

        if (focused && caret_visible) {
            target.draw(caret, states);
        }
    }
    void setOutsideClickReset(bool reset) {
        outside_click_reset = reset;
	}
private:
    std::string value;
    std::string placeholder;
    bool focused = false;
    std::size_t max_length = 5;
    sf::Vector2f caret_pos;
    sf::Clock caret_timer;
    bool caret_visible;
    sf::RectangleShape caret;
    bool outside_click_reset = true;

    const float BLINK_INTERVAL = 0.5f;
    const sf::Color FOCUSED_COLOR = sf::Color(70, 110, 180);
    const sf::Color PLACEHOLDER_COLOR = sf::Color(160, 160, 160);
    const sf::Color TEXT_COLOR = sf::Color::White;

    void refreshText();
    bool isPrintable(char32_t unicode) const;
};