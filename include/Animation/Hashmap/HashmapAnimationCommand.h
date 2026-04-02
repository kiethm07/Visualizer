#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <Animation/Hashmap/HashmapAnimationType.h>
#include <Animation/Hashmap/HashmapMoveDirection.h>
#include <iostream>

float getCommandDuration(const HashmapAnimationType& type);

struct HashmapAnimationCommand {
	HashmapAnimationCommand(HashmapAnimationTarget target, HashmapAnimationType type, int ui_id) :
		target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1)
	{
		duration = getCommandDuration(type);
		if (type == HashmapAnimationType::Move) {
			direction = HashmapMoveDirection::Right;
		}
		if (type == HashmapAnimationType::FadeIn || type == HashmapAnimationType::FadeOut) {
			direction = HashmapMoveDirection::None;
		}
	}
	HashmapAnimationCommand(HashmapAnimationTarget target, HashmapAnimationType type, int from_ui_id, int to_ui_id) :
		target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id)
	{
		duration = getCommandDuration(type);
		if (type == HashmapAnimationType::Move) {
			direction = HashmapMoveDirection::Right;
		}
		if (type == HashmapAnimationType::FadeIn || type == HashmapAnimationType::FadeOut) {
			direction = HashmapMoveDirection::None;
		}
	}
	HashmapAnimationCommand() :
		target(HashmapAnimationTarget::Node), type(HashmapAnimationType::Wait), ui_id(-1), from_ui_id(-1), to_ui_id(-1) {
		duration = getCommandDuration(type);
		direction = HashmapMoveDirection::None;
	}
	HashmapAnimationCommand(HashmapAnimationTarget target, HashmapAnimationType type, HashmapMoveDirection direction, int ui_id) :
		target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1), direction(direction) {
		duration = getCommandDuration(type);
	}
	HashmapAnimationCommand(HashmapAnimationTarget target, HashmapAnimationType type, HashmapMoveDirection direction, int from_ui_id, int to_ui_id) :
		target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id), direction(direction) {
		duration = getCommandDuration(type);
	}
	static HashmapAnimationCommand createSpawnNodeCommand(int ui_id, int spawn_from_ui_id, int value, sf::Vector2f spawn_offset = { 0, 100 }) {
		HashmapAnimationCommand command;
		command.target = HashmapAnimationTarget::Node;
		command.type = HashmapAnimationType::Spawn;
		command.ui_id = ui_id;
		command.spawn_from_ui_id = spawn_from_ui_id;
		command.spawn_offset = spawn_offset;
		command.duration = getCommandDuration(command.type);
		command.value = value;
		return command;
	}
	static HashmapAnimationCommand createSpawnEdgeCommand(int from_ui_id, int to_ui_id) {
		HashmapAnimationCommand command;
		command.target = HashmapAnimationTarget::Edge;
		command.type = HashmapAnimationType::Spawn;
		command.from_ui_id = from_ui_id;
		command.to_ui_id = to_ui_id;
		command.duration = getCommandDuration(command.type);
		return command;
	}
	HashmapAnimationTarget target;
	HashmapAnimationType type;
	HashmapMoveDirection direction;
	sf::Vector2f spawn_offset = { 0, 100 };
	int ui_id;
	int from_ui_id, to_ui_id;
	int spawn_from_ui_id;
	int value = 0;
	float duration = 1.f;
};