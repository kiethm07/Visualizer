#pragma once
#include <Model/Button.h>
#include <SFML/Graphics.hpp>
#include <string>

class TextBox : public Button {
public:
    explicit TextBox(
        const sf::Font& font,
        const std::string& placeholder,
        const sf::Vector2f& pos,
        const sf::Vector2f& size,
        unsigned int char_size
    );

    void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
    void update(const sf::RenderWindow& window, const sf::View& view);

    void setValue(const std::string& value);
    void setPlaceholder(const std::string& placeholder);
    void setFocused(bool focused);
    void setMaxLength(std::size_t max_length);

    const std::string& getValue() const;
    const std::string& getPlaceholder() const;
    bool isFocused() const;
    bool isEmpty() const;

private:
    std::string value;
    std::string placeholder;
    bool focused = false;
    std::size_t max_length = 32;

    const sf::Color FOCUSED_COLOR = sf::Color(70, 110, 180);
    const sf::Color PLACEHOLDER_COLOR = sf::Color(160, 160, 160);
    const sf::Color TEXT_COLOR = sf::Color::White;

    void refreshText();
    bool isPrintable(char32_t unicode) const;
};