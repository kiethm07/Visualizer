#include <Panel/PseudoCodePanel.h>

PseudoCodePanel::PseudoCodePanel(const AssetManager& asset_manager, const std::string& font_name):
	asset_manager(asset_manager), 
	current_highlight(-1), 
	minimized(false),
	title(asset_manager, "No Operation", { 0, 0 }, TITLE_SIZE, 20),
	minimize_button(asset_manager, "-", { 0, 0 }, BUTTON_SIZE, 20),
	maximize_button(asset_manager, "+", { 0, 0 }, BUTTON_SIZE, 20)
{
	position = { 600, 600 };
	title.setCornerRadius(0);
	minimize_button.setCornerRadius(0);
	maximize_button.setCornerRadius(0);
	updateButtonPosition();
}

void PseudoCodePanel::setTitle(const std::string& title) {
	this->title.setLabel(title);
}

static sf::Color lerpColor(const sf::Color& a, const sf::Color& b, float t) {
	return sf::Color(
		(a.r + t * (b.r - a.r)),
		(a.g + t * (b.g - a.g)),
		(a.b + t * (b.b - a.b)),
		(a.a + t * (b.a - a.a))
	);
}

void PseudoCodePanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//std::cout << title.getSize().x << " " << title.getSize().y << "\n";
	//std::cout << title.getPosition().x << " " << title.getPosition().y << "\n";
	//std::cout << position.x << " " << position.y << "\n";
	//target.draw(background, states);
	target.draw(title, states);
	//std::cout << minimize_button.getSize().x << " " << minimize_button.getSize().y << "\n";
	//std::cout << minimize_button.getPosition().x << " " << minimize_button.getPosition().y << "\n";
	if (minimized) {
		if (hover == 0) {
			target.draw(maximize_button, states);
			target.draw(minimize_button, states);
		}
		else {
			target.draw(minimize_button, states);
			target.draw(maximize_button, states);
		}
		return;
	}
	for (int i = 0; i < code_lines.size(); i++) {
		if (i == current_highlight) {
			CodeLine highlighted_code = code_lines[i];
			highlighted_code.setTextColor(HIGHLIGHT_TEXT_COLOR);
			target.draw(highlighted_code);
			continue;
		}
		target.draw(code_lines[i], states);
		//std::cout << i << "\n";
		//std::cout << code_lines[i].getPosition().x << " " << code_lines[i].getPosition().y << "\n";
		//std::cout << code_lines[i].getSize().x << " " << code_lines[i].getSize().y << "\n";
	}
	if (hover == 0) {
		target.draw(maximize_button, states);
		target.draw(minimize_button, states);
	}
	else {
		target.draw(minimize_button, states);
		target.draw(maximize_button, states);
	}
}

void PseudoCodePanel::setCode(const std::vector<std::string>& new_lines) {
	lines.clear();
	auto int2string = [](int x) -> std::string {
		std::string res = "";
		if (x == 0) return "0";
		while (x > 0) {
			res = char('0' + x % 10) + res;
			x /= 10;
		}
		return res;
		};
	for (int i = 0; i < new_lines.size(); i++) {
		std::string num = int2string(i);
		lines.push_back(num + ". " + new_lines[i]);
	}
	const sf::Font& code_font = asset_manager.getFont("Consola");
	code_lines.clear();

	sf::Vector2f line_size = { 0,0 };
	for (int i = 0; i < lines.size(); i++) {
		std::string label = lines[i];
		CodeLine line(label, code_font, CHAR_SIZE);
		line.setSize(sf::Vector2f(0, LINE_HEIGHT));
		line.setTextColor(DEFAULT_TEXT_COLOR);
		line.setPosition(sf::Vector2f(position.x, position.y + PADDING + i * (LINE_HEIGHT + PADDING) + BUTTON_SIZE.y));
		line_size.x = std::max(line_size.x, line.getSize().x);
		line_size.y = std::max(line_size.y, line.getSize().y);
		code_lines.push_back(line);
	}
	for (int i = 0; i < code_lines.size(); i++) {
		code_lines[i].setSize(line_size);
		//std::cout << i << "\n";
		//std::cout << code_lines[i].getPosition().x << " " << code_lines[i].getPosition().y << "\n";
		//std::cout << code_lines[i].getSize().x << " " << code_lines[i].getSize().y << "\n";
	}
	updateButtonPosition();
	background.setSize(sf::Vector2f(line_size.x + 2 * PADDING, line_size.y * lines.size() + 2 * PADDING + BUTTON_SIZE.y));

}

void PseudoCodePanel::setHighlight(int index) {
	current_highlight = index;
	//int i = current_highlight;
	//if (i >= 0 && i < code_lines.size()) {
	//	code_lines[i].setTextColor(HIGHLIGHT_TEXT_COLOR);
	//}
} 

