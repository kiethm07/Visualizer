#pragma once
#include <Animation/Trie/TrieRecorder.h>
#include <Animation/Trie/TrieAnimationState.h>
#include <Model/TrieModel/TrieState.h>
#include <vector>
#include <iostream>

class TrieAnimator {
public:
	void generateBaseStates(const TrieState& initial_state, const TrieRecorder& record);
	TrieAnimationState getStateAtTime(float t); //Get the state of the animation at time t, used to draw the animation
	float getTotalDuration() const {
		return total_duration;
	}
	float getNextPhaseTimer(float t) const;
	float getPreviousPhaseTimer(float t) const;
	float getLeftBound(float t) const;
	float getRightBound(float t) const;
	bool isPhaseBoundary(float t) const; // Return -1 if not the boundary, or the boundary itself
	void clear();
private:
	void normalizeEdgeLists(TrieAnimationState& animation_state);
	void generateAnimationState(TrieAnimationState& animation_state, const TrieState& state);
	void applyCommand(const TrieAnimationCommand& command, const TrieAnimationState& base_state, TrieAnimationState& state, const float& progress) const;
	void applySpawnCommand(const TrieAnimationCommand& command, TrieAnimationState& state, const TrieAnimationState& base_state) const;
	void applyCommandOnNode(TrieAnimationNode& node, const TrieAnimationCommand& command, const float& progress) const;
	void applyCommandOnEdge(TrieAnimationEdge& edge, const TrieAnimationCommand& command, const float& progress) const;
	TrieAnimationState initial_state;
	std::vector<TrieAnimationState> base_states; //The base state of each phase
	std::vector<float> start_time; //The start time of each phase, used to calculate the current animation state
	std::vector<TrieAnimationPhase> phases;
	float total_duration = 0.f;
	const int NODE_RADIUS = 50;
	const int NODE_GAP = 2 * NODE_RADIUS + 100;
	const int X_MARGIN = 400;
	const int Y_MARGIN = 200;
	const sf::Color DEFAULT_NODE_COLOR = sf::Color(89, 145, 46);
	const sf::Color DEFAULT_EDGE_COLOR = sf::Color(255, 255, 255);
	const sf::Color HIGHLIGHT_NODE_COLOR = sf::Color(209, 70, 38);
	const sf::Color FOUNDED_NODE_COLOR = sf::Color::Blue;
	int move_distance = NODE_GAP;
};