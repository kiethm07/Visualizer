#include <Panel/TimelinePanel.h>

TimelinePanel::TimelinePanel(const AssetManager& a_manager) :
    a_manager(a_manager),
    FONT(a_manager.getFont("Roboto-Regular")),
    play(FONT, "Play", {}, {}, 20),
    one_phase_forward(FONT, ">", {}, {}, 20),
    one_phase_backward(FONT, "<", {}, {}, 20),
    one_step_forward(FONT, ">>", {}, {}, 20),
    one_step_backward(FONT, "<<", {}, {}, 20),
    skip_to_last(FONT, "|>", {}, {}, 20),
    skip_to_init(FONT, "<|", {}, {}, 20),
    auto_play(FONT, "Auto play", {}, {}, 20)
{
    background.setFillColor(sf::Color::Blue);
    background.setOrigin({ 0,0 });
    background.setPosition({ 0,0 });
    background.setSize({ 0,0 });
}

std::optional<TimelineConfig> TimelinePanel::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
    TimelineConfig timeline_config;
    if (const auto* mb = ev.getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2f mouse_pos = sf::Vector2f(mb->position);
        if (play.contains(window, view, mouse_pos)) {
            return TimelineConfig::play();
        }
        if (auto_play.contains(window, view, mouse_pos)) {
            return TimelineConfig::autoPlay();
        }
        if (one_phase_forward.contains(window, view, mouse_pos)) {
            return TimelineConfig::onePhaseForward();
        }
        if (one_phase_backward.contains(window, view, mouse_pos)) {
            return TimelineConfig::onePhaseBackward();
        }
        if (one_step_forward.contains(window, view, mouse_pos)) {
            return TimelineConfig::oneStepForward();
        }
        if (one_step_backward.contains(window, view, mouse_pos)) {
            return TimelineConfig::oneStepBackward();
        }
        if (skip_to_last.contains(window, view, mouse_pos)) {
            return TimelineConfig::toLast();
        }
        if (skip_to_init.contains(window, view, mouse_pos)) {
            return TimelineConfig::toInit();
        }
    }
    return std::nullopt;
}

void TimelinePanel::update(const sf::RenderWindow& window, const sf::View& view) {
    updateButtonState(window, view);
    updateWindowState(window, view);
}

void TimelinePanel::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
    play.update(window, view);
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
        static_cast<float>(size.y) * 0.2f
    };

    background.setSize(background_size);
    background.setPosition({ 0.f, static_cast<float>(size.y) - background_size.y });

    sf::Vector2f button_size = {
        background_size.x * 0.09f,
        background_size.y * 0.28f
    };

    float gap = background_size.x * 0.015f;

    float center_x = background.getPosition().x + background_size.x / 2.f;

    float top_y = background.getPosition().y + background_size.y * 0.35f;
    float bottom_y = background.getPosition().y + background_size.y * 0.75f;

    auto place = [&](Button& b, float x, float y) {
        b.setButtonSize(button_size);
        b.setOrigin(button_size / 2.f);
        b.setPosition({ x, y });
    };

    float delta = button_size.x + gap;

    // Center play
    place(play, center_x, top_y);

    // Phase (near center)
    place(one_phase_backward, center_x - delta, top_y);
    place(one_phase_forward, center_x + delta, top_y);

    // Step (farther)
    place(one_step_backward, center_x - 2.f * delta, top_y);
    place(one_step_forward, center_x + 2.f * delta, top_y);

    // Skip (farthest)
    place(skip_to_init, center_x - 3.f * delta, top_y);
    place(skip_to_last, center_x + 3.f * delta, top_y);

    // Auto play (bottom center)
    place(auto_play, center_x, bottom_y);
}