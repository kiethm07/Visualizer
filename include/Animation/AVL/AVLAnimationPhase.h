#pragma once
#include <vector>
#include <optional>
#include <Animation/AVL/AVLAnimationCommand.h>
#include <Model/AVLModel/AVLState.h>

struct AVLAnimationPhase {
    std::vector<AVLAnimationCommand> commands;
    std::optional<AVLState> snapshot = std::nullopt; //Only used in "Reconstruct" operation
};