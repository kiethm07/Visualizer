#pragma once
#include <SFML/Graphics.hpp>
#include <Animation/Graph/GraphAnimationState.h>
#include <map>
#include <set>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <random>

struct PhysicsNode {
    sf::Vector2f position;
    sf::Vector2f velocity;

    PhysicsNode():
        position(0.f, 0.f), velocity(0.f, 0.f) {
    }
    PhysicsNode(sf::Vector2f pos):
        position(pos), velocity(0.f, 0.f) {
	}
};

class GraphPhysics {
public:
    GraphPhysics() {}

    void setCenter(sf::Vector2f center);
    void update(const GraphAnimationState& anim_state, float dt);
    sf::Vector2f getPosition(int ui_id);

    void setDraggedNode(int ui_id) { dragged_node = ui_id; }
    void setDragOffset(sf::Vector2f offset) { drag_offset = offset; }
    void updateMousePos(sf::Vector2f pos) { current_mouse_pos = pos; }
    void releaseNode() { dragged_node = -1; }
private:
    std::map<int, PhysicsNode> physics_nodes;
    int dragged_node = -1;
    sf::Vector2f drag_offset;
    sf::Vector2f current_mouse_pos;

    // Physics parameters
    float k_repel = 400000.f;
    float k_spring = 0.1f;
    float ideal_length = 200.f;
    float damping = 0.80f;
    float gravity = 0.05f;

    sf::Vector2f center_pos = sf::Vector2f(600.f, 400.f);
};