#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <Animation/LinkedList/LinkedListAnimationType.h>
#include <Animation/LinkedList/LinkedListMoveDirection.h>
#include <iostream>

float getCommandDuration(const LinkedListAnimationType& type);

struct LinkedListAnimationCommand {
	LinkedListAnimationCommand(LinkedListAnimationTarget target, LinkedListAnimationType type, int ui_id) :
		target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1) 
	{
		duration = getCommandDuration(type);
		if (type == LinkedListAnimationType::Move) {
			direction = LinkedListMoveDirection::Right; // default move direction is right
		}
		if (type == LinkedListAnimationType::FadeIn || type == LinkedListAnimationType::FadeOut) {
			direction = LinkedListMoveDirection::None; // no direction for fade in/out
		}
	}
	LinkedListAnimationCommand(LinkedListAnimationTarget target, LinkedListAnimationType type, int from_ui_id, int to_ui_id) :
		target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id) 
	{
		duration = getCommandDuration(type);
		if (type == LinkedListAnimationType::Move) {
			direction = LinkedListMoveDirection::Right; // default move direction is right
		}
		if (type == LinkedListAnimationType::FadeIn || type == LinkedListAnimationType::FadeOut) {
			direction = LinkedListMoveDirection::None; // no direction for fade in/out
		}
	}
	LinkedListAnimationCommand() :
		target(LinkedListAnimationTarget::Node), type(LinkedListAnimationType::Wait), ui_id(-1), from_ui_id(-1), to_ui_id(-1) {
		duration = getCommandDuration(type);
		direction = LinkedListMoveDirection::None;
	}
	LinkedListAnimationCommand(LinkedListAnimationTarget target, LinkedListAnimationType type, LinkedListMoveDirection direction, int ui_id):
		target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1), direction(direction) {
		duration = getCommandDuration(type);
	}
	LinkedListAnimationCommand(LinkedListAnimationTarget target, LinkedListAnimationType type, LinkedListMoveDirection direction, int from_ui_id, int to_ui_id):
		target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id), direction(direction) {
		duration = getCommandDuration(type);
	}
	static LinkedListAnimationCommand createSpawnNodeCommand(int ui_id, int spawn_from_ui_id, int value, sf::Vector2f spawn_offset = { 0, 100 }) {
		LinkedListAnimationCommand command;
		command.target = LinkedListAnimationTarget::Node;
		command.type = LinkedListAnimationType::Spawn;
		command.ui_id = ui_id;
		command.spawn_from_ui_id = spawn_from_ui_id;
		command.spawn_offset = spawn_offset;
		command.duration = getCommandDuration(command.type);
		command.value = value;
		return command;
	}
	static LinkedListAnimationCommand createSpawnEdgeCommand(int from_ui_id, int to_ui_id) {
		LinkedListAnimationCommand command;
		command.target = LinkedListAnimationTarget::Edge;
		command.type = LinkedListAnimationType::Spawn;
		command.from_ui_id = from_ui_id;
		command.to_ui_id = to_ui_id;
		command.duration = getCommandDuration(command.type);
		return command;
	}
	static LinkedListAnimationCommand createChangeValueCommand(int ui_id, int value) {
		LinkedListAnimationCommand command;
		command.target = LinkedListAnimationTarget::Node;
		command.type = LinkedListAnimationType::UpdateValue;
		command.ui_id = ui_id;
		command.value = value;
		command.duration = getCommandDuration(command.type);
		std::cout << command.duration << "\n";
		return command;
	}
	LinkedListAnimationTarget target;
	LinkedListAnimationType type;
	LinkedListMoveDirection direction; // for move and fade in/out, default is right for move, no direction for fade in/out
	sf::Vector2f spawn_offset = { 0, 100 }; // for spawn command, the offset from the original position to spawn position
	//Spawn commands must be handled first
	int ui_id; // for node
	int from_ui_id, to_ui_id; // for edge
	int spawn_from_ui_id;
	int value = 0; // for spawn node, the value to display on the node, default is 0


	float duration = 1.f; //Duration of this animation command, default is 1 second
};