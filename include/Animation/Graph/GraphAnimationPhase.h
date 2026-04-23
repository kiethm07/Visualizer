#pragma once
#include <vector>
#include <optional>
#include <Animation/Graph/GraphAnimationCommand.h>
#include <Model/GraphModel/GraphState.h>

struct GraphAnimationPhase {
    std::vector<GraphAnimationCommand> commands;
    std::optional<GraphState> snapshot = std::nullopt; //Only used in "Reconstruct" operation
};