#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <Animation/Trie/TrieAnimationType.h>
#include <Animation/Trie/TrieMoveDirection.h>
#include <iostream>
float getCommandDuration(const TrieAnimationType& type);
struct TrieAnimationCommand
{
    TrieAnimationCommand(TrieAnimationTarget target, TrieAnimationType type, int ui_id)
        : target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1)
    {
        duration = getCommandDuration(type);
        if (type == TrieAnimationType::Move)
        {
            direction = TrieMoveDirection::Right;
        }
        if (type == TrieAnimationType::FadeIn || type == TrieAnimationType::FadeOut)
        {
            direction = TrieMoveDirection::None;
        }
    }
    TrieAnimationCommand(TrieAnimationTarget target, TrieAnimationType type, int from_ui_id, int to_ui_id)
        : target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id)
    {
        duration = getCommandDuration(type);
        if (type == TrieAnimationType::Move)
        {
            direction = TrieMoveDirection::Right;
        }
        if (type == TrieAnimationType::FadeIn || type == TrieAnimationType::FadeOut)
        {
            direction = TrieMoveDirection::None;
        }
    }
    TrieAnimationCommand()
        : target(TrieAnimationTarget::Node), type(TrieAnimationType::Wait), ui_id(-1), from_ui_id(-1), to_ui_id(-1)
    {
        duration = getCommandDuration(type);
        direction = TrieMoveDirection::None;
    }
    TrieAnimationCommand(TrieAnimationTarget target, TrieAnimationType type, TrieMoveDirection direction, int ui_id)
        : target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1), direction(direction)
    {
        duration = getCommandDuration(type);
    }
    TrieAnimationCommand(TrieAnimationTarget target, TrieAnimationType type, TrieMoveDirection direction, int from_ui_id, int to_ui_id)
        : target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id), direction(direction)
    {
        duration = getCommandDuration(type);
    }
    static TrieAnimationCommand createSpawnNodeCommand(int ui_id, int spawn_from_ui_id, char value, sf::Vector2f spawn_offset = { 0, 100 })
    {
        TrieAnimationCommand command;
        command.target = TrieAnimationTarget::Node;
        command.type = TrieAnimationType::Spawn;
        command.ui_id = ui_id;
        command.spawn_from_ui_id = spawn_from_ui_id;
        command.spawn_offset = spawn_offset;
        command.duration = getCommandDuration(command.type);
        command.value = value;
        return command;
    }
    static TrieAnimationCommand createSpawnEdgeCommand(int from_ui_id, int to_ui_id)
    {
        TrieAnimationCommand command;
        command.target = TrieAnimationTarget::Edge;
        command.type = TrieAnimationType::Spawn;
        command.from_ui_id = from_ui_id;
        command.to_ui_id = to_ui_id;
        command.duration = getCommandDuration(command.type);
        return command;
    }
    TrieAnimationTarget target;
    TrieAnimationType type;
    TrieMoveDirection direction;
    sf::Vector2f spawn_offset = { 0, 100 };
    int ui_id;
    int from_ui_id, to_ui_id;
    int spawn_from_ui_id;
    char value = ' ';
    float duration = 1.f;
};