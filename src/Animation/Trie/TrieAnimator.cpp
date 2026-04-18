//#include <Animation/Trie/TrieAnimator.h>
//
//void TrieAnimator::generateBaseStates(const TrieState& state, const TrieState& fin_state, const TrieRecorder& record) {
//    generateAnimationState(initial_state, state);
//    phases = record.getPhases();
//    clear();
//    start_time.push_back(0.f);
//    TrieAnimationState base_state_after_spawn = initial_state;
//
//    for (int i = 0; i < (int)phases.size(); i++) {
//        std::vector<TrieAnimationCommand> commands = phases[i].commands;
//        TrieAnimationState tmp = base_state_after_spawn;
//        for (int j = 0; j < (int)commands.size(); j++) {
//            if (commands[j].type == TrieAnimationType::Spawn && commands[j].target == TrieAnimationTarget::Node) {
//                applySpawnCommand(commands[j], base_state_after_spawn, tmp);
//                tmp = base_state_after_spawn;
//            }
//        }
//    }
//
//    initial_state = base_state_after_spawn;
//    for (int i = 0; i < (int)phases.size(); i++) {
//        std::vector<TrieAnimationCommand> commands = phases[i].commands;
//        TrieAnimationState tmp = base_state_after_spawn;
//        for (int j = 0; j < (int)commands.size(); j++) {
//            if (commands[j].type == TrieAnimationType::Spawn && commands[j].target == TrieAnimationTarget::Edge) {
//                applySpawnCommand(commands[j], base_state_after_spawn, initial_state);
//                tmp = base_state_after_spawn;
//            }
//        }
//    }
//
//    normalizeEdgeLists(base_state_after_spawn);
//    base_states.push_back(base_state_after_spawn);
//
//    for (int i = 0; i < (int)phases.size(); i++) {
//        const TrieAnimationState& base_state = base_states.back();
//        TrieAnimationState new_state = base_state;
//        std::vector<TrieAnimationCommand> commands = phases[i].commands;
//        float phase_duration = 0.f;
//        bool trigger_reconstruct = false;
//
//        for (int j = 0; j < (int)commands.size(); j++) {
//            if (commands[j].type == TrieAnimationType::Spawn) {
//                continue;
//            }
//            if (commands[j].type == TrieAnimationType::Reconstruct) {
//                trigger_reconstruct = true;
//                continue;
//            }
//            applyCommand(commands[j], base_state, new_state, 1.0f);
//            phase_duration = std::max(phase_duration, commands[j].duration);
//        }
//
//        if (trigger_reconstruct) {
//            std::vector<TrieAnimationNode> node_list;
//            std::vector<TrieAnimationEdge> edge_list;
//            std::unordered_map<int, float> subtree_width;
//            calculateSubtreeWidth(0, fin_state, subtree_width);
//            reconstructTree(0, fin_state, (float)X_MARGIN, (float)Y_MARGIN, subtree_width, node_list, edge_list);
//            new_state.setNodeList(node_list);
//            new_state.setEdgeList(edge_list);
//        }
//
//        start_time.push_back(start_time.back() + phase_duration);
//        normalizeEdgeLists(new_state);
//        base_states.push_back(new_state);
//        total_duration += phase_duration;
//    }
//}
//
//float TrieAnimator::calculateSubtreeWidth(int u_idx, const TrieState& state, std::unordered_map<int, float>& subtree_width) {
//    const auto& snp = state.nodes[u_idx];
//    float total_w = 0.f;
//    bool has_child = false;
//    for (int i = 0; i < 26; i++) {
//        if (snp.child[i] != -1) {
//            total_w += calculateSubtreeWidth(snp.child[i], state, subtree_width);
//            has_child = true;
//        }
//    }
//    float width = has_child ? total_w : (float)NODE_RADIUS * 2.5f;
//    subtree_width[snp.ui_id] = width;
//    return width;
//}
//
//void TrieAnimator::reconstructTree(int u_idx, const TrieState& state, float x, float y, std::unordered_map<int, float>& subtree_width, std::vector<TrieAnimationNode>& node_list, std::vector<TrieAnimationEdge>& edge_list) {
//    const auto& snp = state.nodes[u_idx];
//    TrieAnimationNode node;
//    node.value = snp.label;
//    node.ui_id = snp.ui_id;
//    node.position = { x, y };
//    node.alpha = 255;
//    node.fill_color = snp.isEnd ? END_NODE_COLOR : DEFAULT_NODE_COLOR;
//    node_list.push_back(node);
//
//    float current_x = x - (subtree_width[snp.ui_id] / 2.f);
//    for (int i = 0; i < 26; i++) {
//        if (snp.child[i] != -1) {
//            int child_snp_idx = snp.child[i];
//            float child_w = subtree_width[state.nodes[child_snp_idx].ui_id];
//            float child_x = current_x + (child_w / 2.f);
//            float child_y = y + LEVEL_GAP;
//            reconstructTree(child_snp_idx, state, child_x, child_y, subtree_width, node_list, edge_list);
//            TrieAnimationEdge edge;
//            edge.from_ui_id = snp.ui_id;
//            edge.to_ui_id = state.nodes[child_snp_idx].ui_id;
//            edge.alpha = 255;
//            edge.fill_color = DEFAULT_EDGE_COLOR;
//            edge_list.push_back(edge);
//            current_x += child_w;
//        }
//    }
//}
//
//void TrieAnimator::generateAnimationState(TrieAnimationState& animation_state, const TrieState& state) {
//    std::vector<TrieAnimationNode> node_list;
//    std::vector<TrieAnimationEdge> edge_list;
//    if (state.nodes.empty()) {
//        return;
//    }
//    std::unordered_map<int, float> subtree_width;
//    calculateSubtreeWidth(0, state, subtree_width);
//    reconstructTree(0, state, (float)X_MARGIN, (float)Y_MARGIN, subtree_width, node_list, edge_list);
//    animation_state.setNodeList(node_list);
//    animation_state.setEdgeList(edge_list);
//    normalizeEdgeLists(animation_state);
//}
//
//TrieAnimationState TrieAnimator::getStateAtTime(float t) {
//    if (base_states.empty()) {
//        return TrieAnimationState();
//    }
//    if (t >= total_duration) {
//        return base_states.back();
//    }
//    int phase_index = 0;
//    for (int i = 0; i < (int)start_time.size(); i++) {
//        if (start_time[i] < t) {
//            phase_index = i;
//        }
//        else {
//            break;
//        }
//    }
//    const TrieAnimationState& base_state = base_states[phase_index];
//    TrieAnimationState new_state = base_state;
//    std::vector<TrieAnimationCommand> commands = phases[phase_index].commands;
//    float phase_time = t - start_time[phase_index];
//    float duration = (phase_index + 1 < (int)start_time.size()) ? (start_time[phase_index + 1] - start_time[phase_index]) : (total_duration - start_time[phase_index]);
//    float progress = (duration > 0.f) ? (phase_time / duration) : 1.f;
//    for (int i = 0; i < (int)commands.size(); i++) {
//        if (commands[i].type == TrieAnimationType::Spawn) {
//            continue;
//        }
//        applyCommand(commands[i], base_state, new_state, progress);
//    }
//    normalizeEdgeLists(new_state);
//    return new_state;
//}
//
//static float lerpFloat(float a, float b, float t) {
//    return a + (b - a) * t;
//}
//
//static sf::Vector2f lerpVector2f(const sf::Vector2f& a, const sf::Vector2f& b, float t) {
//    return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
//}
//
//static std::uint8_t lerpByte(std::uint8_t a, std::uint8_t b, float t) {
//    return static_cast<std::uint8_t>(a + (b - a) * t);
//}
//
//static sf::Color lerpColor(const sf::Color& a, const sf::Color& b, float t) {
//    return sf::Color(
//        lerpByte(a.r, b.r, t),
//        lerpByte(a.g, b.g, t),
//        lerpByte(a.b, b.b, t),
//        lerpByte(a.a, b.a, t)
//    );
//}
//
//void TrieAnimator::applyCommandOnNode(TrieAnimationNode& node, const TrieAnimationCommand& command, const float& progress) const {
//    using Type = TrieAnimationType;
//    Type type = command.type;
//    if (type == Type::FadeIn) {
//        node.alpha = lerpByte(0, 255, progress);
//    }
//    else if (type == Type::FadeOut) {
//        node.alpha = lerpByte(255, 0, progress);
//    }
//    else if (type == Type::HighlightOn) {
//        node.fill_color = lerpColor(DEFAULT_NODE_COLOR, HIGHLIGHT_NODE_COLOR, progress);
//    }
//    else if (type == Type::HighlightOff) {
//        node.fill_color = lerpColor(HIGHLIGHT_NODE_COLOR, DEFAULT_NODE_COLOR, progress);
//    }
//    else if (type == Type::FoundedOn) {
//        node.fill_color = lerpColor(HIGHLIGHT_NODE_COLOR, FOUNDED_NODE_COLOR, progress);
//    }
//    else if (type == Type::FoundedOff) {
//        node.fill_color = lerpColor(FOUNDED_NODE_COLOR, DEFAULT_NODE_COLOR, progress);
//    }
//    else if (type == Type::Move) {
//        sf::Vector2f offset = { 0.f, 0.f };
//        if (command.direction == TrieMoveDirection::Right) offset.x = (float)move_distance;
//        else if (command.direction == TrieMoveDirection::Left) offset.x = (float)-move_distance;
//        else if (command.direction == TrieMoveDirection::Up) offset.y = (float)-move_distance;
//        else if (command.direction == TrieMoveDirection::Down) offset.y = (float)move_distance;
//        node.position = lerpVector2f(node.position, node.position + offset, progress);
//    }
//}
//
//void TrieAnimator::applyCommandOnEdge(TrieAnimationEdge& edge, const TrieAnimationCommand& command, const float& progress) const {
//    using Type = TrieAnimationType;
//    if (command.type == Type::FadeIn) {
//        edge.alpha = lerpByte(0, 255, progress);
//    }
//    else if (command.type == Type::FadeOut) {
//        edge.alpha = lerpByte(255, 0, progress);
//    }
//}
//
//void TrieAnimator::applySpawnCommand(const TrieAnimationCommand& command, TrieAnimationState& state, const TrieAnimationState& base_state) const {
//    if (command.target == TrieAnimationTarget::Node) {
//        TrieAnimationNode new_node;
//        new_node.value = command.value;
//        new_node.ui_id = command.ui_id;
//        new_node.position = { (float)X_MARGIN, (float)Y_MARGIN };
//        for (const auto& node : base_state.getNodeList()) {
//            if (node.ui_id == command.spawn_from_ui_id) {
//                new_node.position = node.position + command.spawn_offset;
//                break;
//            }
//        }
//        new_node.alpha = 0;
//        new_node.fill_color = DEFAULT_NODE_COLOR;
//        state.insertNode(new_node);
//    }
//    else {
//        TrieAnimationEdge new_edge;
//        new_edge.from_ui_id = command.from_ui_id;
//        new_edge.to_ui_id = command.to_ui_id;
//        new_edge.alpha = 0;
//        new_edge.fill_color = DEFAULT_EDGE_COLOR;
//        state.insertEdge(new_edge);
//    }
//}
//
//void TrieAnimator::applyCommand(const TrieAnimationCommand& command, const TrieAnimationState& base_state, TrieAnimationState& state, const float& progress) const {
//    if (command.target == TrieAnimationTarget::All) {
//        const auto& nodes = base_state.getNodeList();
//        for (int i = 0; i < (int)nodes.size(); i++) {
//            TrieAnimationNode new_node = nodes[i];
//            applyCommandOnNode(new_node, command, progress);
//            state.modifyNode(new_node, i);
//        }
//        const auto& edges = base_state.getEdgeList();
//        for (int i = 0; i < (int)edges.size(); i++) {
//            TrieAnimationEdge new_edge = edges[i];
//            applyCommandOnEdge(new_edge, command, progress);
//            state.modifyEdge(new_edge, i);
//        }
//        return;
//    }
//    if (command.target == TrieAnimationTarget::Node) {
//        const auto& nodes = base_state.getNodeList();
//        for (int i = 0; i < (int)nodes.size(); i++) {
//            if (nodes[i].ui_id == command.ui_id) {
//                TrieAnimationNode new_node = nodes[i];
//                applyCommandOnNode(new_node, command, progress);
//                state.modifyNode(new_node, i);
//                return;
//            }
//        }
//    }
//    else {
//        const auto& edges = base_state.getEdgeList();
//        for (int i = 0; i < (int)edges.size(); i++) {
//            if (edges[i].from_ui_id == command.from_ui_id && edges[i].to_ui_id == command.to_ui_id) {
//                TrieAnimationEdge new_edge = edges[i];
//                applyCommandOnEdge(new_edge, command, progress);
//                state.modifyEdge(new_edge, i);
//                return;
//            }
//        }
//    }
//}
//
//void TrieAnimator::normalizeEdgeLists(TrieAnimationState& animation_state) {
//    std::vector<TrieAnimationEdge> edge_list = animation_state.getEdgeList();
//    const std::vector<TrieAnimationNode>& node_list = animation_state.getNodeList();
//    std::unordered_map<int, sf::Vector2f> pos_map;
//    for (const auto& node : node_list) pos_map[node.ui_id] = node.position;
//    for (auto& edge : edge_list) {
//        if (pos_map.count(edge.from_ui_id)) edge.from_position = pos_map[edge.from_ui_id] + sf::Vector2f(0.f, (float)NODE_RADIUS);
//        if (pos_map.count(edge.to_ui_id)) edge.to_position = pos_map[edge.to_ui_id] - sf::Vector2f(0.f, (float)NODE_RADIUS);
//    }
//    animation_state.setEdgeList(edge_list);
//}
//
//void TrieAnimator::clear() {
//    base_states.clear();
//    start_time.clear();
//    total_duration = 0.f;
//}
//
//bool TrieAnimator::isPhaseBoundary(float t) const {
//    int i = 0;
//    for (i = 0; i < (int)start_time.size(); i++) {
//        if (start_time[i] < t) continue;
//        if (start_time[i] > t) break;
//    }
//    const float EPSILON = 1e-5;
//    for (int j = i - 1; j <= i + 1; j++) {
//        if (j < 0) continue;
//        if (j >= (int)start_time.size()) break;
//        if (abs(start_time[j] - t) <= EPSILON) return 1;
//    }
//    return 0;
//}
//
//float TrieAnimator::getNextPhaseTimer(float t) const {
//    for (int i = 0; i < (int)start_time.size(); i++) {
//        if (start_time[i] > t) return start_time[i];
//    }
//    return -1;
//}
//
//float TrieAnimator::getPreviousPhaseTimer(float t) const {
//    for (int i = 0; i < (int)start_time.size(); i++) {
//        if (start_time[i] >= t && i > 0) return start_time[i - 1];
//    }
//    return -1;
//}
//
//float TrieAnimator::getLeftBound(float t) const {
//    for (int i = 0; i < (int)start_time.size(); i++) {
//        if (start_time[i] > t && i > 0) return start_time[i - 1];
//    }
//    return -1;
//}
//
//float TrieAnimator::getRightBound(float t) const {
//    for (int i = 0; i < (int)start_time.size(); i++) {
//        if (start_time[i] >= t) return start_time[i];
//    }
//    return -1;
//}
#include <Animation/Trie/TrieAnimator.h>
#include <cmath>
#include <algorithm>

