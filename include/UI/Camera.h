#pragma once
#include <SFML/Graphics.hpp>

struct CameraController {
    bool dragging = false;
    sf::Vector2i lastMousePos{};
    float zoom = 1.f;              // 1 = default
    float zoomMin = 0.1f;
    float zoomMax = 10.0f;

    void handleEvent(sf::RenderWindow& window, sf::View& view, const sf::Event& ev);
};