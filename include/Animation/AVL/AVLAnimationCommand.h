#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <Animation/AVL/AVLAnimationType.h>
#include <Animation/AVL/AVLMoveDirection.h>
#include <iostream>
float getCommandDuration(const AVLAnimationType& type);
struct AVLAnimationCommand
{
    AVLAnimationCommand(AVLAnimationTarget target, AVLAnimationType type, int ui_id)
        : target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1)
    {
        duration = getCommandDuration(type);
        if (type == AVLAnimationType::Move)
        {
            direction = AVLMoveDirection::Right;
        }
        if (type == AVLAnimationType::FadeIn || type == AVLAnimationType::FadeOut)
        {
            direction = AVLMoveDirection::None;
        }
    }
    AVLAnimationCommand(AVLAnimationTarget target, AVLAnimationType type, int from_ui_id, int to_ui_id)
        : target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id)
    {
        duration = getCommandDuration(type);
        if (type == AVLAnimationType::Move)
        {
            direction = AVLMoveDirection::Right;
        }
        if (type == AVLAnimationType::FadeIn || type == AVLAnimationType::FadeOut)
        {
            direction = AVLMoveDirection::None;
        }
    }
    AVLAnimationCommand()
        : target(AVLAnimationTarget::Node), type(AVLAnimationType::Wait), ui_id(-1), from_ui_id(-1), to_ui_id(-1)
    {
        duration = getCommandDuration(type);
        direction = AVLMoveDirection::None;
    }
    AVLAnimationCommand(AVLAnimationTarget target, AVLAnimationType type, AVLMoveDirection direction, int ui_id)
        : target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1), direction(direction)
    {
        duration = getCommandDuration(type);
    }
    AVLAnimationCommand(AVLAnimationTarget target, AVLAnimationType type, AVLMoveDirection direction, int from_ui_id, int to_ui_id)
        : target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id), direction(direction)
    {
        duration = getCommandDuration(type);
    }
    static AVLAnimationCommand createSpawnNodeCommand(int ui_id, int spawn_from_ui_id, int value, sf::Vector2f spawn_offset = { 0, 100 })
    {
        AVLAnimationCommand command;
        command.target = AVLAnimationTarget::Node;
        command.type = AVLAnimationType::Spawn;
        command.ui_id = ui_id;
        command.spawn_from_ui_id = spawn_from_ui_id;
        command.spawn_offset = spawn_offset;
        command.duration = getCommandDuration(command.type);
        command.value = value;
        return command;
    }
    static AVLAnimationCommand createSpawnEdgeCommand(int from_ui_id, int to_ui_id)
    {
        AVLAnimationCommand command;
        command.target = AVLAnimationTarget::Edge;
        command.type = AVLAnimationType::Spawn;
        command.from_ui_id = from_ui_id;
        command.to_ui_id = to_ui_id;
        command.duration = getCommandDuration(command.type);
        return command;
    }
    static AVLAnimationCommand createChangeValueCommand(int ui_id, int value) {
		AVLAnimationCommand command;
		command.target = AVLAnimationTarget::Node;
		command.type = AVLAnimationType::UpdateValue;
        command.value = value;
		command.ui_id = ui_id;
        command.duration = getCommandDuration(command.type);
        return command;
    }
    AVLAnimationTarget target;
    AVLAnimationType type;
    AVLMoveDirection direction;
    sf::Vector2f spawn_offset = { 0, 100 };
    int ui_id;
    int from_ui_id, to_ui_id;
    int spawn_from_ui_id;
    int value = 0;
    float duration = 1.f;
};