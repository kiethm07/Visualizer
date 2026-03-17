#include <Model/TextBox.h>

TextBox::TextBox(
    const sf::Font& font,
    const std::string& placeholder,
    const sf::Vector2f& pos,
    const sf::Vector2f& size,
    unsigned int char_size
) : Button(font, "", pos, size, char_size), placeholder(placeholder) {
    refreshText();
}

void TextBox::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
    if (const auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            sf::Vector2f mouse_pos = window.mapPixelToCoords(mb->position, view);
            focused = contains(window, view, mouse_pos);
            refreshText();
        }
    }

    if (!focused) return;

    if (const auto* te = ev.getIf<sf::Event::TextEntered>()) {
        char32_t unicode = te->unicode;

        if (unicode == 8) {
            if (!value.empty()) {
                value.pop_back();
            }
        }
        else if (unicode == 13 || unicode == '\n' || unicode == '\r') {
        }
        else if (isPrintable(unicode) && value.size() < max_length) {
            value += static_cast<char>(unicode);
        }

        refreshText();
    }
}

void TextBox::update(const sf::RenderWindow& window, const sf::View& view) {
    Button::update(window, view);

    if (focused) {
        container.setFillColor(FOCUSED_COLOR);
    }

    refreshText();
}

void TextBox::setValue(const std::string& value) {
    this->value = value;
    if (this->value.size() > max_length) {
        this->value.resize(max_length);
    }
    refreshText();
}

void TextBox::setPlaceholder(const std::string& placeholder) {
    this->placeholder = placeholder;
    refreshText();
}

void TextBox::setFocused(bool focused) {
    this->focused = focused;
    refreshText();
}

void TextBox::setMaxLength(std::size_t max_length) {
    this->max_length = max_length;
    if (value.size() > this->max_length) {
        value.resize(this->max_length);
    }
    refreshText();
}

const std::string& TextBox::getValue() const {
    return value;
}

const std::string& TextBox::getPlaceholder() const {
    return placeholder;
}

bool TextBox::isFocused() const {
    return focused;
}

bool TextBox::isEmpty() const {
    return value.empty();
}

void TextBox::refreshText() {
    if (value.empty()) {
        text.setString(placeholder);
        text.setFillColor(PLACEHOLDER_COLOR);
    }
    else {
        text.setString(value);
        text.setFillColor(TEXT_COLOR);
    }

    centerText();
}

bool TextBox::isPrintable(char32_t unicode) const {
    return unicode >= 32 && unicode <= 126;
}