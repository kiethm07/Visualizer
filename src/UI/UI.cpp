#include <UI/UI.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <Model/Button.h>

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
    sf::Font font;
    if (!font.openFromFile(std::string(ASSET_DIR) + "/fonts/Roboto-Regular.ttf")) {
        std::cout << "Cannot load font!\n";
    }
    while (window.isOpen()) {
        while (const std::optional<sf::Event> ev = window.pollEvent()) {
            cam.handleEvent(window, view, *ev);
        }
        window.setView(view);
        window.clear();
        sf::CircleShape cir;
        cir.setPointCount(75);
        //cir.setOrigin(sf::Vector2f({ (float)DEFAULT_WIDTH / 2, (float)DEFAULT_HEIGHT / 2 }));
        cir.setRadius(100);
        cir.setOrigin(sf::Vector2f({ 100, 100 }));
        cir.setPosition(sf::Vector2f({ (float)DEFAULT_WIDTH / 2, (float)DEFAULT_HEIGHT / 2 }));
        auto t = cir.getOrigin();
        Button start(font, "start", cir.getPosition(), sf::Vector2f({ 300, 300 }), 50);
        start.setPosition(cir.getPosition());
        start.setOrigin(start.getSize() / 2.f);
        window.draw(start);
        window.display();
    }
}