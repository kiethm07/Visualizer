#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class RoundedRectangleShape : public sf::Shape {
public:
    RoundedRectangleShape(sf::Vector2f size = { 0, 0 }, float radius = 0, unsigned int points = 10)
        : mySize(size), myRadius(radius), myPointCount(points) {
        update();
    }

    void setPointCount(int x) { myPointCount = x; }
    void setSize(sf::Vector2f size) { mySize = size; update(); }
    void setCornerRadius(float radius) { myRadius = radius; update(); }

    std::size_t getPointCount() const override { return myPointCount * 4; }
    sf::Vector2f getSize() const { return mySize; }
    sf::Vector2f getPoint(std::size_t index) const override {
        if (index >= myPointCount * 4) return { 0, 0 };

        float deltaAngle = 90.f / (myPointCount - 1);
        sf::Vector2f center;
        unsigned int centerIndex = index / myPointCount;
        unsigned int offsetIndex = index % myPointCount;

        if (centerIndex == 0) center = { mySize.x - myRadius, myRadius };
        else if (centerIndex == 1) center = center = { mySize.x - myRadius, mySize.y - myRadius };
        else if (centerIndex == 2) center = { myRadius, mySize.y - myRadius };
        else if (centerIndex == 3) center = { myRadius, myRadius };

        float angle = (centerIndex * 90.f) + (offsetIndex * deltaAngle) - 90.f;
        float x = std::cos(angle * PI / 180.f) * myRadius;
        float y = std::sin(angle * PI / 180.f) * myRadius;

        return center + sf::Vector2f(x, y);
    }

private:
    static inline const float PI = 3.141592654f;
    sf::Vector2f mySize;
    unsigned int myPointCount;
    float myRadius = 0.0f;
};