#include <Animation/Hashmap/HashmapAnimator.h>

void HashmapAnimator::generateBaseStates(const HashmapState& state, const HashmapState& fin_state, const HashmapRecorder& record) {
	generateAnimationState(initial_state, state);
	phases = record.getPhases();
	highlighted_lines = record.getHighlightedLines();
	clear();
	start_time.push_back(0.f);
	HashmapAnimationState base_state_after_spawn = initial_state;

	for (int i = 0; i < phases.size(); i++) {
		std::vector<HashmapAnimationCommand> commands = phases[i].commands;
		for (int j = 0; j < commands.size(); j++) {
			if (commands[j].type == HashmapAnimationType::Spawn && commands[j].target == HashmapAnimationTarget::Node) {
				applySpawnCommand(commands[j], base_state_after_spawn, initial_state);
			}
		}
	}
	initial_state = base_state_after_spawn;
	for (int i = 0; i < phases.size(); i++) {
		std::vector<HashmapAnimationCommand> commands = phases[i].commands;
		for (int j = 0; j < commands.size(); j++) {
			if (commands[j].type == HashmapAnimationType::Spawn && commands[j].target == HashmapAnimationTarget::Edge) {
				applySpawnCommand(commands[j], base_state_after_spawn, initial_state);
			}
		}
	}
	normalizeEdgeLists(base_state_after_spawn);
	base_states.push_back(base_state_after_spawn);

	for (int i = 0; i < phases.size(); i++) {
		const HashmapAnimationState& base_state = base_states.back();
		HashmapAnimationState new_state = base_state;
		std::vector<HashmapAnimationCommand> commands = phases[i].commands;

		float phase_duration = 0.f;
		for (int j = 0; j < commands.size(); j++) {
			if (commands[j].type == HashmapAnimationType::Spawn) continue;
			applyCommand(commands[j], base_state, new_state, 1.0f);
			phase_duration = std::max(phase_duration, commands[j].duration);
		}

		start_time.push_back(start_time.back() + phase_duration);
		normalizeEdgeLists(new_state);
		base_states.push_back(new_state);
		total_duration += phase_duration;
	}
}

void HashmapAnimator::generateAnimationState(HashmapAnimationState& animation_state, const HashmapState& state) {
	std::vector<HashmapAnimationNode> node_list;
	std::vector<HashmapAnimationEdge> edge_list;
	std::vector<HashmapAnimationBucket> bucket_list;

	float start_x = (float)X_MARGIN;
	float start_y = (float)Y_MARGIN;

	for (int i = 0; i < state.bucket_count; i++) {
		HashmapAnimationBucket bucket;
		bucket.position = { start_x, start_y + i * BUCKET_GAP };
		bucket.fill_color = DEFAULT_BUCKET_COLOR;
		bucket_list.push_back(bucket);

		float node_x = bucket.position.x + BUCKET_WIDTH + NODE_GAP;
		for (int j = 0; j < state.value[i].size(); j++) {
			HashmapAnimationNode node;
			node.value = state.value[i][j];
			node.ui_id = state.ui_id[i][j];
			node.position = { node_x, bucket.position.y + (BUCKET_HEIGHT / 2.f) };
			node.alpha = 255;
			node.fill_color = DEFAULT_NODE_COLOR;
			node_list.push_back(node);

			HashmapAnimationEdge edge;
			edge.from_ui_id = (j == 0) ? (-(i + 1)) : state.ui_id[i][j - 1];
			edge.to_ui_id = state.ui_id[i][j];
			edge.fill_color = DEFAULT_EDGE_COLOR;
			edge.alpha = 255;
			edge_list.push_back(edge);

			node_x += NODE_GAP;
		}
	}
	animation_state.setBucketList(bucket_list);
	animation_state.setNodeList(node_list);
	animation_state.setEdgeList(edge_list);
	normalizeEdgeLists(animation_state);
}

bool HashmapAnimator::isPhaseBoundary(float t) const {
	int i = 0;
	for (i = 0; i < start_time.size(); i++) {
		if (start_time[i] < t) continue;
		if (start_time[i] > t) break;
	}
	const float EPSILON = 1e-5;
	for (int j = i - 1; j <= i + 1; j++) {
		if (j < 0) continue;
		if (j >= start_time.size()) break;
		if (abs(start_time[j] - t) <= EPSILON) return 1;
	}
	return 0;
}

float HashmapAnimator::getNextPhaseTimer(float t) const {
	for (int i = 0; i < start_time.size(); i++) {
		if (start_time[i] > t) return start_time[i];
	}
	return -1;
}

float HashmapAnimator::getPreviousPhaseTimer(float t) const {
	for (int i = 0; i < start_time.size(); i++) {
		if (start_time[i] >= t && i > 0) return start_time[i - 1];
	}
	return -1;
}

float HashmapAnimator::getLeftBound(float t) const {
	for (int i = 0; i < start_time.size(); i++) {
		if (start_time[i] > t && i > 0) return start_time[i - 1];
	}
	return -1;
}

