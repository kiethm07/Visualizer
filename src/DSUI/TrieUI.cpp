#include <DSUI/TrieUI.h>
#include <iostream>
#include <random>
static std::mt19937 rng(6969);
static int rand(int l, int r) {
	if (l > r) std::swap(l, r);
	return std::uniform_int_distribution<int>(l, r)(rng);
}
TrieUI::TrieUI(const AssetManager& a_manager) :
	a_manager(a_manager),
	NODE_FONT(a_manager.getFont("Roboto-Regular")),
	panel(NODE_FONT),
	timeline(a_manager),
	timeline_panel(a_manager),
	ui_state(UIState::Init),
	init_panel(a_manager)
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
	}
}
void TrieUI::Init(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const PanelData& data) {
	ui_state = UIState::Running;
	cam.reset(window, cam_view);
	if (data.operation == PanelOperation::Empty) {
		trie.rawInit({});
	}
	else if (data.operation == PanelOperation::Random) {
		std::vector<std::string> v;
		int num = rand(3, 6);
		for (int i = 0; i < num; i++) {
			std::string s = "";
			int len = rand(2, 5);
			for (int j = 0; j < len; j++) s += (char)('A' + rand(0, 25));
			v.push_back(s);
		}
		trie.rawInit(v);
	}
	else if (data.operation == PanelOperation::Manual || data.operation == PanelOperation::File) {
		std::vector<std::string> v;
		std::string current = "";
		for (int x : data.values) {
			if (x == (int)',') {
				if (!current.empty()) v.push_back(current);
				current = "";
			}
			else current += (char)x;
		}
		if (!current.empty()) v.push_back(current);
		trie.rawInit(v);
	}
	current_state = trie.getState();
	timeline.setInitialState(current_state);
	timeline.generateAnimation(current_state, current_state, TrieRecorder());
}
void TrieUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev) {
	if (ui_state == UIState::Init) {
		std::optional<PanelData> panel_data = init_panel.handleEvent(window, view, ev);
		if (!panel_data.has_value()) return;
		Init(window, view, cam_view, cam, *panel_data);
		return;
	}
	if (ui_state == UIState::Running) {
		if (const auto op = panel.handleEvent(window, view, ev); op.has_value()) {
			recorder.clear();
			trie.applyOperation(*op, recorder);
			TrieState prev = current_state;
			current_state = trie.getState();
			timeline.push(prev, current_state, *op, recorder);
		}
		if (const auto op = timeline_panel.handleEvent(window, view, cam_view, cam, ev)) {
			if (op->type == TimelineOperation::Play) {
				if (timeline.isRunning()) timeline.pause();
				else timeline.run();
			}
			else if (op->type == TimelineOperation::AutoPlay) {
				timeline.setAutoPlay(timeline.isAutoPlaying() ^ 1);
			}
			else if (op->type == TimelineOperation::OnePhaseForward) timeline.onePhaseForward();
			else if (op->type == TimelineOperation::OnePhaseBackward) timeline.onePhaseBackward();
			else if (op->type == TimelineOperation::OneStepForward) timeline.oneStepForward();
			else if (op->type == TimelineOperation::OneStepBackward) timeline.oneStepBackward();
			else if (op->type == TimelineOperation::LastState) timeline.toLast();
			else if (op->type == TimelineOperation::InitState) timeline.toInit();
			else if (op->type == TimelineOperation::ChangeSpeed) {

			}
		}
	}
}
void TrieUI::draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	if (ui_state == UIState::Init) {
		window.setView(fixed_view);
		window.draw(init_panel);
	}
	if (ui_state == UIState::Running) {
		window.setView(cam_view);
		timeline.draw(window, cam_view);
		window.setView(fixed_view);
		window.draw(panel);
		window.draw(timeline_panel);
	}
}