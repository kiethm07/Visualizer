#include <Animation/LinkedList/LinkedListAnimationCommand.h>

float getCommandDuration(const LinkedListAnimationType& type) {
	using Type = LinkedListAnimationType;
	if (type == Type::HighlightOn || type == Type::HighlightOff) {
		return 0.5f;
	}
	if (type == Type::FadeIn || type == Type::FadeOut) {
		return 0.5f;
	}
	if (type == Type::Wait) {
		return 0.5f;
	}
	if (type == Type::Move) {
		return 1.f;
	}
	if (type == Type::Spawn) {
		return 0.0f;
	}
	return 1.f; //Default duration is 1 second
}