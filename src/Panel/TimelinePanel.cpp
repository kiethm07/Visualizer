#include <Panel/TimelinePanel.h>

TimelinePanel::TimelinePanel(const AssetManager& a_manager) :
    a_manager(a_manager),
    FONT(a_manager.getFont("Roboto-Regular")),
    auto_play(FONT, "Play", {}, {}, 20),
    one_phase_forward(FONT, ">", {}, {}, 20),
    one_phase_backward(FONT, "<", {}, {}, 20),
    one_step_forward(FONT, ">>", {}, {}, 20),
    one_step_backward(FONT, "<<", {}, {}, 20),
    skip_to_last(FONT, "|>", {}, {}, 20),
    skip_to_init(FONT, "<|", {}, {}, 20)
{
    background.setFillColor(sf::Color::Blue);
    background.setOrigin({ 0,0 });
    background.setPosition({ 0,0 });
    background.setSize({ 0,0 });
}

void TimelinePanel::update(const sf::RenderWindow& window, const sf::View& view) {
    updateButtonState(window, view);
    updateWindowState(window, view);
}

void TimelinePanel::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
    auto_play.update(window, view);
    one_phase_forward.update(window, view);
    one_phase_backward.update(window, view);
    one_step_forward.update(window, view);
    one_step_backward.update(window, view);
    skip_to_last.update(window, view);
    skip_to_init.update(window, view);
}

void TimelinePanel::updateWindowState(const sf::RenderWindow& window, const sf::View& view) {
    sf::Vector2u size = window.getSize();

    sf::Vector2f background_size = {
        static_cast<float>(size.x),
        static_cast<float>(size.y) * 0.15f
    };

    background.setSize(background_size);
    background.setPosition({ 0.f, static_cast<float>(size.y) - background_size.y });

    sf::Vector2f button_size = {
        background_size.x * 0.09f,
        background_size.y * 0.38f
    };

    float gap = background_size.x * 0.015f;

    float center_x = background.getPosition().x + background_size.x / 2.f;
    float center_y = background.getPosition().y + background_size.y / 2.f;

    auto place_button = [&](Button& button, float x) {
        button.setButtonSize(button_size);
        button.setOrigin(button_size / 2.f);
        button.setPosition({ x, center_y });
        };

    float delta = button_size.x + gap;

    // Center
    place_button(auto_play, center_x);

    // Phase: closer to center
    place_button(one_phase_backward, center_x - delta);
    place_button(one_phase_forward, center_x + delta);

    // Step: farther than phase
    place_button(one_step_backward, center_x - 2.f * delta);
    place_button(one_step_forward, center_x + 2.f * delta);

    // Skip: farthest
    place_button(skip_to_init, center_x - 3.f * delta);
    place_button(skip_to_last, center_x + 3.f * delta);
}