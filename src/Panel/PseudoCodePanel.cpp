#include <Panel/PseudoCodePanel.h>

PseudoCodePanel::PseudoCodePanel(const AssetManager& asset_manager, const std::string& font_name):
	asset_manager(asset_manager), 
	current_highlight(-1), 
	minimized(false),
	minimize_button(asset_manager.getFont("Roboto-Regular"), "-", { 0, 0 }, { 30, 30 }, 20),
	maximize_button(asset_manager.getFont("Roboto-Regular"), "+", { 0, 0 }, { 30, 30 }, 20)
{
	position = {600, 600};
	size = {400, 300};
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
	target.draw(minimize_button, states);
	target.draw(maximize_button, states);
	if (minimized) return;
	target.draw(background, states);
	for (int i = 0; i < code_lines.size(); i++) {
		target.draw(code_lines[i], states);
	}
}

void PseudoCodePanel::setCode(const std::vector<std::string>& new_lines) {
	lines = new_lines;
	const sf::Font& code_font = asset_manager.getFont("Consolas-Regular");

	sf::Vector2f line_size = { 0,0 };
	for (int i = 0; i < lines.size(); i++) {
		std::string label = lines[i];
		CodeLine line(label, code_font, CHAR_SIZE);
		line.setColor(DEFAULT_TEXT_COLOR);
		line.setPosition(sf::Vector2f(position.x + PADDING, position.y + PADDING + i * (LINE_HEIGHT + PADDING)));
		line_size.x = std::max(line_size.x, line.getSize().x);
		line_size.y = std::max(line_size.y, line.getSize().y);
		code_lines.push_back(line);
	}
	for (int i = 0; i < code_lines.size(); i++) {
		code_lines[i].setSize(line_size);
	}

}

void PseudoCodePanel::setHighlight(int index) {
	current_highlight = index;
}

void PseudoCodePanel::setPosition(const sf::Vector2f& pos) {
	position = pos;
	background.setPosition(pos);
	for (int i = 0; i < code_lines.size(); i++) {
		code_lines[i].setPosition(sf::Vector2f(position.x + PADDING, position.y + PADDING + i * (LINE_HEIGHT + PADDING)));
	}
}

void PseudoCodePanel::setSize(const sf::Vector2f& size) {
	this->size = size;
	background.setSize(size);
}

void PseudoCodePanel::setBackgroundColor(const sf::Color& color) {
	background.setFillColor(color);
}

void PseudoCodePanel::setTextColor(const sf::Color& color) {
	for (int i = 0; i < code_lines.size(); i++) {
		code_lines[i].setColor(color);
	}
}

bool handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev, const sf::Vector2f& mouse_pos) {
	if (minimize_button.contains(window, view, mouse_pos)) {
		minimized = 1;
		return 0;
	}
	if (maximize_button.contains(window, view, mouse_pos)) {
		minimized = 0;
		return 1;
	}
	return false;
}

void PseudoCodePanel::animateHighlight(float progress, bool isHighlight, int index) {
	if (index >= 0 && index < code_lines.size()) {
		sf::Color target_color = isHighlight ? HIGHLIGHT_TEXT_COLOR : DEFAULT_TEXT_COLOR;
		sf::Color current_color = isHighlight ? DEFAULT_TEXT_COLOR : HIGHLIGHT_TEXT_COLOR;
		sf::Color new_color = lerpColor(current_color, target_color, progress);
		code_lines[index].setColor(new_color);
	}
}

void PseudoCodePanel::update(const sf::RenderWindow& window, const sf::View& view) {
	minimize_button.update(window, view);
	maximize_button.update(window, view);
}