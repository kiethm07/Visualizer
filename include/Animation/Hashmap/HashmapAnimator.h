#pragma once
#include <Animation/Hashmap/HashmapRecorder.h>
#include <Animation/Hashmap/HashmapAnimationState.h>
#include <Model/HashmapModel/HashmapState.h>
#include <vector>
#include <iostream>

class HashmapAnimator {
public:
	void generateBaseStates(const HashmapState& initial_state, const HashmapState& fin_state, const HashmapRecorder& record);
	HashmapAnimationState getStateAtTime(float t); //Get the state of the animation at time t, used to draw the animation
	int getHighlightedLine(float t) const;
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
	void normalizeEdgeLists(HashmapAnimationState& animation_state);
	void generateAnimationState(HashmapAnimationState& animation_state, const HashmapState& state);
	void applyCommand(const HashmapAnimationCommand& command, const HashmapAnimationState& base_state, HashmapAnimationState& state, const float& progress) const;
	void applySpawnCommand(const HashmapAnimationCommand& command, HashmapAnimationState& state, const HashmapAnimationState& base_state) const;
	void applyCommandOnNode(HashmapAnimationNode& node, const HashmapAnimationCommand& command, const float& progress) const;
	void applyCommandOnEdge(HashmapAnimationEdge& edge, const HashmapAnimationCommand& command, const float& progress) const;
	void applyCommandOnBucket(HashmapAnimationBucket& bucket, const HashmapAnimationCommand& command, const float& progress) const;
	HashmapAnimationState initial_state;
	std::vector<HashmapAnimationState> base_states; //The base state of each phase
	std::vector<float> start_time; //The start time of each phase, used to calculate the current animation state
	std::vector<HashmapAnimationPhase> phases;
	std::vector<int> highlighted_lines;
	float total_duration = 0.f;
	const int NODE_RADIUS = 50;
	const int NODE_GAP = 2 * NODE_RADIUS + 100;
	const int BUCKET_WIDTH = 100;
	const int BUCKET_HEIGHT = 100;
	const int BUCKET_GAP = BUCKET_HEIGHT + 20;
	const int X_MARGIN = 400;
	const int Y_MARGIN = 200;
	const sf::Color DEFAULT_NODE_COLOR = sf::Color(89, 145, 46);
	const sf::Color DEFAULT_EDGE_COLOR = sf::Color(255, 255, 255);
	const sf::Color HIGHLIGHT_NODE_COLOR = sf::Color(209, 70, 38);
	const sf::Color FOUNDED_NODE_COLOR = sf::Color::Blue;
	const sf::Color DEFAULT_BUCKET_COLOR = sf::Color(128, 128, 128);
	const sf::Color HIGHLIGHT_BUCKET_COLOR = sf::Color(255, 255, 0);
	int move_distance = NODE_GAP;
};