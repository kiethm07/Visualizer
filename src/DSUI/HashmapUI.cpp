#include <DSUI/HashmapUI.h>
#include <iostream>
#include <unordered_map>
#include <random>

static std::mt19937 rng(6969);

static int rand(int l, int r) {
	if (l > r) std::swap(l, r);
	return std::uniform_int_distribution<int>(l, r)(rng);
}

HashmapUI::HashmapUI(const AssetManager& a_manager) :
	a_manager(a_manager),
	NODE_FONT(a_manager.getFont("Roboto-Regular")),
	panel(NODE_FONT),
	timeline(a_manager),
	timeline_panel(a_manager),
	ui_state(UIState::Init),
	init_panel(a_manager),
	code_panel(a_manager, "Consola")
{
	init_panel.setPlaceHolderForManualInput("Input value manually, format: bucket_count, values");
}

void HashmapUI::update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	if (ui_state == UIState::Init) {
		init_panel.update(window, fixed_view);
	}
	else if (ui_state == UIState::Running) {
		panel.update(window, fixed_view);
		timeline_panel.update(window, fixed_view);
		timeline.update(clock.restart().asSeconds());
		std::optional<HashmapOperation> current_operation = timeline.getCurrentOperation();
		std::optional<HashmapOperationType> type = std::nullopt;
		if (current_operation.has_value()) type = current_operation->type;
		int highlighted_line = timeline.getHighlightedLine();
		//std::cout << highlighted_line << "\n";
		code_panel.sync(type, highlighted_line);
		code_panel.update(window, fixed_view);
	}
}

void HashmapUI::Init(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const PanelData& data) {
	ui_state = UIState::Running;
	cam.reset(window, cam_view);
	//for (int i : data.values) std::cout << i << " "; std::cout << "\n";
	auto parseValues = [](const std::vector<std::string>& v) -> std::vector<int> {
		auto parseString = [](const std::string& s) -> std::optional<int> {
			if (s.empty()) return std::nullopt;
			std::string t = "";
			bool sign = 0;
			if (s[0] == '-') sign = 1;
			for (int i = 0; i < s.size(); i++) {
				if (s[i] > '9' || s[i] < '0') continue;
				t += s[i];
				if (t.size() >= 5) break;
			}
			int res = 0;
			if (t.empty()) return std::nullopt;
			for (int i = 0; i < t.size(); i++) {
				res = res * 10 + t[i] - '0';
			}
			if (sign) res = -res;
			return res;
		};
		std::vector<int> res;
		for (const std::string& s : v) {
			std::optional<int> val = parseString(s);
			if (val.has_value()) res.push_back(*val);
		}
		return res;
	};
	std::vector<int> values = parseValues(data.values);
	if (data.operation == PanelOperation::Empty) {
		//Do nothing
		hashmap.rawInit(DEFAULT_BUCKET_COUNT, {});
	}
	else if (data.operation == PanelOperation::Random) {
		std::vector<int> v;
		int num = rand(15, 20);
		int bucket_count = rand(7, 13);
		std::unordered_map<int, bool> used;
		for (int i = 0; i < num; i++) {
			int tmp = rand(-20, 20);
			if (used.find(tmp) != used.end()) continue;
			used[tmp] = 1;
			v.push_back(tmp);
		}
		hashmap.rawInit(bucket_count, v);
	}
	else if (data.operation == PanelOperation::Manual) {
		if (values[0] < 0) values[0] = -values[0];
		int bucket_count = values[0];
		if (bucket_count == 0) bucket_count = DEFAULT_BUCKET_COUNT;
		values.erase(values.begin());
		hashmap.rawInit(bucket_count, values);
	}
	else if (data.operation == PanelOperation::File) {
		if (values[0] < 0) values[0] = -values[0];
		int bucket_count = values[0];
		values.erase(values.begin());
		hashmap.rawInit(bucket_count, values);
	}
	current_state = hashmap.getState();
	timeline.clear();
	timeline.setInitialState(current_state);
	timeline.generateAnimation(current_state, current_state, HashmapRecorder());
}

std::optional<MenuState> HashmapUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev) {
	if (ui_state == UIState::Init) {
		std::optional<PanelData> panel_data = init_panel.handleEvent(window, view, ev);
		if (!panel_data.has_value()) return std::nullopt;
		Init(window, view, cam_view, cam, *panel_data);
		return std::nullopt;
	}
	if (ui_state == UIState::Running) {
		code_panel.handleEvent(window, view, ev);
		if (const auto op = panel.handleEvent(window, view, ev); op.has_value()) {
			if (op->type == HashmapOperationType::Home) {
				return MenuState::DSMenu;
			}
			else if (op->type == HashmapOperationType::Setting) {
				return MenuState::Setting;
			}
			else if (op->type == HashmapOperationType::Load) {
				if (op->file_path.empty()) return std::nullopt;
				hashmap.applyOperation(*op, HashmapRecorder());
				current_state = hashmap.getState();
				timeline.clear();
				timeline.setInitialState(current_state);
				timeline.generateAnimation(current_state, current_state, HashmapRecorder());
			}
			else {
				hashmap.applyOperation(*op, recorder);
				HashmapState prev = current_state;
				current_state = hashmap.getState();
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


void HashmapUI::draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	if (ui_state == UIState::Init) {
		window.setView(fixed_view);
		window.draw(init_panel);
	}

	if (ui_state == UIState::Running) {
		window.setView(cam_view);
		timeline.draw(window, cam_view);

		window.setView(fixed_view);
		window.draw(timeline_panel);
		window.draw(panel);
		window.draw(code_panel);
	}
}