void PseudoCodePanel::setPosition(const sf::Vector2f& pos) {
	position = pos;
	background.setPosition(pos);
	for (int i = 0; i < code_lines.size(); i++) {
		code_lines[i].setPosition(sf::Vector2f(position.x + PADDING, position.y + BUTTON_SIZE.y + i * (LINE_HEIGHT + PADDING)));
	}
	updateButtonPosition();
}

void PseudoCodePanel::updateButtonPosition() {
	//title.x + min.x + max.x = max(lines_size.x + 2 * PADDING)
	sf::Vector2f lines_size = sf::Vector2f( 0, BUTTON_SIZE.y );
	for (int i = 0; i < code_lines.size(); i++) {
		lines_size.x = std::max(lines_size.x, code_lines[i].getSize().x);
	}
	title.setPosition(sf::Vector2f(position.x, position.y));
	title.setButtonSize(sf::Vector2f(std::max(TITLE_SIZE.x, lines_size.x + 2 * PADDING - 2 * BUTTON_SIZE.x), TITLE_SIZE.y));
	//std::cout << title.getSize().x << " " << title.getSize().y << "\n";
	//std::cout << title.getPosition().x << " " << title.getPosition().y << "\n";
	minimize_button.setPosition(sf::Vector2f(position.x + title.getSize().x, position.y));
	maximize_button.setPosition(sf::Vector2f(position.x + title.getSize().x + BUTTON_SIZE.x, position.y));
	minimize_button.setButtonSize(BUTTON_SIZE);
	maximize_button.setButtonSize(BUTTON_SIZE);

	sf::Vector2f bg_size = { title.getSize().x + 2 * BUTTON_SIZE.x, BUTTON_SIZE.y + code_lines.size() * (LINE_HEIGHT + PADDING) };
	background.setSize(bg_size);
}

//void PseudoCodePanel::setSize(const sf::Vector2f& size) {
//	this->size = size;
//	background.setSize(size);
//}

void PseudoCodePanel::setBackgroundColor(const sf::Color& color) {
	background.setFillColor(color);
}

void PseudoCodePanel::setTextColor(const sf::Color& color) {
	for (int i = 0; i < code_lines.size(); i++) {
		code_lines[i].setTextColor(color);
	}
}

int PseudoCodePanel::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	if (const auto* mb_pressed = ev.getIf<sf::Event::MouseButtonPressed>()) {
		if (mb_pressed->button == sf::Mouse::Button::Left) {
			sf::Vector2f mouse_pos = window.mapPixelToCoords(mb_pressed->position, view);
			if (title.contains(window, view, mouse_pos) ||
				minimize_button.contains(window, view, mouse_pos) ||
				maximize_button.contains(window, view, mouse_pos)) {
				is_dragging = true;
				drag_offset = position - mouse_pos;
			}
		}
	}
	else if (const auto* mm = ev.getIf<sf::Event::MouseMoved>()) {
		if (is_dragging) {
			sf::Vector2f mouse_pos = window.mapPixelToCoords(mm->position, view);
			sf::Vector2f new_pos = mouse_pos + drag_offset;

			float max_x = window.getSize().x - background.getSize().x;
			float max_y = window.getSize().y - background.getSize().y;

			if (new_pos.x < 0.f) new_pos.x = 0.f;
			else if (new_pos.x > max_x) new_pos.x = max_x;

			if (new_pos.y < 0.f) new_pos.y = 0.f;
			else if (new_pos.y > max_y) new_pos.y = max_y;

			setPosition(new_pos);
		}
	}
	else if (const auto* mb = ev.getIf<sf::Event::MouseButtonReleased>()) {
		if (mb->button == sf::Mouse::Button::Left) {
			is_dragging = false;
			sf::Vector2f mouse_pos = window.mapPixelToCoords(mb->position, view);
			if (minimize_button.contains(window, view, mouse_pos)) {
				minimized = 1;
				return 0;
			}
			if (maximize_button.contains(window, view, mouse_pos)) {
				minimized = 0;
				return 1;
			}
		}
	}
	return -1;
}

void PseudoCodePanel::animateHighlight(float progress, bool isHighlight, int index) {
	if (index >= 0 && index < code_lines.size()) {
		sf::Color target_color = isHighlight ? HIGHLIGHT_TEXT_COLOR : DEFAULT_TEXT_COLOR;
		sf::Color current_color = isHighlight ? DEFAULT_TEXT_COLOR : HIGHLIGHT_TEXT_COLOR;
		sf::Color new_color = lerpColor(current_color, target_color, progress);
		code_lines[index].setTextColor(new_color);
	}
}

void PseudoCodePanel::update(const sf::RenderWindow& window, const sf::View& view) {
	minimize_button.update(window, view);
	maximize_button.update(window, view);
	if (minimize_button.getHover()) {
		hover = 0;
	}
	if (maximize_button.getHover()) {
		hover = 1;
	}
}