#include <UI/UI.h>
#include <iostream>

UI::UI(const AssetManager& a_manager) :
    asset_manager(a_manager),
    main_menu(asset_manager.getFont("Roboto-Regular")),
    ds_menu(asset_manager.getFont("Roboto-Regular")),
    linked_list_ui(asset_manager)
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

    cam_view.setSize(sf::Vector2f({ (float)DEFAULT_WIDTH, (float)DEFAULT_HEIGHT }));
    cam_view.setCenter(sf::Vector2f({ (float)DEFAULT_WIDTH / 2, (float)DEFAULT_HEIGHT / 2 }));
    fixed_view.setSize(sf::Vector2f({ (float)DEFAULT_WIDTH, (float)DEFAULT_HEIGHT }));
    fixed_view.setCenter(sf::Vector2f({ (float)DEFAULT_WIDTH / 2, (float)DEFAULT_HEIGHT / 2 }));
    window.setView(fixed_view);
}

void UI::run() {
    while (window.isOpen()) {
        while (const std::optional<sf::Event> ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) window.close();
            if (ev->is<sf::Event::Resized>()) {
                fixed_view.setSize(sf::Vector2f(window.getSize()));
                fixed_view.setCenter(sf::Vector2f(window.getSize()) / 2.f);
            }
            cam.handleEvent(window, cam_view, *ev);
            //std::cout << cam_view.getSize().x << " " << cam_view.getSize().y << "\n";
            if (const auto* keyPressed = ev->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Home) {
                    current_state = MenuState::MainMenu;
                    break;
                }
            }
            if (current_state == MenuState::MainMenu) {
                cam.setEnable(false);
                std::optional<MenuState> chosen = main_menu.handleEvent(window, fixed_view, *ev);
                if (chosen.has_value()) {
                    current_state = chosen.value();
                    cam.reset(window, cam_view);
                    break;
                }
            }
            if (current_state == MenuState::DSMenu) {
                cam.setEnable(false);
                std::optional<MenuState> chosen = ds_menu.handleEvent(window, fixed_view, *ev);
                if (chosen.has_value()) {
                    current_state = chosen.value();
                    cam.reset(window, cam_view);
                    break;
                }
            }
            if (current_state == MenuState::LinkedList) {
                cam.setEnable(true);
                linked_list_ui.handleEvent(window, fixed_view, *ev);
            }
        }
        window.clear();
        if (current_state == MenuState::MainMenu) {
            main_menu.update(window, fixed_view);
            window.setView(fixed_view);
            window.draw(main_menu);
        }
        if (current_state == MenuState::DSMenu) {
            ds_menu.update(window, fixed_view);
            window.setView(fixed_view);
            window.draw(ds_menu);
        }
        if (current_state == MenuState::LinkedList) {
            linked_list_ui.update(window, fixed_view, cam_view);
            linked_list_ui.draw(window, fixed_view, cam_view);
        }
        window.display();
    }
}