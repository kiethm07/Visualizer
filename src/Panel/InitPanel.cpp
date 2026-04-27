#include <Panel/InitPanel.h>

InitPanel::InitPanel(const AssetManager& a_manager) :
	a_manager(a_manager),
	FONT(a_manager.getFont("Roboto-Regular")),
	manual_input(a_manager, "Input value manually, format : ", {}, {}, 30),
	random(a_manager, "Random data", {}, {}, 30),
	from_file(a_manager, "Choose init data from file", {}, {}, 30),
	empty(a_manager, "Start with empty data", {}, {}, 30),
	background(a_manager.getTexture("Landscape"))
{
	manual_input.setMaxLength(696969);
}

void InitPanel::setPlaceHolderForManualInput(const std::string& s) {
	manual_input.setPlaceholder(s);
}

void InitPanel::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	manual_input.update(window, view);
	random.update(window, view);
	empty.update(window, view);
	from_file.update(window, view);
}

void InitPanel::updateWindowState(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2u size = window.getSize();

	sf::Vector2u windowSize = window.getSize();

	float scaleX = (float)windowSize.x / background.getTexture().getSize().x;
	float scaleY = (float)windowSize.y / background.getTexture().getSize().y;

	background.setScale({ scaleX, scaleY });

	sf::Vector2f button_size = { size.x * 0.8f, size.y * 0.12f };

	const float center_x = view.getCenter().x;
	const float start_y = view.getCenter().y - size.y * 0.3f;
	const float gap = size.y * 0.2f;

	auto place_button = [&](auto& btn, int index) {
		btn.setButtonSize(button_size);
		btn.setOrigin(button_size / 2.f);
		btn.setPosition({ center_x, start_y + gap * index });
		};

	place_button(manual_input, 0);
	place_button(random, 1);
	place_button(empty, 2);
	place_button(from_file, 3);
}

void InitPanel::update(const sf::RenderWindow& window, const sf::View& view) {
	updateButtonState(window, view);
	updateWindowState(window, view);
}

static std::vector<std::string> parseInput(const std::string& s) {
	std::vector<std::string> v;
	std::string token;
	auto isValidToken = [](const std::string& s) {
		if (s.empty()) return 0;
		////if (s.size() > 5) return 0;
		//int start = 0;
		//if (s[0] == '-') {
		//	if (s.size() == 1) return 0;
		//	start = 1;
		//}
		//for (int i = start; i < s.size(); i++) {
		//	if ((s[i] > '9' || s[i] < '0') &&
		//		(s[i] > 'z' || s[i] < 'a') &&
		//		(s[i] > 'Z' || s[i] < 'A')) return 0;
		//}
		return 1;
		};
	auto toInt = [](const std::string& s) {
		int sign = 1;
		int i = 0;
		if (s[0] == '-') {
			sign = -1;
			i = 1;
		}
		int value = 0;
		for (; i < s.size(); i++) {
			value = value * 10 + (s[i] - '0');
		}
		return sign * value;
		};

	auto flush_token = [&]() {
		if (token.empty()) return;
		//if (token.size() <= 5 && isValidToken(token)) {
		//	v.push_back(toInt(token));
		//}
		if (isValidToken(token)) v.push_back(token);
		token.clear();
		};

	for (char c : s) {
		if (c == ',' || c == '[' || c == ']' || c == ' ' || c == '\''){
			flush_token();
		}
		else {
			token += c;
		}
	}
	flush_token();
	return v;
}

std::optional<PanelData> InitPanel::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	manual_input.handleEvent(window, view, ev);

	if (random.handleEvent(window, view, ev)) {
		return PanelData{ PanelOperation::Random, {} };
	}

	if (empty.handleEvent(window, view, ev)) {
		return PanelData{ PanelOperation::Empty, {} };
	}

	if (manual_input.mousePressed(window, view, ev)) {
		const std::string& s = manual_input.getValue();
		if (s.empty()) return std::nullopt;
		PanelData data;
		data.operation = PanelOperation::Manual;
		data.values = parseInput(manual_input.getValue());
		return data;
	}

	if (from_file.handleEvent(window, view, ev)) {
		std::string path = cr::utils::SimpleFileDialog::dialog();
		if (path.empty()) {
			std::cout << "No file selected\n";
			return std::nullopt;
		}
		PanelData data;
		data.operation = PanelOperation::File;
		std::ifstream cin(path);
		if (!cin.is_open()) {
			std::cout << "Cannot open file!\n";
			return std::nullopt;
		}
		std::string line;
		while (std::getline(cin, line)) {
			std::vector<std::string> values = parseInput(line);
			for (const std::string& i : values) data.values.push_back(i);
		}
		return data;
	}

	return std::nullopt;
}