static float lerpFloat(float a, float b, float t) {
	return a + (b - a) * t;
}

static std::uint8_t lerpByte(std::uint8_t a, std::uint8_t b, float t) {
	return static_cast<std::uint8_t>(a + (b - a) * t);
}

static sf::Vector2f lerpVector2f(const sf::Vector2f& a, const sf::Vector2f& b, float t) {
	return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
}

static sf::Color lerpColor(const sf::Color& a, const sf::Color& b, float t) {
	return sf::Color(lerpByte(a.r, b.r, t), lerpByte(a.g, b.g, t), lerpByte(a.b, b.b, t), lerpByte(a.a, b.a, t));
}

void TrieAnimator::generateBaseStates(const TrieState& state, const TrieState& fin_state, const TrieRecorder& record) {
	generateAnimationState(initial_state, state);
	phases = record.getPhases();
	clear();
	start_time.push_back(0.f);
	TrieAnimationState base_state_after_spawn = initial_state;

	for (int i = 0; i < (int)phases.size(); i++) {
		std::vector<TrieAnimationCommand> commands = phases[i].commands;
		for (int j = 0; j < (int)commands.size(); j++) {
			if (commands[j].type == TrieAnimationType::Spawn && commands[j].target == TrieAnimationTarget::Node) {
				TrieAnimationState tmp = base_state_after_spawn;
				applySpawnCommand(commands[j], base_state_after_spawn, tmp);
			}
		}
	}

	initial_state = base_state_after_spawn;

	for (int i = 0; i < (int)phases.size(); i++) {
		std::vector<TrieAnimationCommand> commands = phases[i].commands;
		for (int j = 0; j < (int)commands.size(); j++) {
			if (commands[j].type == TrieAnimationType::Spawn && commands[j].target == TrieAnimationTarget::Edge) {
				applySpawnCommand(commands[j], base_state_after_spawn, initial_state);
			}
		}
	}

	normalizeEdgeLists(base_state_after_spawn);
	base_states.push_back(base_state_after_spawn);

	for (int i = 0; i < (int)phases.size(); i++) {
		const TrieAnimationState& base_state = base_states.back();
		TrieAnimationState new_state = base_state;
		std::vector<TrieAnimationCommand> commands = phases[i].commands;
		float phase_duration = 0.f;

		for (int j = 0; j < (int)commands.size(); j++) {
			if (commands[j].type == TrieAnimationType::Spawn) continue;
			applyCommand(commands[j], base_state, new_state, phases[i].snapshot, 1.0f);
			phase_duration = std::max(phase_duration, commands[j].duration);
		}

		start_time.push_back(start_time.back() + phase_duration);
		normalizeEdgeLists(new_state);
		base_states.push_back(new_state);
		total_duration += phase_duration;
	}
}

