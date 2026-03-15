#pragma once
#include <SFML/Graphics.hpp>

class Arrow : public sf::Drawable {
public:
    Arrow();
    Arrow(const sf::Vector2f& from, const sf::Vector2f& to, const float& thickness = 6.f, const float& head_length = 18.f, const float& head_width = 14.f);

    void setPosition(const sf::Vector2f& from);
    sf::Vector2f getPosition() const;

    void setTarget(const sf::Vector2f& to);
    sf::Vector2f getTarget() const;

    void setPoints(const sf::Vector2f& from, const sf::Vector2f& to);

    void setThickness(const float& thickness);
    float getThickness() const;

    void setHeadLength(const float& head_length);
    float getHeadLength() const;

    void setHeadWidth(const float& head_width);
    float getHeadWidth() const;

    void setColor(const sf::Color& color);
    sf::Color getColor() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(body, states);
        target.draw(head, states);
    }

private:
    sf::Vector2f from;
    sf::Vector2f to;

    float thickness = 6.f;
    float head_length = 18.f;
    float head_width = 14.f;

    sf::RectangleShape body;
    sf::ConvexShape head;

    void updateShape();

    static float length(const sf::Vector2f& v);
    static sf::Vector2f normalize(const sf::Vector2f& v);
};