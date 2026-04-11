#pragma once
#include <vector>
#include <Animation/AVL/AVLAnimationCommand.h>

struct AVLAnimationPhase {
    std::vector<AVLAnimationCommand> commands;
};