float TrieAnimator::calculateSubtreeWidth(int u_idx, const TrieState& state, std::unordered_map<int, float>& subtree_width) const {
	if (u_idx == -1 || u_idx >= state.nodes.size()) return 0.f;
	const auto& snp = state.nodes[u_idx];
	float total_w = 0.f;
	bool has_child = false;
	for (int i = 0; i < 26; i++) {
		if (snp.child[i] != -1) {
			total_w += calculateSubtreeWidth(snp.child[i], state, subtree_width);
			has_child = true;
		}
	}
	float width = has_child ? total_w : (float)NODE_RADIUS * 2.5f;
	subtree_width[snp.ui_id] = width;
	return width;
}

void TrieAnimator::reconstructTree(int u_idx, const TrieState& state, float x, float y, std::unordered_map<int, float>& subtree_width, std::vector<TrieAnimationNode>& node_list, std::vector<TrieAnimationEdge>& edge_list) const {
	if (u_idx == -1 || u_idx >= state.nodes.size()) return;
	const auto& snp = state.nodes[u_idx];
	TrieAnimationNode node;
	node.value = snp.label;
	node.ui_id = snp.ui_id;
	node.position = { x, y };
	node.alpha = 255;
	node.fill_color = snp.isEnd ? END_NODE_COLOR : DEFAULT_NODE_COLOR;
	node.is_end = snp.isEnd;
	node_list.push_back(node);

	float current_x = x - (subtree_width.at(snp.ui_id) / 2.f);
	for (int i = 0; i < 26; i++) {
		if (snp.child[i] != -1) {
			int child_snp_idx = snp.child[i];
			float child_w = subtree_width.at(state.nodes[child_snp_idx].ui_id);
			float child_x = current_x + (child_w / 2.f);
			float child_y = y + LEVEL_GAP;
			reconstructTree(child_snp_idx, state, child_x, child_y, subtree_width, node_list, edge_list);

			TrieAnimationEdge edge;
			edge.from_ui_id = snp.ui_id;
			edge.to_ui_id = state.nodes[child_snp_idx].ui_id;
			edge.alpha = 255;
			edge.fill_color = DEFAULT_EDGE_COLOR;
			edge_list.push_back(edge);

			current_x += child_w;
		}
	}
}

