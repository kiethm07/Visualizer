#pragma once
#include <SFML/Graphics.hpp>

struct CameraController {
    bool dragging = false;
    bool enable = false;
    sf::Vector2i lastMousePos{};
    float zoom = 1.f;              // 1 = default
    float zoomMin = 0.33f;
    float zoomMax = 3.0f;

    void handleEvent(sf::RenderWindow& window, sf::View& view, const sf::Event& ev);
    void setEnable(const bool flag);
};