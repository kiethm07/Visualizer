#include <UI/Camera.h>
#include <SFML/Graphics.hpp>
#include <iostream>

void CameraController::handleEvent(sf::RenderWindow& window, sf::View& view, const sf::Event& ev) {
    if (enable == false) return;
    if (const auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Right) {
            dragging = true;
            lastMousePos = mb->position;
        }
    }
    if (const auto* mb = ev.getIf<sf::Event::MouseButtonReleased>()) {
        if (mb->button == sf::Mouse::Button::Right) dragging = false;
    }
    if (const auto* mw = ev.getIf<sf::Event::MouseWheelScrolled>()) {
        const float factor = (mw->delta > 0.f) ? 0.9f : 1.1f;

        float newZoom = zoom * factor;
        if (newZoom < zoomMin) newZoom = zoomMin;
        if (newZoom > zoomMax) newZoom = zoomMax;

        const sf::Vector2i mousePx = mw->position;
        const sf::Vector2f before = window.mapPixelToCoords(mousePx, view);

        view.zoom(newZoom / zoom);
        zoom = newZoom;

        const sf::Vector2f after = window.mapPixelToCoords(mousePx, view);
        view.move(before - after);
    }

    if (const auto* mm = ev.getIf<sf::Event::MouseMoved>()) {
        if (!dragging) return;

        const sf::Vector2i nowPos = mm->position;
        const sf::Vector2f a = window.mapPixelToCoords(lastMousePos, view);
        const sf::Vector2f b = window.mapPixelToCoords(nowPos, view);

        view.move(a - b);
        lastMousePos = nowPos;
    }

    if (const auto* rs = ev.getIf<sf::Event::Resized>()) {
        //std::cout << window.getSize().x << " " << window.getSize().y << "\n";
    }
}

void CameraController::setEnable(const bool flag) {
    enable = flag;
}