void TrieAnimator::generateAnimationState(TrieAnimationState& animation_state, const TrieState& state) const {
	std::vector<TrieAnimationNode> node_list;
	std::vector<TrieAnimationEdge> edge_list;
	if (state.nodes.empty()) return;
	std::unordered_map<int, float> subtree_width;
	calculateSubtreeWidth(0, state, subtree_width);
	reconstructTree(0, state, (float)X_MARGIN, (float)Y_MARGIN, subtree_width, node_list, edge_list);
	animation_state.setNodeList(node_list);
	animation_state.setEdgeList(edge_list);
	normalizeEdgeLists(animation_state);
}

TrieAnimationState TrieAnimator::getStateAtTime(float t) {
	if (base_states.empty()) return TrieAnimationState();
	if (t >= total_duration) return base_states.back();
	int phase_index = 0;
	for (int i = 0; i < (int)start_time.size(); i++) {
		if (start_time[i] < t) phase_index = i;
		else break;
	}
	const TrieAnimationState& base_state = base_states[phase_index];
	TrieAnimationState new_state = base_state;
	std::vector<TrieAnimationCommand> commands = phases[phase_index].commands;
	float phase_time = t - start_time[phase_index];
	float duration = (phase_index + 1 < (int)start_time.size()) ? (start_time[phase_index + 1] - start_time[phase_index]) : (total_duration - start_time[phase_index]);
	float progress = (duration > 0.f) ? (phase_time / duration) : 1.f;

	for (int i = 0; i < (int)commands.size(); i++) {
		if (commands[i].type == TrieAnimationType::Spawn) continue;
		applyCommand(commands[i], base_state, new_state, phases[phase_index].snapshot, progress);
	}

	normalizeEdgeLists(new_state);
	return new_state;
}

