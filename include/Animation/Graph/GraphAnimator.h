#pragma once
#include <Animation/Graph/GraphRecorder.h>
#include <Animation/Graph/GraphAnimationState.h>
#include <Model/GraphModel/GraphState.h>
#include <vector>
#include <iostream>
#include <unordered_map>

class GraphAnimator {
public:
    void generateBaseStates(const GraphState& state, const GraphState& fin_state, const GraphRecorder& record);
    int getHighlightedLine(float t) const;
    GraphAnimationState getStateAtTime(float t);
    float getTotalDuration() const {
        return total_duration;
    }
    float getNextPhaseTimer(float t) const;
    float getPreviousPhaseTimer(float t) const;
    float getLeftBound(float t) const;
    float getRightBound(float t) const;
    bool isPhaseBoundary(float t) const;
    void clear();

private:
    void normalizeEdgeLists(GraphAnimationState& animation_state) const;
    void generateAnimationState(GraphAnimationState& animation_state, const GraphState& state) const;
    void applyCommand(const GraphAnimationCommand& command, const GraphAnimationState& base_state, GraphAnimationState& state, const std::optional<GraphState>& snapshot, const float& progress) const;
    void applySpawnCommand(const GraphAnimationCommand& command, GraphAnimationState& state, const GraphAnimationState& base_state) const;
    void applyCommandOnNode(GraphAnimationNode& node, const GraphAnimationCommand& command, const float& progress) const;
    void applyCommandOnEdge(GraphAnimationEdge& edge, const GraphAnimationCommand& comamnd, const float& progress) const;
    void applyCommandOnPopup(GraphAnimationPopup& pop_up, const GraphAnimationCommand& comamnd, const float& progress) const;
    GraphAnimationState initial_state;
    std::vector<GraphAnimationState> base_states;
    std::vector<float> start_time;
    std::vector<GraphAnimationPhase> phases;
    std::vector<int> highlighted_lines;
    float total_duration = 0.f;

    const int NODE_RADIUS = 50;
    const int LEVEL_GAP = 200;
    const int X_MARGIN = 800;
    const int Y_MARGIN = 100;
    const int move_distance = 200;

    const sf::Color DEFAULT_NODE_COLOR = sf::Color(89, 145, 46);
    const sf::Color DEFAULT_EDGE_COLOR = sf::Color(255, 255, 255);
    const sf::Color HIGHLIGHT_EDGE_COLOR = sf::Color::Red;
    const sf::Color HIGHLIGHT_NODE_COLOR = sf::Color(209, 70, 38);
    const sf::Color FOUNDED_NODE_COLOR = sf::Color::Blue;
    const sf::Color FOUNDED_EDGE_COLOR = sf::Color::Green;
};