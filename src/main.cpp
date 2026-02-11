#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

struct CameraController {
    bool dragging = false;
    sf::Vector2i lastMousePos{};
    float zoom = 1.f;              // 1 = default
    float zoomMin = 0.1f;
    float zoomMax = 10.0f;

    void handleEvent(sf::RenderWindow& window, sf::View& view, const sf::Event& ev) {
        if (ev.is<sf::Event::Closed>()) window.close();

        if (const auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Middle) {
                dragging = true;
                lastMousePos = mb->position;
            }
        }
        if (const auto* mb = ev.getIf<sf::Event::MouseButtonReleased>()) {
            if (mb->button == sf::Mouse::Button::Middle) dragging = false;
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

        }
    }
};

int main() {
    sf::RenderWindow window;   

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    window.create(
        sf::VideoMode({ 1280, 720 }),
        "Visualizer",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );

    window.setFramerateLimit(120);
    std::cout << window.getSettings().antiAliasingLevel << "\n";

    sf::View view(sf::FloatRect({ 0.f, 0.f }, { 1280.f, 720.f }));
    window.setView(view);

    CameraController cam;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> ev = window.pollEvent()) {
            cam.handleEvent(window, view, *ev);
        }

        window.setView(view);

        window.clear();
        // draw world...
        sf::CircleShape cir;
        cir.setFillColor(sf::Color::Green);
        cir.setPosition({ 0,0 });
        cir.setRadius(100);
        cir.setPointCount(75);
        //cir.setPointCount(cir.getRadius());
        //std::cout << cir.getPointCount() << "\n";
        window.draw(cir);
        window.display();
    }
}