void TrieAnimator::applyCommandOnNode(TrieAnimationNode& node, const TrieAnimationCommand& command, const float& progress) const {
	using Type = TrieAnimationType;
	if (command.type == Type::FadeIn) node.alpha = lerpByte(0, 255, progress);
	else if (command.type == Type::FadeOut) node.alpha = lerpByte(255, 0, progress);
	else if (command.type == Type::HighlightOn) {
		node.fill_color = lerpColor(node.is_end ? END_NODE_COLOR : DEFAULT_NODE_COLOR, HIGHLIGHT_NODE_COLOR, progress);
	}
	else if (command.type == Type::HighlightOff) {
		node.fill_color = lerpColor(HIGHLIGHT_NODE_COLOR, node.is_end ? END_NODE_COLOR : DEFAULT_NODE_COLOR, progress);
	}
	else if (command.type == Type::FoundedOn) {
		node.fill_color = lerpColor(node.is_end ? END_NODE_COLOR : DEFAULT_NODE_COLOR, FOUNDED_NODE_COLOR, progress);
	}
	else if (command.type == Type::FoundedOff) {
		node.fill_color = lerpColor(FOUNDED_NODE_COLOR, node.is_end ? END_NODE_COLOR : DEFAULT_NODE_COLOR, progress);
	}
	else if (command.type == Type::SetEndMark) node.fill_color = lerpColor(DEFAULT_NODE_COLOR, END_NODE_COLOR, progress);
	else if (command.type == Type::UnsetEndMark) node.fill_color = lerpColor(END_NODE_COLOR, DEFAULT_NODE_COLOR, progress);
	else if (command.type == Type::Move) {
		sf::Vector2f offset = { 0.f, 0.f };
		if (command.direction == TrieMoveDirection::Right) offset.x = (float)move_distance;
		else if (command.direction == TrieMoveDirection::Left) offset.x = (float)-move_distance;
		else if (command.direction == TrieMoveDirection::Up) offset.y = (float)-move_distance;
		else if (command.direction == TrieMoveDirection::Down) offset.y = (float)move_distance;
		node.position = lerpVector2f(node.position, node.position + offset, progress);
	}
}

