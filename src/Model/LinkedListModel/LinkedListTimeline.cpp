#include <Model/LinkedListModel/LinkedListTimeline.h>

LinkedListTimeline::LinkedListTimeline(const AssetManager& a_manager) :
	a_manager(a_manager),
	renderer(a_manager),
	list_operations(),
	list_states(),
	records() {
}

void LinkedListTimeline::update(const sf::RenderWindow& window, const sf::View& view, const float& real_delta_time) {
	if (!running) return;
	float dt = real_delta_time * speed;
	float new_time = current_time + dt * direction;
	if (!auto_play) {
		if (animator.isPhaseBoundary(current_time)) {
			if (!next_phase_wating) new_time = current_time;
			next_phase_wating = 0;
		}
		else {
			float left_bound = animator.getPreviousPhaseTimer(current_time);
			float right_bound = animator.getNextPhaseTimer(current_time);
			if (left_bound == -1) left_bound = 0;
			if (right_bound == -1) right_bound = animator.getTotalDuration();
			if (new_time > right_bound) new_time = right_bound;
			if (new_time < left_bound) new_time = left_bound;
		}
	}
	//speed = 0.1;
	current_time = new_time;
	//Debug purpose
	//current_time = 0.3f; //Debug
	//if (list_operations.size()) {
	//	animator.generateBaseStates(list_states[current_operation_index], records[current_operation_index]);
	//	//std::cout << "Generated base states for operation " << current_operation_index << "\n";
	//	//std::cout << animator.getTotalDuration() << " total duration\n";
	//}
	if (current_time > animator.getTotalDuration() && current_operation_index == list_operations.size()) {
		current_time = animator.getTotalDuration();
	}
	else if (current_time > animator.getTotalDuration()) {
		current_time -= animator.getTotalDuration();
		while (current_operation_index < list_operations.size()) {
			animator.generateBaseStates(list_states[current_operation_index], records[current_operation_index]);
			current_operation_index++;
			if (current_time < animator.getTotalDuration() || current_operation_index == list_operations.size()) {
				break;
			}
			current_time -= animator.getTotalDuration();
		}
		if (current_operation_index == list_operations.size()) {
			current_time = std::min(current_time, animator.getTotalDuration());
		}
	}
	else if (current_time < 0 && current_operation_index == 0) {
		current_time = 0;
		animator.clear();
	}
	else if (current_time < 0) {
		while (current_operation_index > 0) {
			current_operation_index--;
			if (current_operation_index == 0) break;
			animator.generateBaseStates(list_states[current_operation_index - 1], records[current_operation_index - 1]);
			current_time += animator.getTotalDuration();
			if (current_time > 0 || current_operation_index == 0) {
				break;
			}
		}
		if (current_operation_index == 0) {
			current_time = 0;
			animator.clear();
		}
	}
	current_animation_state = animator.getStateAtTime(current_time);
	//std::cout << list_operations.size() << " operations, current index: " << current_operation_index << ", current time: " << current_time << "\n";
	//std::cout << animator.getTotalDuration() << " total duration\n";
	//std::cout << current_operation_index << " current operation index\n";
	//std::cout << current_operation_index << " " << current_time << "\n";
}

void LinkedListTimeline::push(const LinkedListState& current_state, const ListOperation& operation, const LinkedListRecorder& record) {
	list_states.push_back(current_state);
	list_operations.push_back(operation);
	records.push_back(record);
}

void LinkedListTimeline::run() {
	running = true;
}

void LinkedListTimeline::pause() {
	running = false;
}

void LinkedListTimeline::clear() {
	running = false;
	list_operations.clear();
	list_states.clear();
	records.clear();
}

void LinkedListTimeline::generateAnimation(const LinkedListState& initial_state, const LinkedListRecorder& record) {
	animator.generateBaseStates(initial_state, record);
}

void LinkedListTimeline::onePhaseForward() {
	if (!running) return;
	direction = 1;
	float old_time = current_time;
	if (animator.isPhaseBoundary(current_time)) {
		next_phase_wating = 1;
		return;
	}
	float next_phase_timer = animator.getNextPhaseTimer(current_time);
	if (next_phase_timer == -1) {
		//end of current operation
		if (current_operation_index == list_operations.size()) {
			next_phase_timer = animator.getTotalDuration();
		}
		else {
			generateAnimation(list_states[current_operation_index], records[current_operation_index]);
			current_operation_index++;
			next_phase_timer = 0;
		}
		current_time = next_phase_timer;
	}
	else {
		current_time = next_phase_timer;
	}
	//std::cout << old_time << " " << next_phase_timer << "\n";
}

void LinkedListTimeline::onePhaseBackward() {
	if (!running) return;
	direction = -1;
	if (animator.isPhaseBoundary(current_time)) {
		next_phase_wating = 1;
		return;
	}
	float prev_phase_timer = animator.getPreviousPhaseTimer(current_time);
	if (prev_phase_timer == -1) {
		//start of current operation
		if (current_operation_index == 0) {
			prev_phase_timer = 0;
			animator.clear();
		}
		else {
			current_operation_index--;
			generateAnimation(list_states[current_operation_index], records[current_operation_index]);
			prev_phase_timer = animator.getTotalDuration();
		}
		current_time = prev_phase_timer;
	}
	else {
		current_time = prev_phase_timer;
	}
}

void LinkedListTimeline::oneStepForward() {
	if (!running) return;
	direction = 1;
	next_phase_wating = 0;
	//std::cout << current_operation_index << " " << current_time << " " << animator.getTotalDuration() << "\n";
	if (current_operation_index == list_states.size()) {
		current_time = animator.getTotalDuration();
		return;
	}
	if (current_time < animator.getTotalDuration()) {
		current_time = animator.getTotalDuration();
	}
	else {
		generateAnimation(list_states[current_operation_index], records[current_operation_index]);
		current_operation_index++;
		current_time = animator.getTotalDuration();
	}
	//std::cout << current_operation_index << " " << current_time << "\n";	
}

void LinkedListTimeline::oneStepBackward() {
	if (!running) return;
	direction = -1;
	next_phase_wating = 0;
	//std::cout << current_operation_index << " " << current_time << " " << animator.getTotalDuration() << "\n";
	if (current_operation_index == 0) {
		current_time = 0.f;
		animator.clear();
		return;
	}
	if (current_time > 0.f) {
		current_time = 0.f;
	}
	else {
		current_operation_index--;
		if (current_operation_index > 0)
			generateAnimation(list_states[current_operation_index - 1], records[current_operation_index - 1]);
		else
			animator.clear();
		current_time = 0.f;
	}
	//std::cout << current_operation_index << " " << current_time << " " << animator.getTotalDuration() << "\n";
}

void LinkedListTimeline::toLast() {
	if (!running) return;
	direction = 1;
	if (list_states.empty()) return;
	generateAnimation(list_states.back(), records.back());
	current_operation_index = list_states.size();
	current_time = animator.getTotalDuration();
}

void LinkedListTimeline::toInit() {
	if (!running) return;
	direction = -1;
	animator.clear();
	current_operation_index = 0;
	current_time = 0;
}

void LinkedListTimeline::draw(sf::RenderWindow& window, const sf::View& view) {
	renderer.loadState(current_animation_state);
	renderer.draw(window, view);
}