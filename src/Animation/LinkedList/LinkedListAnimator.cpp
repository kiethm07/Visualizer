#include <Animation/LinkedList/LinkedListAnimator.h>

void LinkedListAnimator::generateBaseStates(const LinkedListState& state, const LinkedListRecorder& record) {
	generateAnimationState(initial_state, state);
	phases = record.getPhases();
	total_duration = 0.f;
	base_states.clear();
	start_time.clear();
	base_states.push_back(initial_state);
	start_time.push_back(0.f);
	//std::cout << initial_state.getNodeList().size() << " nodes, " << initial_state.getEdgeList().size() << " edges in initial state\n";
	//std::cout << phases.size() << " phases generated\n";
	for (int i = 0; i < phases.size(); i++) {
		const LinkedListAnimationState& base_state = base_states.back();
		LinkedListAnimationState new_state = base_states.back();
		std::vector<LinkedListAnimationCommand> commands = phases[i].commands;
		//std::cout << commands.size() << " commands in phase " << i << "\n";
		float phase_duration = 0.f;
		for (int j = 0; j < commands.size(); j++) {
			//std::cout << "Applying command " << j << " of phase " << i << "\n";
			//std::cout << "Command type: " << static_cast<int>(commands[j].type) << ", target: " << static_cast<int>(commands[j].target) << "\n";
			applyCommand(commands[j], base_state, new_state, 1.0f);
			phase_duration = std::max(phase_duration, commands[j].duration);
		}
		//std::cout << phase_duration << " seconds for phase " << i << "\n";
		start_time.push_back(start_time.back() + phase_duration);
		base_states.push_back(new_state);
		total_duration += phase_duration;
	}
	//std::cout << "Generated base states for " << base_states.size() << " phases\n";
	//std::cout << "Total duration: " << total_duration << " seconds\n";
}

void LinkedListAnimator::generateAnimationState(LinkedListAnimationState& animation_state, const LinkedListState& state) {
	std::vector<LinkedListAnimationNode> node_list;
	std::vector<LinkedListAnimationEdge> edge_list;

	float x = X_MARGIN;
	float y = Y_MARGIN;

	int n = state.value.size();
	for (int i = 0; i < n; i++) {
		LinkedListAnimationNode node;
		node.value = state.value[i];
		node.ui_id = state.ui_id[i];
		node.position = { x, y };
		node.alpha = 255;
		node.fill_color = DEFAULT_NODE_COLOR;
		x += NODE_GAP;
		node_list.push_back(node);
	}

	for (int i = 1; i < n; i++) {
		LinkedListAnimationEdge edge;
		edge.from_ui_id = state.ui_id[i - 1];
		edge.to_ui_id = state.ui_id[i];
		edge.from_position = node_list[i - 1].position + sf::Vector2f{ (float)NODE_RADIUS, 0.f };
		edge.to_position = node_list[i].position - sf::Vector2f{ (float)NODE_RADIUS, 0.f };
		edge_list.push_back(edge);
	}
	animation_state.setEdgeList(edge_list);
	animation_state.setNodeList(node_list);
}

LinkedListAnimationState LinkedListAnimator::getStateAtTime(float t) const {
	//return LinkedListAnimationState(); //Debug
	if (base_states.empty()) {
		//std::cout << "No base states generated" << std::endl;
		return LinkedListAnimationState();
	}
	if (t >= total_duration) {
		return base_states.back();
	}
	int phase_index = 0;
	//Find the phase index for time t
	for (int i = 0; i < start_time.size(); i++) {
		if (start_time[i] < t) {
			phase_index = i;
		}
		else break;
	}
	//std::cout << start_time[1] << "\n";
	if (phase_index == start_time.size()) {
		return base_states.back();
	}
	const LinkedListAnimationState& base_state = base_states[phase_index];
	LinkedListAnimationState new_state = base_states[phase_index];
	std::vector<LinkedListAnimationCommand> commands = phases[phase_index].commands;
	float phase_time = t - start_time[phase_index];
	float progress = 1.f;
	if (phase_index + 1 == start_time.size()) progress = phase_time / (total_duration - start_time[phase_index]);
	else progress = phase_time / (start_time[phase_index + 1] - start_time[phase_index]);
	for (int i = 0; i < commands.size(); i++) {
		applyCommand(commands[i], base_state, new_state, progress);
	}
	//std::cout << "commands in phase " << phase_index << ": " << commands.size() << "\n";
	//std::cout << "Calculated animation state for time " << t << " in phase " << phase_index << " with progress " << progress << "\n";
	//std::cout << new_state.getNodeList().size() << " nodes, " << new_state.getEdgeList().size() << " edges in new state\n";
	return new_state;
}

static float lerpFloat(float a, float b, float t) {
	return a + (b - a) * t;
}

static sf::Vector2f lerpVector2f(const sf::Vector2f& a, const sf::Vector2f& b, float t) {
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t
	};
}