void TrieAnimator::applyCommandOnEdge(TrieAnimationEdge& edge, const TrieAnimationCommand& command, const float& progress) const {
	using Type = TrieAnimationType;
	if (command.type == Type::FadeIn) edge.alpha = lerpByte(0, 255, progress);
	else if (command.type == Type::FadeOut) edge.alpha = lerpByte(255, 0, progress);
}

void TrieAnimator::applySpawnCommand(const TrieAnimationCommand& command, TrieAnimationState& state, const TrieAnimationState& base_state) const {
	if (command.target == TrieAnimationTarget::Node) {
		TrieAnimationNode new_node;
		new_node.value = command.value;
		new_node.ui_id = command.ui_id;
		new_node.position = { (float)X_MARGIN, (float)Y_MARGIN };
		for (const auto& node : base_state.getNodeList()) {
			if (node.ui_id == command.spawn_from_ui_id) {
				new_node.position = node.position + command.spawn_offset;
				break;
			}
		}
		new_node.alpha = 0;
		new_node.fill_color = DEFAULT_NODE_COLOR;
		state.insertNode(new_node);
	}
	else {
		TrieAnimationEdge new_edge;
		new_edge.from_ui_id = command.from_ui_id;
		new_edge.to_ui_id = command.to_ui_id;
		new_edge.alpha = 0;
		new_edge.fill_color = DEFAULT_EDGE_COLOR;
		state.insertEdge(new_edge);
	}
}

