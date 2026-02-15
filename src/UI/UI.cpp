#include <UI/UI.h>
#include <iostream>

UI::UI(const AssetManager& a_manager) : 
    asset_manager(a_manager),
    menu(asset_manager.getFont("Roboto-Regular"))
{
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
    std::cout << view.getSize().x << " " << view.getSize().y << "\n";
    while (window.isOpen()) {
        while (const std::optional<sf::Event> ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) window.close();
            if (const auto* keyPressed = ev->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }
            if (current_state == MenuState::Menu) {
                cam.setEnable(false);
            }
            //cam.handleEvent(window, view, *ev);
        }
        view.setSize(sf::Vector2f(window.getSize()));
        view.setCenter(sf::Vector2f(window.getSize()) / 2.f);
        window.setView(view);
        window.clear();
        if (current_state == MenuState::Menu) {
            menu.update(window);
            window.draw(menu);
        }
        window.display();
    }
}