#include <DSUI/HashmapUI.h>
#include <iostream>
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
	init_panel(a_manager)
{
	init_panel.setPlaceHolderForManualInput("Not inited");
}

void HashmapUI::update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	if (ui_state == UIState::Init) {
		init_panel.update(window, fixed_view);
	}
	else if (ui_state == UIState::Running) {
		panel.update(window, fixed_view);
		timeline_panel.update(window, fixed_view);
		timeline.update(clock.restart().asSeconds());
	}
}

void HashmapUI::Init(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const PanelData& data) {
	ui_state = UIState::Running;
	cam.reset(window, cam_view);
	//for (int i : data.values) std::cout << i << " "; std::cout << "\n";
	if (data.operation == PanelOperation::Empty) {
		//Do nothing
		hashmap.rawInit(DEFAULT_BUCKET_COUNT, {});
	}
	else if (data.operation == PanelOperation::Random) {
		std::vector<int> v;
		int num = rand(5, 7);
		int bucket_count = rand(13, 23);
		for (int i = 0; i < num; i++) {
			v.push_back(rand(-5, 20));
		}
		hashmap.rawInit(bucket_count, v);
	}
	else if (data.operation == PanelOperation::Manual) {
		std::vector<int> values = data.values;
		int bucket_count = values[0];
		values.erase(values.begin());
		hashmap.rawInit(bucket_count, values);
	}
	else if (data.operation == PanelOperation::File) {
		std::vector<int> values = data.values;
		int bucket_count = values[0];
		values.erase(values.begin());
		hashmap.rawInit(bucket_count, values);
	}
	current_state = hashmap.getState();
	timeline.setInitialState(current_state);
	timeline.generateAnimation(current_state, current_state, HashmapRecorder());
}

void HashmapUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev) {
	if (ui_state == UIState::Init) {
		std::optional<PanelData> panel_data = init_panel.handleEvent(window, view, ev);
		if (!panel_data.has_value()) return;
		Init(window, view, cam_view, cam, *panel_data);
		return;
	}

	if (ui_state == UIState::Running) {
		if (const auto op = panel.handleEvent(window, view, ev); op.has_value()) {
			recorder.clear();
			hashmap.applyOperation(*op, recorder);
			HashmapState prev = current_state;
			current_state = hashmap.getState();
			timeline.push(prev, current_state, *op, recorder);
		}

		if (const auto op = timeline_panel.handleEvent(window, view, cam_view, cam, ev)) {
			if (op->type == TimelineOperation::Play) {
				if (timeline.isRunning()) {
					timeline.pause();
				}
				else {
					timeline.run();
				}
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

			}
		}
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
		window.draw(panel);
		window.draw(timeline_panel);
	}
}