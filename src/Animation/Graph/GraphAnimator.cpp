#include <Animation/Graph/GraphAnimator.h>
#include <cmath>
#include <algorithm>

static int string_to_int(const std::string& s) {
	if (s.empty()) return 0;
	bool sign = s[0] == '-';
	int res = 0;
	for (int i = sign; i < s.size(); i++) {
		res = res * 10 + s[i] - '0';
	}
	if (sign) res = -res;
	return res;
}

static std::string int_to_string(int n) {
	std::string res = "";
	bool sign = n < 0;
	if (sign) n = -n;
	while (n) {
		char c = n % 10 + '0';
		res += c;
		n /= 10;
	}
	if (res.empty()) res += '0';
	if (sign) res += '-';
	reverse(res.begin(), res.end());
	return res;
}

void GraphAnimator::generateBaseStates(const GraphState& state, const GraphState& fin_state, const GraphRecorder& record) {
	generateAnimationState(initial_state, state);
	phases = record.getPhases();
	highlighted_lines = record.getHighlightedLines();
	clear();
	start_time.push_back(0.f);
	GraphAnimationState base_state_after_spawn = initial_state;
	for (int i = 0; i < (int)phases.size(); i++) {
		std::vector<GraphAnimationCommand> commands = phases[i].commands;
		for (int j = 0; j < (int)commands.size(); j++) {
			if (commands[j].type == GraphAnimationType::Spawn && commands[j].target == GraphAnimationTarget::Node) {
				GraphAnimationState tmp = base_state_after_spawn;
				applySpawnCommand(commands[j], base_state_after_spawn, tmp);
			}
		}
	}
	initial_state = base_state_after_spawn;
	for (int i = 0; i < (int)phases.size(); i++) {
		std::vector<GraphAnimationCommand> commands = phases[i].commands;
		for (int j = 0; j < (int)commands.size(); j++) {
			if (commands[j].type == GraphAnimationType::Spawn && commands[j].target == GraphAnimationTarget::Edge) {
				applySpawnCommand(commands[j], base_state_after_spawn, initial_state);
			}
		}
	}
	normalizeEdgeLists(base_state_after_spawn);
	base_states.push_back(base_state_after_spawn);
	for (int i = 0; i < (int)phases.size(); i++) {
		const GraphAnimationState& base_state = base_states.back();
		GraphAnimationState new_state = base_state;
		std::vector<GraphAnimationCommand> commands = phases[i].commands;
		float phase_duration = 0.f;
		for (int j = 0; j < (int)commands.size(); j++) {
			if (commands[j].type == GraphAnimationType::Spawn) continue;
			applyCommand(commands[j], base_state, new_state, phases[i].snapshot, 1.0f);
			phase_duration = std::max(phase_duration, commands[j].duration);
		}
		//if (trigger_reconstruct && !fin_state.nodes.empty()) {
		//	std::vector<GraphAnimationNode> node_list;
		//	std::vector<GraphAnimationEdge> edge_list;
		//	std::unordered_map<int, float> subtree_width;
		//	calculateSubtreeWidth(0, fin_state, subtree_width);
		//	reconstructTree(0, fin_state, (float)X_MARGIN, (float)Y_MARGIN, subtree_width, node_list, edge_list);
		//	new_state.setNodeList(node_list);
		//	new_state.setEdgeList(edge_list);
		//}
		//if (phase_duration == 0.f) phase_duration = 0.5f;
		start_time.push_back(start_time.back() + phase_duration);
		normalizeEdgeLists(new_state);
		base_states.push_back(new_state);
		total_duration += phase_duration;
	}
}

//float GraphAnimator::calculateSubtreeWidth(int u_idx, const GraphState& state, std::unordered_map<int, float>& subtree_width) const {
//	if (u_idx == -1 || u_idx >= state.nodes.size()) return 0.f;
//	const auto& snp = state.nodes[u_idx];
//	float left_w = calculateSubtreeWidth(snp.leftChild, state, subtree_width);
//	float right_w = calculateSubtreeWidth(snp.rightChild, state, subtree_width);
//	float width = std::max(left_w + right_w, (float)NODE_RADIUS * 2.5f);
//	subtree_width[snp.ui_id] = width;
//	return width;
//}

