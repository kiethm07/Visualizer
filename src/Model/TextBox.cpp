#include <Model/TextBox.h>

TextBox::TextBox(
    const sf::Font& font,
    const std::string& placeholder,
    const sf::Vector2f& pos,
    const sf::Vector2f& size,
    unsigned int char_size
) : Button(font, "", pos, size, char_size), placeholder(placeholder), caret_visible(0) {
    caret.setSize({ 1.5f, float(char_size) });
    caret.setFillColor(sf::Color::White);
    refreshText();
}

void TextBox::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
    if (const auto* mb = ev.getIf<sf::Event::MouseButtonReleased>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            sf::Vector2f mouse_pos = window.mapPixelToCoords(mb->position, view);
            focused = contains(window, view, mouse_pos);

            caret_visible = 1;
            caret_timer.restart();
            refreshText();
        }
    }

    if (const auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            sf::Vector2f mouse_pos = window.mapPixelToCoords(mb->position, view);
            focused = contains(window, view, mouse_pos);
            if (focused) return;
            value.clear();
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
        caret_visible = 1;
        caret_timer.restart();
        refreshText();
    }
}

void TextBox::update(const sf::RenderWindow& window, const sf::View& view) {
    Button::update(window, view);

    if (focused) {
        container.setFillColor(FOCUSED_COLOR);
        if (caret_timer.getElapsedTime().asSeconds() >= BLINK_INTERVAL) {
            caret_timer.restart();
            caret_visible = !caret_visible;
        }
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

static bool isValidInt(const std::string& s) {
    if (s.empty()) return 0;
    if (s[0] != '-' && (s[0] > '9' || s[0] < '0')) return 0;
    bool sign = s[0] == '-';
    if (sign && s.size() == 1) return 0;
    for (int i = sign; i < s.size(); i++) {
        if (s[i] > '9' || s[i] < '0') return 0;
    }
    return 1;
}

static int string_to_int(const std::string& s) {
    if (s.empty()) return 0;
    bool sign = s[0] == '-';
    int res = 0;
    for (int i = sign; i < s.size(); i++) {
        res = res * 10 + s[i] - '0';
    }
    if (sign) res = -res;
    return res;
}

static std::string int_to_string(int n) {
    std::string res = "";
    bool sign = n < 0;
    if (sign) n = -n;
    while (n) {
        char c = n % 10 + '0';
        res += c;
        n /= 10;
    }
    if (res.empty()) res += '0';
    if (sign) res += '-';
    reverse(res.begin(), res.end());
    return res;
}

std::optional<int> TextBox::getValueAsInt() const {
    if (value.size() > max_length) return std::nullopt;
    if (!isValidInt(value)) return std::nullopt;
    return string_to_int(value);
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
    if (value.empty() && (!focused)) {
        text.setString(placeholder);
        text.setFillColor(PLACEHOLDER_COLOR);
    }
    else {
        text.setString(value);
        text.setFillColor(TEXT_COLOR);
    }

    centerText();
    sf::Vector2f caretPos;
    if (value.empty()) {
        auto [x, y] = caret.getSize();
        caretPos = getCenter() - sf::Vector2f({0.f, y / 2.f});
    }
    else {
        caretPos = text.findCharacterPos(text.getString().getSize());
    }

    caret.setPosition(caretPos);
}

bool TextBox::isPrintable(char32_t unicode) const {
    return unicode >= 32 && unicode <= 126;
}