void TrieAnimator::applyCommand(const TrieAnimationCommand& command, const TrieAnimationState& base_state, TrieAnimationState& state, const std::optional<TrieState>& snapshot, const float& progress) const {
	if (command.target == TrieAnimationTarget::All) {
		if (command.type == TrieAnimationType::Reconstruct) {
			if (!snapshot.has_value()) return;
			TrieAnimationState tmp_state;
			TrieState Trie_snapshot = snapshot.value();
			generateAnimationState(tmp_state, Trie_snapshot);

			const auto& base_nodes = base_state.getNodeList();
			const auto& new_nodes = tmp_state.getNodeList();
			std::unordered_map<int, sf::Vector2f> new_pos;

			for (int i = 0; i < new_nodes.size(); i++) {
				new_pos[new_nodes[i].ui_id] = new_nodes[i].position;
			}

			for (int i = 0; i < base_nodes.size(); i++) {
				int ui_id = base_nodes[i].ui_id;
				TrieAnimationNode n = base_nodes[i];
				if (new_pos.count(ui_id)) {
					n.position = lerpVector2f(n.position, new_pos[ui_id], progress);
				}
				state.modifyNode(n, i);
			}
			normalizeEdgeLists(state);
			return;
		}
		else {
			const auto& nodes = base_state.getNodeList();
			for (int i = 0; i < (int)nodes.size(); i++) {
				TrieAnimationNode n = nodes[i];
				applyCommandOnNode(n, command, progress);
				state.modifyNode(n, i);
			}
			const auto& edges = base_state.getEdgeList();
			for (int i = 0; i < (int)edges.size(); i++) {
				TrieAnimationEdge e = edges[i];
				applyCommandOnEdge(e, command, progress);
				state.modifyEdge(e, i);
			}
			return;
		}
	}

	if (command.target == TrieAnimationTarget::Node) {
		const auto& nodes = base_state.getNodeList();
		for (int i = 0; i < (int)nodes.size(); i++) {
			if (nodes[i].ui_id == command.ui_id) {
				TrieAnimationNode n = nodes[i];
				applyCommandOnNode(n, command, progress);
				state.modifyNode(n, i);
				return;
			}
		}
	}
	else {
		const auto& edges = base_state.getEdgeList();
		for (int i = 0; i < (int)edges.size(); i++) {
			if (edges[i].from_ui_id == command.from_ui_id && edges[i].to_ui_id == command.to_ui_id) {
				TrieAnimationEdge e = edges[i];
				applyCommandOnEdge(e, command, progress);
				state.modifyEdge(e, i);
				return;
			}
		}
	}
}

void TrieAnimator::normalizeEdgeLists(TrieAnimationState& animation_state) const {
	std::vector<TrieAnimationEdge> edge_list = animation_state.getEdgeList();
	const std::vector<TrieAnimationNode>& node_list = animation_state.getNodeList();
	std::unordered_map<int, sf::Vector2f> pos_map;
	for (const auto& node : node_list) pos_map[node.ui_id] = node.position;
	for (auto& edge : edge_list) {
		if (pos_map.count(edge.from_ui_id)) edge.from_position = pos_map[edge.from_ui_id] + sf::Vector2f(0.f, (float)NODE_RADIUS);
		if (pos_map.count(edge.to_ui_id)) edge.to_position = pos_map[edge.to_ui_id] - sf::Vector2f(0.f, (float)NODE_RADIUS);
	}
	animation_state.setEdgeList(edge_list);
}

void TrieAnimator::clear() {
	base_states.clear();
	start_time.clear();
	total_duration = 0.f;
}

bool TrieAnimator::isPhaseBoundary(float t) const {
	const float EPSILON = 1e-5;
	for (float start : start_time) if (std::abs(start - t) <= EPSILON) return true;
	return false;
}

float TrieAnimator::getNextPhaseTimer(float t) const {
	for (float start : start_time) if (start > t) return start;
	return -1.f;
}

float TrieAnimator::getPreviousPhaseTimer(float t) const {
	for (int i = (int)start_time.size() - 1; i >= 0; i--) if (start_time[i] < t) return start_time[i];
	return -1.f;
}

float TrieAnimator::getLeftBound(float t) const {
	for (int i = (int)start_time.size() - 1; i >= 0; i--) if (start_time[i] <= t) return start_time[i];
	return 0.f;
}

float TrieAnimator::getRightBound(float t) const {
	for (float start : start_time) if (start >= t) return start;
	return total_duration;
}