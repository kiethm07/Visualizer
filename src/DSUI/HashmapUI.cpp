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
		//timeline.update(clock.restart().asSeconds());
	}
}

void HashmapUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev) {
	if (ui_state == UIState::Init) {
		std::optional<PanelData> panel_data = init_panel.handleEvent(window, view, ev);
		if (!panel_data.has_value()) return;
		ui_state = UIState::Running;
		cam.reset(window, cam_view);
		//for (int i : panel_data->values) std::cout << i << " ";
		//std::cout << "\n";
		if (panel_data->operation == PanelOperation::Empty) {
			hashmap.rawInit(bucket_count, panel_data->values);
		}
		else if (panel_data->operation == PanelOperation::Random) {
			std::vector<int> v;
			int num = rand(5, 10);
			for (int i = 0; i < num; i++) {
				v.push_back(rand(-20, 50));
			}
			hashmap.rawInit(bucket_count, v);
		}
		else if (panel_data->operation == PanelOperation::Manual) {
			hashmap.rawInit(bucket_count, panel_data->values);
		}
		else if (panel_data->operation == PanelOperation::File) {
			hashmap.rawInit(bucket_count, panel_data->values);
		}
		current_state = hashmap.getState();
		//timeline.generateAnimation(current_state, HashmapRecorder());
		clock.restart();
		return;
	}

	if (ui_state == UIState::Running) {
		if (const auto op = panel.handleEvent(window, view, ev); op.has_value()) {
			recorder.clear();
			hashmap.applyOperation(*op, recorder);
			//timeline.push(current_state, *op, recorder);
			current_state = hashmap.getState();
		}

		//if (const auto op = timeline_panel.handleEvent(window, view, cam_view, cam, ev)) {
		//	if (op->type == TimelineOperation::Play) {
		//		if (timeline.isRunning()) {
		//			timeline.pause();
		//		}
		//		else {
		//			timeline.run();
		//		}
		//	}
		//	else if (op->type == TimelineOperation::AutoPlay) {
		//		bool flag = timeline.isAutoPlaying() ^ 1;
		//		timeline.setAutoPlay(flag);
		//	}
		//	else if (op->type == TimelineOperation::OnePhaseForward) {
		//		timeline.onePhaseForward();
		//	}
		//	else if (op->type == TimelineOperation::OnePhaseBackward) {
		//		timeline.onePhaseBackward();
		//	}
		//	else if (op->type == TimelineOperation::OneStepForward) {
		//		timeline.oneStepForward();
		//	}
		//	else if (op->type == TimelineOperation::OneStepBackward) {
		//		timeline.oneStepBackward();
		//	}
		//	else if (op->type == TimelineOperation::LastState) {
		//		timeline.toLast();
		//	}
		//	else if (op->type == TimelineOperation::InitState) {
		//		timeline.toInit();
		//	}
		//	else if (op->type == TimelineOperation::ChangeSpeed) {

		//	}
		//}
	}
}

void HashmapUI::draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	if (ui_state == UIState::Init) {
		window.setView(fixed_view);
		window.draw(init_panel);
	}

	if (ui_state == UIState::Running) {
		window.setView(cam_view);
		//timeline.draw(window, cam_view);

		window.setView(fixed_view);
		window.draw(panel);
		window.draw(timeline_panel);
	}
}