float HashmapAnimator::getRightBound(float t) const {
	for (int i = 0; i < start_time.size(); i++) {
		if (start_time[i] >= t) return start_time[i];
	}
	return -1;
}

int HashmapAnimator::getHighlightedLine(float t) const {
	if (highlighted_lines.empty()) {
		return -1;
	}
	if (t >= total_duration) return -1;

	int index = 0;
	for (int i = 0; i < start_time.size(); i++) {
		if (start_time[i] < t) {
			index = i;
		}
		else break;
	}

	if (index == start_time.size()) return -1;

	return highlighted_lines[index];
}

HashmapAnimationState HashmapAnimator::getStateAtTime(float t) {
	if (base_states.empty()) {
		return HashmapAnimationState();
	}
	if (t >= total_duration) {
		return base_states.back();
	}

	int phase_index = 0;
	for (int i = 0; i < start_time.size(); i++) {
		if (start_time[i] < t) {
			phase_index = i;
		}
		else {
			break;
		}
	}

	if (phase_index == start_time.size()) {
		return base_states.back();
	}

	const HashmapAnimationState& base_state = base_states[phase_index];
	HashmapAnimationState new_state = base_state;
	std::vector<HashmapAnimationCommand> commands = phases[phase_index].commands;
	float phase_time = t - start_time[phase_index];
	float progress = 1.f;

	if (phase_index + 1 == start_time.size()) {
		progress = phase_time / (total_duration - start_time[phase_index]);
	}
	else {
		progress = phase_time / (start_time[phase_index + 1] - start_time[phase_index]);
	}

	for (int i = 0; i < commands.size(); i++) {
		if (commands[i].type == HashmapAnimationType::Spawn) {
			continue;
		}
		applyCommand(commands[i], base_state, new_state, progress);
	}
	normalizeEdgeLists(new_state);
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

void HashmapAnimator::applyCommandOnNode(HashmapAnimationNode& node, const HashmapAnimationCommand& command, const float& progress) const {
	using Type = HashmapAnimationType;
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
	else if (type == Type::FoundedOn) {
		node.fill_color = lerpColor(HIGHLIGHT_NODE_COLOR, FOUNDED_NODE_COLOR, progress);
	}
	else if (type == Type::FoundedOff) {
		node.fill_color = lerpColor(FOUNDED_NODE_COLOR, DEFAULT_NODE_COLOR, progress);
	}
	else if (type == Type::Move) {
		sf::Vector2f move_vector = { 0.f, 0.f };
		if (command.direction == HashmapMoveDirection::Right) {
			move_vector = { (float)move_distance, 0.f };
		}
		else if (command.direction == HashmapMoveDirection::Left) {
			move_vector = { (float)-move_distance, 0.f };
		}
		else if (command.direction == HashmapMoveDirection::Up) {
			move_vector = { 0.f, (float)-move_distance };
		}
		else if (command.direction == HashmapMoveDirection::Down) {
			move_vector = { 0.f, (float)move_distance };
		}
		node.position = lerpVector2f(node.position, node.position + move_vector, progress);
	}
}

void HashmapAnimator::applyCommandOnBucket(HashmapAnimationBucket& bucket, const HashmapAnimationCommand& command, const float& progress) const {
	using Type = HashmapAnimationType;
	if (command.type == Type::HighlightOn) {
		bucket.fill_color = lerpColor(DEFAULT_BUCKET_COLOR, HIGHLIGHT_BUCKET_COLOR, progress);
	}
	else if (command.type == Type::HighlightOff) {
		bucket.fill_color = lerpColor(HIGHLIGHT_BUCKET_COLOR, DEFAULT_BUCKET_COLOR, progress);
	}
}

void HashmapAnimator::applyCommandOnEdge(HashmapAnimationEdge& edge, const HashmapAnimationCommand& command, const float& progress) const {
	using Type = HashmapAnimationType;
	if (command.type == Type::FadeIn) {
		edge.alpha = lerpByte(0, 255, progress);
	}
	else if (command.type == Type::FadeOut) {
		edge.alpha = lerpByte(255, 0, progress);
	}
}

void HashmapAnimator::applySpawnCommand(const HashmapAnimationCommand& command, HashmapAnimationState& state, const HashmapAnimationState& base_state) const {
	if (command.target == HashmapAnimationTarget::Node) {
		HashmapAnimationNode new_node;
		new_node.value = command.value;
		new_node.position = { (float)X_MARGIN, (float)Y_MARGIN };

		bool found_source = false;
		for (const HashmapAnimationNode& node : base_state.getNodeList()) {
			if (node.ui_id == command.spawn_from_ui_id) {
				new_node.position = node.position + command.spawn_offset;
				found_source = true;
				break;
			}
		}

		if (!found_source && command.spawn_from_ui_id < 0) {
			int b_idx = -command.spawn_from_ui_id - 1;
			const std::vector<HashmapAnimationBucket>& buckets = base_state.getBucketList();
			if (b_idx >= 0 && b_idx < (int)buckets.size()) {
				new_node.position = buckets[b_idx].position + sf::Vector2f{ (float)BUCKET_WIDTH + NODE_GAP, BUCKET_HEIGHT / 2.f };
			}
		}

		new_node.ui_id = command.ui_id;
		new_node.alpha = 0;
		new_node.fill_color = DEFAULT_NODE_COLOR;
		state.insertNode(new_node);
	}
	else if (command.target == HashmapAnimationTarget::Edge) {
		HashmapAnimationEdge new_edge;
		new_edge.from_ui_id = command.from_ui_id;
		new_edge.to_ui_id = command.to_ui_id;
		new_edge.alpha = 0;
		new_edge.fill_color = DEFAULT_EDGE_COLOR;

		const std::vector<HashmapAnimationNode>& nodes = base_state.getNodeList();
		const std::vector<HashmapAnimationBucket>& buckets = base_state.getBucketList();

		bool found_from = false;
		for (const auto& node : nodes) {
			if (node.ui_id == command.from_ui_id) {
				new_edge.from_position = node.position + sf::Vector2f{ (float)NODE_RADIUS, 0.f };
				found_from = true;
				break;
			}
		}
		if (!found_from && command.from_ui_id < 0) {
			int b_idx = -command.from_ui_id - 1;
			if (b_idx >= 0 && b_idx < (int)buckets.size()) {
				new_edge.from_position = buckets[b_idx].position + sf::Vector2f{ (float)BUCKET_WIDTH, BUCKET_HEIGHT / 2.f };
			}
		}

		for (const auto& node : nodes) {
			if (node.ui_id == command.to_ui_id) {
				new_edge.to_position = node.position - sf::Vector2f{ (float)NODE_RADIUS, 0.f };
				break;
			}
		}

		state.insertEdge(new_edge);
	}
}

void HashmapAnimator::applyCommand(const HashmapAnimationCommand& command, const HashmapAnimationState& base_state, HashmapAnimationState& state, const float& progress) const {
	if (command.target == HashmapAnimationTarget::Node) {
		const std::vector<HashmapAnimationNode>& nodes = base_state.getNodeList();
		for (int i = 0; i < (int)nodes.size(); i++) {
			if (nodes[i].ui_id == command.ui_id) {
				HashmapAnimationNode new_node = nodes[i];
				applyCommandOnNode(new_node, command, progress);
				state.modifyNode(new_node, i);
				return;
			}
		}
	}
	else if (command.target == HashmapAnimationTarget::Bucket) {
		const std::vector<HashmapAnimationBucket>& buckets = base_state.getBucketList();
		int b_idx = -command.ui_id - 1;
		if (b_idx >= 0 && b_idx < (int)buckets.size()) {
			HashmapAnimationBucket new_bucket = buckets[b_idx];
			applyCommandOnBucket(new_bucket, command, progress);
			state.modifyBucket(new_bucket, b_idx);
		}
	}
	else if (command.target == HashmapAnimationTarget::Edge) {
		const std::vector<HashmapAnimationEdge>& edges = base_state.getEdgeList();
		for (int i = 0; i < (int)edges.size(); i++) {
			if (edges[i].from_ui_id == command.from_ui_id && edges[i].to_ui_id == command.to_ui_id) {
				HashmapAnimationEdge new_edge = edges[i];
				applyCommandOnEdge(new_edge, command, progress);
				state.modifyEdge(new_edge, i);
				return;
			}
		}
	}
}

void HashmapAnimator::normalizeEdgeLists(HashmapAnimationState& animation_state) {
	std::vector<HashmapAnimationEdge> edge_list = animation_state.getEdgeList();
	std::vector<HashmapAnimationNode> node_list = animation_state.getNodeList();
	std::vector<HashmapAnimationBucket> bucket_list = animation_state.getBucketList();

	std::unordered_map<int, sf::Vector2f> ui_id_to_position;
	for (const auto& node : node_list) {
		ui_id_to_position[node.ui_id] = node.position;
	}

	for (HashmapAnimationEdge& edge : edge_list) {
		if (ui_id_to_position.count(edge.from_ui_id)) {
			edge.from_position = ui_id_to_position[edge.from_ui_id] + sf::Vector2f{ (float)NODE_RADIUS, 0.f };
		}
		else if (edge.from_ui_id < 0) {
			int b_idx = -edge.from_ui_id - 1;
			if (b_idx >= 0 && b_idx < (int)bucket_list.size()) {
				edge.from_position = bucket_list[b_idx].position + sf::Vector2f{ (float)BUCKET_WIDTH, BUCKET_HEIGHT / 2.f };
			}
		}

		if (ui_id_to_position.count(edge.to_ui_id)) {
			edge.to_position = ui_id_to_position[edge.to_ui_id] - sf::Vector2f{ (float)NODE_RADIUS, 0.f };
		}
	}
	animation_state.setEdgeList(edge_list);
}

void HashmapAnimator::clear() {
	base_states.clear();
	start_time.clear();
	total_duration = 0;
}