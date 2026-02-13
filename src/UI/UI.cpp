#include <UI/UI.h>
#include <SFML/Graphics.hpp>
#include <iostream>

UI::UI() {
    settings.antiAliasingLevel = 8;

    window.create(
        sf::VideoMode({ DEFAULT_WIDTH, DEFAULT_HEIGHT }),
        "Visualizer",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );

    window.setVerticalSyncEnabled(true);
    window.setMinimumSize(sf::Vector2u{ MIN_WIDTH, MIN_HEIGHT });

    view.setSize(sf::Vector2f({ (float)DEFAULT_WIDTH, (float)DEFAULT_HEIGHT }));
    view.setCenter(sf::Vector2f({ (float)DEFAULT_WIDTH / 2, (float)DEFAULT_HEIGHT / 2 }));

    window.setView(view);
}

void UI::run() {
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