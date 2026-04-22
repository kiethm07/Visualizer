#include <Model/Slider.h>
#include <algorithm>
#include <cmath>

Slider::Slider(const sf::Vector2f pos, const sf::Vector2f size, float min_val, float max_val, float initial_val)
	: min_value(min_val), max_value(max_val), current_value(initial_val) {

	current_value = std::clamp(current_value, min_value, max_value);

	track.setPosition(pos);
	track.setSize(size);
	track.setFillColor(TRACK_COLOR);
	track.setOutlineThickness(1);
	track.setOutlineColor(sf::Color::White);

	fill_track.setPosition(pos);
	fill_track.setSize({ 0.f, size.y });
	fill_track.setFillColor(FILL_COLOR);

	float knob_radius = size.y > 10.f ? size.y * 0.8f : 8.f;
	knob.setRadius(knob_radius);
	knob.setOrigin({ knob_radius, knob_radius });
	knob.setFillColor(KNOB_COLOR);

	updateVisuals();
}

void Slider::setPosition(const sf::Vector2f& pos) {
	track.setPosition(pos);
	fill_track.setPosition(pos);
	updateVisuals();
}

sf::Vector2f Slider::getPosition() const {
	return track.getPosition();
}

void Slider::setSliderSize(const sf::Vector2f& size) {
	track.setSize(size);
	updateVisuals();
}

sf::Vector2f Slider::getSize() const {
	return track.getSize();
}

void Slider::setOrigin(const sf::Vector2f& origin) {
	track.setOrigin(origin);
	fill_track.setOrigin(origin);
	updateVisuals();
}

sf::Vector2f Slider::getOrigin() const {
	return track.getOrigin();
}

void Slider::setValue(float val) {
	current_value = std::clamp(val, min_value, max_value);
	updateVisuals();
}

float Slider::getValue() const {
	return current_value;
}

bool Slider::getIsDragging() const {
	return is_dragging;
}

sf::FloatRect Slider::getGlobalBounds() const {
	return track.getGlobalBounds();
}

bool Slider::contains(const sf::Vector2f& position) const {
	sf::FloatRect bounds = track.getGlobalBounds();
	bounds.position.y -= knob.getRadius();
	bounds.size.y += knob.getRadius() * 2.f;
	return bounds.contains(position);
}

void Slider::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	if (const auto* mouse_press = ev.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse_press->button == sf::Mouse::Button::Left) {
			sf::Vector2f mouse_world = window.mapPixelToCoords(mouse_press->position, view);
			if (contains(mouse_world)) {
				is_dragging = 1;

				sf::Vector2f track_pos = track.getPosition() - track.getOrigin();
				float mouse_x = std::clamp(mouse_world.x, track_pos.x, track_pos.x + track.getSize().x);
				float percent = (mouse_x - track_pos.x) / track.getSize().x;
				current_value = min_value + percent * (max_value - min_value);
				updateVisuals();
			}
		}
	}
	else if (const auto* mouse_release = ev.getIf<sf::Event::MouseButtonReleased>()) {
		if (mouse_release->button == sf::Mouse::Button::Left) {
			is_dragging = 0;
		}
	}
	else if (const auto* mouse_move = ev.getIf<sf::Event::MouseMoved>()) {
		sf::Vector2f mouse_world = window.mapPixelToCoords(mouse_move->position, view);
		hovered = contains(mouse_world);

		if (is_dragging) {
			sf::Vector2f track_pos = track.getPosition() - track.getOrigin();
			float mouse_x = std::clamp(mouse_world.x, track_pos.x, track_pos.x + track.getSize().x);
			float percent = (mouse_x - track_pos.x) / track.getSize().x;
			current_value = min_value + percent * (max_value - min_value);
			updateVisuals();
		}
	}

	fill_track.setFillColor(hovered || is_dragging ? HOVER_COLOR : FILL_COLOR);
}

void Slider::updateVisuals() {
	float percent = (current_value - min_value) / (max_value - min_value);
	fill_track.setSize({ track.getSize().x * percent, track.getSize().y });

	sf::Vector2f track_pos = track.getPosition() - track.getOrigin();
	float knob_x = track_pos.x + track.getSize().x * percent;
	float knob_y = track_pos.y + track.getSize().y / 2.f;

	knob_x = std::lround(knob_x);
	knob_y = std::lround(knob_y);

	knob.setPosition({ knob_x, knob_y });
}