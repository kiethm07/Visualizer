#pragma once
#include <Animation/LinkedList/LinkedListRecorder.h>
#include <Animation/LinkedList/LinkedListAnimationState.h>
#include <Model/LinkedListModel/LinkedListState.h>
#include <vector>
#include <iostream>

class LinkedListAnimator{
public:
	void generateBaseStates(const LinkedListState& initial_state, const LinkedListRecorder& record);
	LinkedListAnimationState getStateAtTime(float t) const; //Get the state of the animation at time t, used to draw the animation
	float getTotalDuration() const {
		return total_duration;
	}
private:
	void generateAnimationState(LinkedListAnimationState& animation_state, const LinkedListState& state);
	void applyCommand(const LinkedListAnimationCommand& command, const LinkedListAnimationState& base_state, LinkedListAnimationState& state, const float& progress) const;
	void applySpawnCommand(const LinkedListAnimationCommand& command, LinkedListAnimationState& state, const LinkedListAnimationState& base_state) const;
	void applyCommandOnNode(LinkedListAnimationNode& node, const LinkedListAnimationCommand& command, const float& progress) const;
	void applyCommandOnEdge(LinkedListAnimationEdge& edge, const LinkedListAnimationCommand& command, const float& progress) const;
	LinkedListAnimationState initial_state;
	std::vector<LinkedListAnimationState> base_states; //The base state of each phase
	std::vector<float> start_time; //The start time of each phase, used to calculate the current animation state
	std::vector<LinkedListAnimationPhase> phases;
	float total_duration = 0.f;
	const int NODE_RADIUS = 50;
	const int NODE_GAP = 2 * NODE_RADIUS + 100;
	const int X_MARGIN = 400;
	const int Y_MARGIN = 400;
	const sf::Color DEFAULT_NODE_COLOR = sf::Color(89, 145, 46);
	const sf::Color DEFAULT_EDGE_COLOR = sf::Color(255, 255, 255);
	const sf::Color HIGHLIGHT_NODE_COLOR = sf::Color(209, 70, 38);
	int move_distance = NODE_GAP;
};