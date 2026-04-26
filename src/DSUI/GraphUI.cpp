#include <DSUI/GraphUI.h>
#include <iostream>

static std::mt19937 rng(6969);

static int rand(int l, int r) {
	if (l > r) std::swap(l, r);
	return std::uniform_int_distribution<int>(l, r)(rng);
}

GraphUI::GraphUI(const AssetManager& a_manager) :
	a_manager(a_manager),
	NODE_FONT(a_manager.getFont("Roboto-Regular")),
	panel(NODE_FONT),
	timeline(a_manager),
	timeline_panel(a_manager),
	ui_state(UIState::Init),
	init_panel(a_manager),
	code_panel(a_manager, "Consola")
{
	init_panel.setPlaceHolderForManualInput("Manual input: (nodes count, edge info [u, v, w])");
}

void GraphUI::update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	if (ui_state == UIState::Init) {
		init_panel.update(window, fixed_view);
	}
	else if (ui_state == UIState::Running) {
		panel.update(window, fixed_view);
		timeline_panel.update(window, fixed_view);
		//test.setPosition(panel.getSize() + sf::Vector2f({ 100.f, -300.f }));
		//test.update(window, cam_view);
		//renderer.update(window, cam_view);
		timeline.update(clock.restart().asSeconds());
		std::optional<GraphOperation> current_operation = timeline.getCurrentOperation();
		std::optional<GraphOperationType> type = std::nullopt;
		if (current_operation.has_value()) type = current_operation->type;
		int highlighted_line = timeline.getHighlightedLine();
		//std::cout << highlighted_line << "\n";
		code_panel.sync(type, highlighted_line);
		code_panel.update(window, fixed_view);
	}
}

void GraphUI::Init(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const PanelData& data) {
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
		graph.rawInit({}, {});
	}
	else if (data.operation == PanelOperation::Random) {
		int n = rand(7, 11);
		int m = rand(11, 15);
		std::vector<std::tuple<int, int, int>> edges;
		bool tree_guarantee = n & 1;
		if (tree_guarantee) {
			for (int i = 1; i < n; i++) {
				int u = rand(0, i - 1);
				int v = i;
				int weight = rand(1, 100);
				edges.push_back({ u,v,weight });
			}
			m = rand(5, 8);
		}
		for (int i = 0; i < m; i++) {
			int u = rand(0, n);
			int v = rand(0, n);
			while (u == v) v = rand(0, n);
			if (u > v) std::swap(u, v);
			int weight = rand(1, 100);
			edges.push_back({ u, v, weight });
		}
		graph.rawInit(n, edges);
	}
	else if (data.operation == PanelOperation::Manual) {

	}
	else if (data.operation == PanelOperation::File) {

	}
	current_state = graph.getState();
	timeline.clear();
	timeline.setInitialState(current_state);
	GraphRecorder dummy;
	timeline.generateAnimation(current_state, current_state, dummy);
}

std::optional<MenuState> GraphUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev) {
	if (ui_state == UIState::Init) {
		std::optional<PanelData> panel_data = init_panel.handleEvent(window, view, ev);
		if (!panel_data.has_value()) return std::nullopt;
		Init(window, view, cam_view, cam, *panel_data);
		return std::nullopt;
	}
	if (ui_state == UIState::Running) {
		code_panel.handleEvent(window, view, ev);
		if (const auto op = panel.handleEvent(window, view, ev); op.has_value()) {
			if (op->type == GraphOperationType::Home) {
				return MenuState::DSMenu;
			}
			else if (op->type == GraphOperationType::Setting) {
				return MenuState::Setting;
			}
			else if (op->type == GraphOperationType::Load) {
				if (op->file_path.empty()) return std::nullopt;
				GraphRecorder dummy;
				graph.applyOperation(*op, dummy);
				current_state = graph.getState();
				timeline.clear();
				timeline.setInitialState(current_state);
				timeline.generateAnimation(current_state, current_state, dummy);
			}
			else {
				graph.applyOperation(*op, recorder);
				GraphState prev = current_state;
				current_state = graph.getState();
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

void GraphUI::draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	if (ui_state == UIState::Init) {
		window.setView(fixed_view);
		window.draw(init_panel);
	}
	if (ui_state == UIState::Running) {
		window.setView(cam_view);
		//window.draw(test);
		timeline.draw(window, cam_view);
		window.setView(fixed_view);
		window.draw(timeline_panel);
		window.draw(panel);
		window.draw(code_panel);
	}
}
