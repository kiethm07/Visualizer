#pragma once
#include <vector>
#include <Animation/LinkedList/LinkedListAnimationType.h>
#include <Animation/LinkedList/LinkedListMoveDirection.h>

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
	LinkedListAnimationTarget target;
	LinkedListAnimationType type;
	LinkedListMoveDirection direction; // for move and fade in/out, default is right for move, no direction for fade in/out
	int ui_id; // for node
	int from_ui_id, to_ui_id; // for edge

	float duration = 1.f; //Duration of this animation command, default is 1 second
};