//void GraphAnimator::reconstructTree(int u_idx, const GraphState& state, float x, float y, std::unordered_map<int, float>& subtree_width, std::vector<GraphAnimationNode>& node_list, std::vector<GraphAnimationEdge>& edge_list) const {
//	if (u_idx == -1 || u_idx >= state.nodes.size()) return;
//	const auto& snp = state.nodes[u_idx];
//	GraphAnimationNode node;
//	node.value = snp.value;
//	node.ui_id = snp.ui_id;
//	node.position = { x, y };
//	node.alpha = 255;
//	node.fill_color = DEFAULT_NODE_COLOR;
//	node_list.push_back(node);
//	float total_w = subtree_width[snp.ui_id];
//	if (snp.leftChild != -1) {
//		float child_w = subtree_width[state.nodes[snp.leftChild].ui_id];
//		float child_x = x - (total_w / 2.f) + (child_w / 2.f);
//		float child_y = y + LEVEL_GAP;
//		reconstructTree(snp.leftChild, state, child_x, child_y, subtree_width, node_list, edge_list);
//		GraphAnimationEdge edge;
//		edge.from_ui_id = snp.ui_id;
//		edge.to_ui_id = state.nodes[snp.leftChild].ui_id;
//		edge.alpha = 255;
//		edge.fill_color = DEFAULT_EDGE_COLOR;
//		edge_list.push_back(edge);
//	}
//	if (snp.rightChild != -1) {
//		float child_w = subtree_width[state.nodes[snp.rightChild].ui_id];
//		float child_x = x + (total_w / 2.f) - (child_w / 2.f);
//		float child_y = y + LEVEL_GAP;
//		reconstructTree(snp.rightChild, state, child_x, child_y, subtree_width, node_list, edge_list);
//		GraphAnimationEdge edge;
//		edge.from_ui_id = snp.ui_id;
//		edge.to_ui_id = state.nodes[snp.rightChild].ui_id;
//		edge.alpha = 255;
//		edge.fill_color = DEFAULT_EDGE_COLOR;
//		edge_list.push_back(edge);
//	}
//}

void GraphAnimator::generateAnimationState(GraphAnimationState& animation_state, const GraphState& state) const {
	std::vector<GraphAnimationNode> node_list;
	std::vector<GraphAnimationEdge> edge_list;
	for (const auto& [value, node] : state.nodes) {
		GraphAnimationNode anim_node;
		anim_node.value = value;
		anim_node.ui_id = node.ui_id;
		anim_node.alpha = 255;
		anim_node.disable_physics = false;
		anim_node.fill_color = DEFAULT_NODE_COLOR;
		node_list.push_back(anim_node);
	}
	for (const auto& [edge_pair, weight] : state.edges) {
		int u_val = edge_pair.first;
		int v_val = edge_pair.second;
		if (state.nodes.find(u_val) != state.nodes.end() && state.nodes.find(v_val) != state.nodes.end()) {
			GraphAnimationEdge anim_edge;
			anim_edge.from_ui_id = state.nodes.at(u_val).ui_id;
			anim_edge.to_ui_id = state.nodes.at(v_val).ui_id;
			anim_edge.alpha = 255; 
			anim_edge.disable_physics = false;
			anim_edge.fill_color = DEFAULT_EDGE_COLOR;
			edge_list.push_back(anim_edge);
		}
	}
	animation_state.setNodeList(node_list);
	animation_state.setEdgeList(edge_list);
}

int GraphAnimator::getHighlightedLine(float t) const {
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

GraphAnimationState GraphAnimator::getStateAtTime(float t) {
	if (base_states.empty()) return GraphAnimationState();
	if (t >= total_duration) return base_states.back();
	int phase_index = 0;
	for (int i = 0; i < (int)start_time.size(); i++) {
		if (start_time[i] < t) phase_index = i;
		else break;
	}
	const GraphAnimationState& base_state = base_states[phase_index];
	GraphAnimationState new_state = base_state;
	std::vector<GraphAnimationCommand> commands = phases[phase_index].commands;
	float phase_time = t - start_time[phase_index];
	float duration = (phase_index + 1 < (int)start_time.size()) ? (start_time[phase_index + 1] - start_time[phase_index]) : (total_duration - start_time[phase_index]);
	float progress = (duration > 0.f) ? (phase_time / duration) : 1.f;
	for (int i = 0; i < (int)commands.size(); i++) {
		if (commands[i].type == GraphAnimationType::Spawn) continue;
		applyCommand(commands[i], base_state, new_state, phases[phase_index].snapshot, progress);
	}
	normalizeEdgeLists(new_state);
	return new_state;
}

static std::uint8_t lerpByte(std::uint8_t a, std::uint8_t b, float t) { return static_cast<std::uint8_t>(a + (b - a) * t); }
static sf::Vector2f lerpVector2f(const sf::Vector2f& a, const sf::Vector2f& b, float t) { return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t }; }
static sf::Color lerpColor(const sf::Color& a, const sf::Color& b, float t) {
	return sf::Color(lerpByte(a.r, b.r, t), lerpByte(a.g, b.g, t), lerpByte(a.b, b.b, t), lerpByte(a.a, b.a, t));
}

void GraphAnimator::applyCommandOnNode(GraphAnimationNode& node, const GraphAnimationCommand& command, const float& progress) const {
	using Type = GraphAnimationType;
	if (command.type == Type::FadeIn) {
		node.alpha = lerpByte(0, 255, progress);
		if (node.alpha == 255) node.disable_physics = 0;
	}
	else if (command.type == Type::FadeOut) {
		node.alpha = lerpByte(255, 0, progress);
		if (node.alpha == 0) node.disable_physics = 1;
	}
	else if (command.type == Type::HighlightOn) node.fill_color = lerpColor(DEFAULT_NODE_COLOR, HIGHLIGHT_NODE_COLOR, progress);
	else if (command.type == Type::HighlightOff) node.fill_color = lerpColor(HIGHLIGHT_NODE_COLOR, DEFAULT_NODE_COLOR, progress);
}

