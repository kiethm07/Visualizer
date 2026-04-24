#include <Model/GraphModel/GraphPhysics.h>

void GraphPhysics::setCenter(sf::Vector2f center) {
    center_pos = center;
}

void GraphPhysics::update(const GraphAnimationState& anim_state, float dt) {
    std::set<int> current_ids;
    const auto& nodes = anim_state.getNodeList();
    const auto& edges = anim_state.getEdgeList();

    // 1. Sync active nodes and init positions
    for (const auto& node : nodes) {
        int id = node.ui_id;
        current_ids.insert(id);
        if (physics_nodes.find(id) == physics_nodes.end()) {
            float rx = center_pos.x + (std::rand() % 100 - 50);
            float ry = center_pos.y + (std::rand() % 100 - 50);
            physics_nodes[id] = PhysicsNode(sf::Vector2f(rx, ry));
        }
    }

    // 2. Remove deleted nodes
    for (auto it = physics_nodes.begin(); it != physics_nodes.end(); ) {
        if (current_ids.find(it->first) == current_ids.end()) {
            it = physics_nodes.erase(it);
        }
        else {
            ++it;
        }
    }

    std::map<int, sf::Vector2f> forces;

    // 3. Repulsion force (Coulomb)
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].disable_physics) continue;
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            if (nodes[j].disable_physics) continue;

            int u = nodes[i].ui_id;
            int v = nodes[j].ui_id;
            sf::Vector2f delta = physics_nodes[u].position - physics_nodes[v].position;
            float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

            if (dist > 0.1f) {
                sf::Vector2f repel = (delta / dist) * (k_repel / (dist * dist));
                forces[u] += repel;
                forces[v] -= repel;
            }
        }
    }

    // 4. Attraction force (Hooke's spring)
    for (const auto& edge : edges) {
        if (edge.disable_physics) continue;

        int u = edge.from_ui_id;
        int v = edge.to_ui_id;

        if (physics_nodes.find(u) == physics_nodes.end() || physics_nodes.find(v) == physics_nodes.end()) {
            continue;
        }

        sf::Vector2f delta = physics_nodes[v].position - physics_nodes[u].position;
        float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (dist > 0.1f) {
            float mag = k_spring * (dist - ideal_length);
            sf::Vector2f spring = (delta / dist) * mag;
            forces[u] += spring;
            forces[v] -= spring;
        }
    }

    // 5. Apply gravity, damping, and integrate velocity to position
    for (const auto& node : nodes) {
        int id = node.ui_id;
        sf::Vector2f grav = (center_pos - physics_nodes[id].position) * gravity;
        forces[id] += grav;

        physics_nodes[id].velocity = (physics_nodes[id].velocity + forces[id]) * damping;
        physics_nodes[id].position += physics_nodes[id].velocity * dt;
    }
}

sf::Vector2f GraphPhysics::getPosition(int ui_id) {
    if (physics_nodes.find(ui_id) != physics_nodes.end()) {
        return physics_nodes[ui_id].position;
    }
    return center_pos;
}