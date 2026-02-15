#include <UI/UI.h>
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
    sf::Font font;
    if (!font.openFromFile(std::string(ASSET_DIR) + "/fonts/Roboto-Regular.ttf")) {
        std::cout << "Cannot load font!\n";
    }
    while (window.isOpen()) {
        Button start(font, "Halo", { 0.f,0.f }, { 100.f,100.f }, 20);
        while (const std::optional<sf::Event> ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) window.close();
            if (current_state == MenuState::Menu) {
                cam.setEnable(false);
            }
            cam.handleEvent(window, view, *ev);
            if (const auto* keyPressed = ev->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }
        }
        start.update(window, view);
        window.setView(view);
        window.clear();
        window.draw(start);
        window.display();
    }
}