void GraphAnimator::applyCommandOnEdge(GraphAnimationEdge& edge, const GraphAnimationCommand& command, const float& progress) const {
	if (command.type == GraphAnimationType::FadeIn) {
		edge.alpha = lerpByte(0, 255, progress);
		if (edge.alpha == 255) edge.disable_physics = 0;
	}
	else if (command.type == GraphAnimationType::FadeOut) {
		edge.alpha = lerpByte(255, 0, progress);
		if (edge.alpha == 0) edge.disable_physics = 1;
	}
	else if (command.type == GraphAnimationType::HighlightOn) edge.fill_color = lerpColor(DEFAULT_EDGE_COLOR, HIGHLIGHT_EDGE_COLOR, progress);
	else if (command.type == GraphAnimationType::HighlightOff) edge.fill_color = lerpColor(HIGHLIGHT_EDGE_COLOR, DEFAULT_EDGE_COLOR, progress);
}

void GraphAnimator::applySpawnCommand(const GraphAnimationCommand& command, GraphAnimationState& state, const GraphAnimationState& base_state) const {
	if (command.target == GraphAnimationTarget::Node) {
		GraphAnimationNode new_node;
		new_node.value = command.value;
		new_node.ui_id = command.ui_id;
		new_node.alpha = 0;
		new_node.disable_physics = false;
		new_node.fill_color = DEFAULT_NODE_COLOR;
		state.insertNode(new_node);
	}
	else {
		GraphAnimationEdge new_edge;
		new_edge.from_ui_id = command.from_ui_id;
		new_edge.to_ui_id = command.to_ui_id;
		new_edge.alpha = 0;
		new_edge.disable_physics = false;
		new_edge.fill_color = DEFAULT_EDGE_COLOR;
		state.insertEdge(new_edge);
	}
}

void GraphAnimator::applyCommand(const GraphAnimationCommand& command, const GraphAnimationState& base_state, GraphAnimationState& state, const std::optional<GraphState>& snapshot, const float& progress) const {
	using Type = GraphAnimationType;
	if (command.type == Type::Wait) return;
	if (command.target == GraphAnimationTarget::All) {
		const auto& nodes = base_state.getNodeList();
		for (int i = 0; i < (int)nodes.size(); i++) {
			GraphAnimationNode n = nodes[i];
			applyCommandOnNode(n, command, progress);
			state.modifyNode(n, i);
		}
		const auto& edges = base_state.getEdgeList();
		for (int i = 0; i < (int)edges.size(); i++) {
			GraphAnimationEdge e = edges[i];
			applyCommandOnEdge(e, command, progress);
			state.modifyEdge(e, i);
		}
		return;
	}
	if (command.target == GraphAnimationTarget::Node) {
		const auto& nodes = base_state.getNodeList();
		for (int i = 0; i < (int)nodes.size(); i++) {
			if (nodes[i].ui_id == command.ui_id) {
				GraphAnimationNode n = nodes[i];
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
				GraphAnimationEdge e = edges[i];
				applyCommandOnEdge(e, command, progress);
				state.modifyEdge(e, i);
				return;
			}
		}
	}
}

void GraphAnimator::normalizeEdgeLists(GraphAnimationState& animation_state) const {
	//std::vector<GraphAnimationEdge> edge_list = animation_state.getEdgeList();
	//const std::vector<GraphAnimationNode>& node_list = animation_state.getNodeList();
	//std::unordered_map<int, sf::Vector2f> pos_map;
	//for (const auto& node : node_list) pos_map[node.ui_id] = node.position;
	//for (auto& edge : edge_list) {
	//	if (pos_map.count(edge.from_ui_id)) edge.from_position = pos_map[edge.from_ui_id] + sf::Vector2f(0.f, (float)NODE_RADIUS);
	//	if (pos_map.count(edge.to_ui_id)) edge.to_position = pos_map[edge.to_ui_id] - sf::Vector2f(0.f, (float)NODE_RADIUS);
	//}
	//animation_state.setEdgeList(edge_list);
}

void GraphAnimator::clear() {
	base_states.clear();
	start_time.clear();
	total_duration = 0.f;
}

bool GraphAnimator::isPhaseBoundary(float t) const {
	const float EPSILON = 1e-5;
	for (float start : start_time) if (std::abs(start - t) <= EPSILON) return true;
	return false;
}

float GraphAnimator::getNextPhaseTimer(float t) const {
	for (float start : start_time) if (start > t) return start;
	return -1.f;
}

float GraphAnimator::getPreviousPhaseTimer(float t) const {
	for (int i = (int)start_time.size() - 1; i >= 0; i--) if (start_time[i] < t) return start_time[i];
	return -1.f;
}

float GraphAnimator::getLeftBound(float t) const {
	for (int i = (int)start_time.size() - 1; i >= 0; i--) if (start_time[i] <= t) return start_time[i];
	return 0.f;
}

float GraphAnimator::getRightBound(float t) const {
	for (float start : start_time) if (start >= t) return start;
	return total_duration;
}