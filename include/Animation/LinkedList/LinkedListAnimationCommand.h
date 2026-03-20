#pragma once
#include <vector>
#include <Animation/LinkedList/LinkedListAnimationType.h>

struct LinkedListAnimationCommand {
	LinkedListAnimationCommand(LinkedListAnimationTarget target, LinkedListAnimationType type, int ui_id) :
		target(target), type(type), ui_id(ui_id), from_ui_id(-1), to_ui_id(-1) {
	}
	LinkedListAnimationCommand(LinkedListAnimationTarget target, LinkedListAnimationType type, int from_ui_id, int to_ui_id) :
		target(target), type(type), ui_id(-1), from_ui_id(from_ui_id), to_ui_id(to_ui_id) {
	}
	LinkedListAnimationTarget target;
	LinkedListAnimationType type;
	int ui_id; // for node
	int from_ui_id, to_ui_id; // for edge
};