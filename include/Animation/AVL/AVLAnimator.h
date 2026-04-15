#pragma once
#include <Animation/AVL/AVLRecorder.h>
#include <Animation/AVL/AVLAnimationState.h>
#include <Model/AVLModel/AVLState.h>
#include <vector>
#include <iostream>
#include <unordered_map>

class AVLAnimator {
public:
    void generateBaseStates(const AVLState& state, const AVLState& fin_state, const AVLRecorder& record);
    AVLAnimationState getStateAtTime(float t);
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
    float calculateSubtreeWidth(int u_idx, const AVLState& state, std::unordered_map<int, float>& subtree_width) const;
    void reconstructTree(int u_idx, const AVLState& state, float x, float y, std::unordered_map<int, float>& subtree_width, std::vector<AVLAnimationNode>& node_list, std::vector<AVLAnimationEdge>& edge_list) const;
    void normalizeEdgeLists(AVLAnimationState& animation_state) const;
    void generateAnimationState(AVLAnimationState& animation_state, const AVLState& state) const;
    void applyCommand(const AVLAnimationCommand& command, const AVLAnimationState& base_state, AVLAnimationState& state, const std::optional<AVLState>& snapshot, const float& progress) const;
    void applySpawnCommand(const AVLAnimationCommand& command, AVLAnimationState& state, const AVLAnimationState& base_state) const;
    void applyCommandOnNode(AVLAnimationNode& node, const AVLAnimationCommand& command, const float& progress) const;
    void applyCommandOnEdge(AVLAnimationEdge& edge, const AVLAnimationCommand& comamnd, const float& progress) const;

    AVLAnimationState initial_state;
    std::vector<AVLAnimationState> base_states;
    std::vector<float> start_time;
    std::vector<AVLAnimationPhase> phases;
    float total_duration = 0.f;

    const int NODE_RADIUS = 50;
    const int LEVEL_GAP = 200;
    const int X_MARGIN = 800;
    const int Y_MARGIN = 100;
    const int move_distance = 200;

    const sf::Color DEFAULT_NODE_COLOR = sf::Color(89, 145, 46);
    const sf::Color DEFAULT_EDGE_COLOR = sf::Color(255, 255, 255);
    const sf::Color HIGHLIGHT_NODE_COLOR = sf::Color(209, 70, 38);
    const sf::Color FOUNDED_NODE_COLOR = sf::Color::Blue;
    const sf::Color END_NODE_COLOR = sf::Color(255, 165, 0);
};