static std::uint8_t lerpByte(std::uint8_t a, std::uint8_t b, float t) {
	return static_cast<std::uint8_t>(a + (b - a) * t);
}

static sf::Color lerpColor(const sf::Color& a, const sf::Color& b, float t) {
	return sf::Color(
		lerpByte(a.r, b.r, t),
		lerpByte(a.g, b.g, t),
		lerpByte(a.b, b.b, t),
		lerpByte(a.a, b.a, t)
	);
}

void LinkedListAnimator::applyCommandOnNode(LinkedListAnimationNode& node, const LinkedListAnimationCommand& command, const float& progress) const {
	using Type = LinkedListAnimationType;
	Type type = command.type;
	if (type == Type::FadeIn) {
		node.alpha = lerpByte(0, 255, progress);
	}
	else if (type == Type::FadeOut) {
		node.alpha = lerpByte(255, 0, progress);
	}
	else if (type == Type::HighlightOn) {
		node.fill_color = lerpColor(DEFAULT_NODE_COLOR, HIGHLIGHT_NODE_COLOR, progress);
	}
	else if (type == Type::HighlightOff) {
		node.fill_color = lerpColor(HIGHLIGHT_NODE_COLOR, DEFAULT_NODE_COLOR, progress);
	}
	else if (type == Type::Move) {
		sf::Vector2f move_vector = { 0.f, 0.f };
		if (command.direction == LinkedListMoveDirection::Right) {
			move_vector = { (float)move_distance, 0.f };
		}
		else if (command.direction == LinkedListMoveDirection::Left) {
			move_vector = { (float)-move_distance, 0.f };
		}
		else if (command.direction == LinkedListMoveDirection::Up) {
			move_vector = { 0.f, (float)-move_distance };
		}
		else if (command.direction == LinkedListMoveDirection::Down) {
			move_vector = { 0.f, (float)move_distance };
		}
		node.position = lerpVector2f(node.position, node.position + move_vector, progress);
	}
}

void LinkedListAnimator::applyCommandOnEdge(LinkedListAnimationEdge& edge, const LinkedListAnimationCommand& command, const float& progress) const {
	using Type = LinkedListAnimationType;
	Type type = command.type;
	if (type == Type::FadeIn) {
		edge.alpha = lerpByte(0, 255, progress);
	}
	else if (type == Type::FadeOut) {
		edge.alpha = lerpByte(255, 0, progress);
	}
	else if (type == Type::Move) {
		sf::Vector2f move_vector = { 0.f, 0.f };
		if (command.direction == LinkedListMoveDirection::Right) {
			move_vector = { (float)move_distance, 0.f };
		}
		else if (command.direction == LinkedListMoveDirection::Left) {
			move_vector = { (float)-move_distance, 0.f };
		}
		else if (command.direction == LinkedListMoveDirection::Up) {
			move_vector = { 0.f, (float)-move_distance };
		}
		else if (command.direction == LinkedListMoveDirection::Down) {
			move_vector = { 0.f, (float)move_distance };
		}
		edge.from_position = lerpVector2f(edge.from_position, edge.from_position + move_vector, progress);
		edge.to_position = lerpVector2f(edge.to_position, edge.to_position + move_vector, progress);
	}
}

void LinkedListAnimator::applyCommand(const LinkedListAnimationCommand& command, const LinkedListAnimationState& base_state, LinkedListAnimationState& state, const float& progress) const {
	if (command.target == LinkedListAnimationTarget::Node) {
		const std::vector<LinkedListAnimationNode>& nodes = base_state.getNodeList();
		//std::cout << nodes.size() << " nodes in base state\n";
		int node_index = 0;
		for (node_index; node_index < nodes.size(); node_index++) {
			if (nodes[node_index].ui_id == command.ui_id) {
				break;
			}
		}
		if (node_index == nodes.size()) {
			std::cout << "Node with ui_id " << command.ui_id << " not found" << std::endl;
			return;
		}
		LinkedListAnimationNode new_node = nodes[node_index];
		applyCommandOnNode(new_node, command, progress);
		state.modifyNode(new_node, node_index);
	}
	else {
		const std::vector<LinkedListAnimationEdge>& edges = base_state.getEdgeList();
		int edge_index = 0;
		for (edge_index; edge_index < edges.size(); edge_index++) {
			if (edges[edge_index].from_ui_id == command.from_ui_id && edges[edge_index].to_ui_id == command.to_ui_id) {
				break;
			}
		}
		if (edge_index == edges.size()) {
			std::cout << "Edge with from_ui_id " << command.from_ui_id << " and to_ui_id " << command.to_ui_id << " not found" << std::endl;
			return;
		}
		LinkedListAnimationEdge new_edge = edges[edge_index];
		applyCommandOnEdge(new_edge, command, progress);
		state.modifyEdge(new_edge, edge_index);
	}
}