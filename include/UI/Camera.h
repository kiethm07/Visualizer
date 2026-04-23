#pragma once
#include <SFML/Graphics.hpp>

class CameraController {
public:
    CameraController() {}
    void handleEvent(sf::RenderWindow& window, sf::View& view, const sf::Event& ev);
    void setEnable(const bool flag);
    bool isEnable() const {
        return enable;
    }
    void reset(const sf::RenderWindow& window, sf::View& view);
private:
    bool dragging = false;
    bool enable = false;
    sf::Vector2i lastMousePos{};
    float zoom = 1.f;              // 1 = default
    float zoomMin = 0.25f;
    float zoomMax = 4.0f;
};