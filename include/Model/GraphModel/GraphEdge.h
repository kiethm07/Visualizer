#pragma once
#include <Model/Arrow.h>

class GraphEdge : public Arrow {
public:
    GraphEdge(const sf::Font& font, int weight, const sf::Vector2f& from, const sf::Vector2f& to, const float& thickness = 3.f)
        : Arrow(from, to, thickness, 0.f, 0.f), weight_text(font, std::to_string(weight), 20)
    {
        weight_text.setFillColor(sf::Color::Yellow);

        updateWeightPosition();
    }

    void setWeightColor(const sf::Color& color) {
        weight_text.setFillColor(color);
    }

    void updateWeightPosition() {
        sf::Vector2f start = getPosition();
        sf::Vector2f end = getTarget();
        sf::Vector2f mid = (start + end) / 2.f;

        sf::FloatRect textRect = weight_text.getLocalBounds();

        weight_text.setOrigin({
            textRect.position.x + textRect.size.x / 2.0f,
            textRect.position.y + textRect.size.y / 2.0f
            });

        sf::Vector2f direction = end - start;
        sf::Vector2f normal = { -direction.y, direction.x };
        float len = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        if (len > 0.1f) normal /= len;

        weight_text.setPosition(mid + normal * 12.f);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        Arrow::draw(target, states);
        target.draw(weight_text, states);
    }

private:
    sf::Text weight_text;
};