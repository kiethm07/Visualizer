#pragma once
#include <Animation/Trie/TrieRecorder.h>
#include <Animation/Trie/TrieAnimationState.h>
#include <Model/TrieModel/TrieState.h>
#include <vector>
#include <iostream>
#include <unordered_map>

class TrieAnimator {
public:
    void generateBaseStates(const TrieState& state, const TrieState& fin_state, const TrieRecorder& record);
    TrieAnimationState getStateAtTime(float t);
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
    float calculateSubtreeWidth(int u_idx, const TrieState& state, std::unordered_map<int, float>& subtree_width);
    void reconstructTree(int u_idx, const TrieState& state, float x, float y, std::unordered_map<int, float>& subtree_width, std::vector<TrieAnimationNode>& node_list, std::vector<TrieAnimationEdge>& edge_list);
    void normalizeEdgeLists(TrieAnimationState& animation_state);
    void generateAnimationState(TrieAnimationState& animation_state, const TrieState& state);
    void applyCommand(const TrieAnimationCommand& command, const TrieAnimationState& base_state, TrieAnimationState& state, const float& progress) const;
    void applySpawnCommand(const TrieAnimationCommand& command, TrieAnimationState& state, const TrieAnimationState& base_state) const;
    void applyCommandOnNode(TrieAnimationNode& node, const TrieAnimationCommand& command, const float& progress) const;
    void applyCommandOnEdge(TrieAnimationEdge& edge, const TrieAnimationCommand& comamnd, const float& progress) const;

    TrieAnimationState initial_state;
    std::vector<TrieAnimationState> base_states;
    std::vector<float> start_time;
    std::vector<TrieAnimationPhase> phases;
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