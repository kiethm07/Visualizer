#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <Animation/Graph/GraphAnimationType.h>
#include <Animation/Graph/GraphMoveDirection.h>
#include <iostream>
float getCommandDuration(const GraphAnimationType& type);
struct GraphAnimationCommand
{
    GraphAnimationCommand(GraphAnimationTarget target, GraphAnimationType type, int ui_id)
        : target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1)
    {
        duration = getCommandDuration(type);
        if (type == GraphAnimationType::Move)
        {
            direction = GraphMoveDirection::Right;
        }
        if (type == GraphAnimationType::FadeIn || type == GraphAnimationType::FadeOut)
        {
            direction = GraphMoveDirection::None;
        }
    }
    GraphAnimationCommand(GraphAnimationTarget target, GraphAnimationType type, int from_ui_id, int to_ui_id)
        : target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id)
    {
        duration = getCommandDuration(type);
        if (type == GraphAnimationType::Move)
        {
            direction = GraphMoveDirection::Right;
        }
        if (type == GraphAnimationType::FadeIn || type == GraphAnimationType::FadeOut)
        {
            direction = GraphMoveDirection::None;
        }
    }
    GraphAnimationCommand()
        : target(GraphAnimationTarget::Node), type(GraphAnimationType::Wait), ui_id(-1), from_ui_id(-1), to_ui_id(-1)
    {
        duration = getCommandDuration(type);
        direction = GraphMoveDirection::None;
    }
    GraphAnimationCommand(GraphAnimationTarget target, GraphAnimationType type, GraphMoveDirection direction, int ui_id)
        : target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1), direction(direction)
    {
        duration = getCommandDuration(type);
    }
    GraphAnimationCommand(GraphAnimationTarget target, GraphAnimationType type, GraphMoveDirection direction, int from_ui_id, int to_ui_id)
        : target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id), direction(direction)
    {
        duration = getCommandDuration(type);
    }
    static GraphAnimationCommand createSpawnNodeCommand(int ui_id, int spawn_from_ui_id, int value, sf::Vector2f spawn_offset = { 0, 100 })
    {
        GraphAnimationCommand command;
        command.target = GraphAnimationTarget::Node;
        command.type = GraphAnimationType::Spawn;
        command.ui_id = ui_id;
        command.spawn_from_ui_id = spawn_from_ui_id;
        command.spawn_offset = spawn_offset;
        command.duration = getCommandDuration(command.type);
        command.value = value;
        return command;
    }
    static GraphAnimationCommand createSpawnEdgeCommand(int from_ui_id, int to_ui_id)
    {
        GraphAnimationCommand command;
        command.target = GraphAnimationTarget::Edge;
        command.type = GraphAnimationType::Spawn;
        command.from_ui_id = from_ui_id;
        command.to_ui_id = to_ui_id;
        command.duration = getCommandDuration(command.type);
        return command;
    }
    static GraphAnimationCommand createChangeValueCommand(int ui_id, int value) {
		GraphAnimationCommand command;
		command.target = GraphAnimationTarget::Node;
		command.type = GraphAnimationType::UpdateValue;
        command.value = value;
		command.ui_id = ui_id;
        command.duration = getCommandDuration(command.type);
        return command;
    }
    static GraphAnimationCommand createWaitCommand() {
        GraphAnimationCommand command;
        command.target = GraphAnimationTarget::Node;
        command.type = GraphAnimationType::Wait;
        command.ui_id = -1;
        command.duration = getCommandDuration(command.type);
		return command;
    }
    GraphAnimationTarget target;
    GraphAnimationType type;
    GraphMoveDirection direction;
    sf::Vector2f spawn_offset = { 0, 100 };
    int ui_id;
    int from_ui_id, to_ui_id;
    int spawn_from_ui_id;
    int value = 0;
    float duration = 1.f;
};