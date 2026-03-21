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
	current_time += dt * direction;
	//current_time = 0.3f; //Debug
	//Debug purpose
	//if (list_operations.size()) {
	//	animator.generateBaseStates(list_states[current_operation_index], records[current_operation_index]);
		//std::cout << "Generated base states for operation " << current_operation_index << "\n";
		//std::cout << animator.getTotalDuration() << " total duration\n";
	//}
	if (current_time > animator.getTotalDuration()) {
		while(current_time > animator.getTotalDuration() && current_operation_index < list_operations.size()) {
			animator.generateBaseStates(list_states[current_operation_index], records[current_operation_index]);
			if (current_time > animator.getTotalDuration()) {
				current_time -= animator.getTotalDuration();
			}
			else break;
			current_operation_index++;
		}
		current_time = std::min(current_time, animator.getTotalDuration());
	}
	//else if (current_time < 0){
	//	while(current_time < 0 && current_operation_index > 0) {
	//		animator.generateBaseStates(list_states[current_operation_index - 1], records[current_operation_index - 1]);
	//		current_operation_index--;
	//		current_time += animator.getTotalDuration();
	//	}
	//	current_time = std::max(current_time, 0.f);
	//}
	std::cout << list_operations.size() << " operations, current index: " << current_operation_index << ", current time: " << current_time << "\n";
	std::cout << animator.getTotalDuration() << " total duration\n";
	std::cout << current_operation_index << " current operation index\n";
	current_animation_state = animator.getStateAtTime(current_time);
}

void LinkedListTimeline::push(const LinkedListState& current_state, const ListOperation& operation, const LinkedListRecorder& record) {
	list_states.push_back(current_state);
	list_operations.push_back(operation);
	records.push_back(record);
}

void LinkedListTimeline::run() {
	if (running) return;
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

void LinkedListTimeline::draw(sf::RenderWindow& window, const sf::View& view) {
	renderer.loadState(current_animation_state);
	renderer.draw(window, view);
}