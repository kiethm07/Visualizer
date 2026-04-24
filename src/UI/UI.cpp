#include <UI/UI.h>
#include <iostream>
#include <Model/CodeLine.h>

UI::UI(const AssetManager& a_manager) :
    asset_manager(a_manager),
    main_menu(asset_manager.getFont("Roboto-Regular")),
    ds_menu(asset_manager.getFont("Roboto-Regular")),
    linked_list_ui(asset_manager),
    hashmap_ui(asset_manager),
    trie_ui(asset_manager),
    avl_ui(asset_manager),
	graph_ui(asset_manager)
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
    //window.setFramerateLimit(5); //Debug purpose
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
                if (changed_state) {
                    cam.reset(window, cam_view);
                    cam.setEnable(0);
                    changed_state = 0;
                }
                std::optional<MenuState> chosen = main_menu.handleEvent(window, fixed_view, *ev);
                if (chosen.has_value()) {
                    changed_state = 1;
                    current_state = chosen.value();
                    cam.reset(window, cam_view);
                    break;
                }
            }
            if (current_state == MenuState::DSMenu) {
                if (changed_state) {
                    cam.reset(window, cam_view);
                    cam.setEnable(0);
                    changed_state = 0;
                }
                std::optional<MenuState> chosen = ds_menu.handleEvent(window, fixed_view, *ev);
                if (chosen.has_value()) {
                    changed_state = 1;
                    current_state = chosen.value();
                    cam.reset(window, cam_view);
                    break;
                }
            }
            if (current_state == MenuState::LinkedList) {
                if (changed_state) {
                    linked_list_ui.setInit();
                    cam.reset(window, cam_view);
                    cam.setEnable(1);
                    changed_state = 0;
                }
                linked_list_ui.handleEvent(window, fixed_view, cam_view, cam, *ev);
            }
            if (current_state == MenuState::Hashmap) {
                if (changed_state) {
                    hashmap_ui.setInit();
                    cam.reset(window, cam_view);
                    cam.setEnable(1);
                    changed_state = 0;
                }
                hashmap_ui.handleEvent(window, fixed_view, cam_view, cam, *ev);
            }
            if (current_state == MenuState::Trie) {
                if (changed_state) {
                    trie_ui.setInit();
                    cam.reset(window, cam_view);
                    cam.setEnable(1);
                    changed_state = 0;
                }
                trie_ui.handleEvent(window, fixed_view, cam_view, cam, *ev);
            }
            if (current_state == MenuState::AVL) {
                if (changed_state) {
                    avl_ui.setInit();
                    cam.reset(window, cam_view);
                    cam.setEnable(1);
                    changed_state = 0;
                }
                avl_ui.handleEvent(window, fixed_view, cam_view, cam, *ev);
            }
            if (current_state == MenuState::Graph) {
                if (changed_state) {
                    graph_ui.setInit();
                    cam.reset(window, cam_view);
                    cam.setEnable(1);
                    changed_state = 0;
                }
                graph_ui.handleEvent(window, fixed_view, cam_view, cam, *ev);
			}
        }
        window.clear();
        if (current_state == MenuState::MainMenu) {
            main_menu.update(window, fixed_view);
            window.setView(fixed_view);
            window.draw(main_menu);
			//sf::Font font = asset_manager.getFont("Roboto-Regular");
			//CodeLine title("Data Structure Visualizer", font, 20);
			//title.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 4.f));
			//title.setSize(sf::Vector2f(400, 100));
			//title.setColor(sf::Color(89, 145, 46));
			//window.draw(title);
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
        if (current_state == MenuState::Hashmap) {
            hashmap_ui.update(window, fixed_view, cam_view);
            hashmap_ui.draw(window, fixed_view, cam_view);
        }
        if (current_state == MenuState::Trie) {
            trie_ui.update(window, fixed_view, cam_view);
            trie_ui.draw(window, fixed_view, cam_view);
        }
        if (current_state == MenuState::AVL) {
            avl_ui.update(window, fixed_view, cam_view);
            avl_ui.draw(window, fixed_view, cam_view);
        }
        if (current_state == MenuState::Graph) {
            graph_ui.update(window, fixed_view, cam_view);
            graph_ui.draw(window, fixed_view, cam_view);
		}
        window.display();
    }
}