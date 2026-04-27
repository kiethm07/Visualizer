#include <DSUI/TrieUI.h>
#include <iostream>
#include <random>
#include <unordered_map>
static std::mt19937 rng(6969);
static int rand(int l, int r) {
	if (l > r) std::swap(l, r);
	return std::uniform_int_distribution<int>(l, r)(rng);
}
TrieUI::TrieUI(const AssetManager& a_manager) :
	a_manager(a_manager),
	NODE_FONT(a_manager.getFont("Roboto-Regular")),
	panel(a_manager),
	timeline(a_manager),
	timeline_panel(a_manager),
	ui_state(UIState::Init),
	init_panel(a_manager),
	code_panel(a_manager, "Consola"),
	background(a_manager.getTexture("NightSky"))
{
	init_panel.setPlaceHolderForManualInput("Input strings (comma separated)");
}
void TrieUI::update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	if (ui_state == UIState::Init) {
		init_panel.update(window, fixed_view);
	}
	else if (ui_state == UIState::Running) {
		panel.update(window, fixed_view);
		timeline_panel.update(window, fixed_view);
		timeline.update(clock.restart().asSeconds());
		std::optional<TrieOperation> current_operation = timeline.getCurrentOperation();
		std::optional<TrieOperationType> type = std::nullopt;
		if (current_operation.has_value()) type = current_operation->type;
		int highlighted_line = timeline.getHighlightedLine();
		//std::cout << highlighted_line << "\n";
		code_panel.sync(type, highlighted_line);
		code_panel.update(window, fixed_view);

		sf::Vector2u windowSize = window.getSize();

		float scaleX = (float)windowSize.x / background.getTexture().getSize().x;
		float scaleY = (float)windowSize.y / background.getTexture().getSize().y;

		background.setScale({ scaleX, scaleY });
	}
}
void TrieUI::Init(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const PanelData& data) {
	ui_state = UIState::Running;
	cam.reset(window, cam_view);
	auto parseValues = [](const std::vector<std::string>& v) -> std::vector<std::string> {
		auto parseString = [](const std::string& s) -> std::optional<std::string> {
			std::string t = "";
			for (const char& c : s) {
				if (c >= 'a' && c <= 'z') t += c - 'a' + 'A';
				else if (c >= 'A' && c <= 'Z') t += c;
			}
			if (t.empty()) return std::nullopt;
			return t;
		};
		std::vector<std::string> res;
		for (const std::string& s : v) {
			std::optional<std::string> val = parseString(s);
			if (val.has_value()) res.push_back(*val);
		}
		return res;
	};
	std::vector<std::string> values = parseValues(data.values);
	if (data.operation == PanelOperation::Empty) {
		trie.rawInit({});
	}
	else if (data.operation == PanelOperation::Random) {
		std::vector<std::string> v;
		int num = rand(6, 9);
		std::unordered_map < std::string, bool> used;
		for (int i = 0; i < num; i++) {
			std::string s = "";
			int len = rand(3, 6);
			for (int j = 0; j < len; j++) s += (char)('A' + rand(0, 25));
			if (used.find(s) != used.end()) continue;
			used[s] = 1;
			v.push_back(s);
		}
		trie.rawInit(v);
	}
	else if (data.operation == PanelOperation::Manual || data.operation == PanelOperation::File) {
		trie.rawInit(values);
	}
	current_state = trie.getState();
	timeline.clear();
	timeline.setInitialState(current_state);
	TrieRecorder dummy;
	timeline.generateAnimation(current_state, current_state, dummy);
}
std::optional<MenuState> TrieUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev) {
	if (ui_state == UIState::Init) {
		std::optional<PanelData> panel_data = init_panel.handleEvent(window, view, ev);
		if (!panel_data.has_value()) return std::nullopt;
		Init(window, view, cam_view, cam, *panel_data);
		return std::nullopt;
	}
	if (ui_state == UIState::Running) {
		code_panel.handleEvent(window, view, ev);
		if (const auto op = panel.handleEvent(window, view, ev); op.has_value()) {
			if (op->type == TrieOperationType::Home) {
				return MenuState::DSMenu;
			}
			else if (op->type == TrieOperationType::Setting) {
				return MenuState::Setting;
			}
			else if (op->type == TrieOperationType::Load) {
				if (op->file_path.empty()) return std::nullopt;
				TrieRecorder dummy;
				trie.applyOperation(*op, dummy);
				current_state = trie.getState();
				timeline.clear();
				timeline.setInitialState(current_state);
				timeline.generateAnimation(current_state, current_state, dummy);
			}
			else {
				trie.applyOperation(*op, recorder);
				TrieState prev = current_state;
				current_state = trie.getState();
				timeline.push(prev, current_state, *op, recorder);
			}
			recorder.clear();
		}
		if (const auto op = timeline_panel.handleEvent(window, view, cam_view, cam, ev)) {
			if (op->type == TimelineOperation::Play) {
				if (timeline.isRunning()) {
					timeline.pause();
				}
				else timeline.run();
			}
			else if (op->type == TimelineOperation::AutoPlay) {
				bool flag = timeline.isAutoPlaying() ^ 1;
				timeline.setAutoPlay(flag);
			}
			else if (op->type == TimelineOperation::OnePhaseForward) {
				timeline.onePhaseForward();
			}
			else if (op->type == TimelineOperation::OnePhaseBackward) {
				timeline.onePhaseBackward();
			}
			else if (op->type == TimelineOperation::OneStepForward) {
				timeline.oneStepForward();
			}
			else if (op->type == TimelineOperation::OneStepBackward) {
				timeline.oneStepBackward();
			}
			else if (op->type == TimelineOperation::LastState) {
				timeline.toLast();
			}
			else if (op->type == TimelineOperation::InitState) {
				timeline.toInit();
			}
			else if (op->type == TimelineOperation::ChangeSpeed) {
				timeline.setSpeed(op->speed);
			}
		}
		return std::nullopt;
	}
}

void TrieUI::draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	if (ui_state == UIState::Init) {
		window.setView(fixed_view);
		window.draw(init_panel);
	}
	if (ui_state == UIState::Running) {
		window.setView(fixed_view);
		window.draw(background);
		window.setView(cam_view);
		timeline.draw(window, cam_view);
		window.setView(fixed_view);
		window.draw(timeline_panel);
		window.draw(panel);
		window.draw(code_panel);
	}
}