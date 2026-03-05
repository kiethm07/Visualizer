#pragma once
#include <SFML/Graphics.hpp>

class CameraController {
public:
    void handleEvent(sf::RenderWindow& window, sf::View& view, const sf::Event& ev);
    void setEnable(const bool flag);
    void reset(const sf::RenderWindow& window, sf::View& view);
private:
    bool dragging = false;
    bool enable = true;
    sf::Vector2i lastMousePos{};
    float zoom = 1.f;              // 1 = default
    float zoomMin = 0.5f;
    float zoomMax = 2.0f;
};