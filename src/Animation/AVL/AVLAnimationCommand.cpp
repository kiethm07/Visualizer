#include <Animation/AVL/AVLAnimationCommand.h>

float getCommandDuration(const AVLAnimationType& type) {
	using Type = AVLAnimationType;
	if (type == Type::HighlightOn || type == Type::HighlightOff) {
		return 0.5f;
	}
	if (type == Type::FadeIn || type == Type::FadeOut) {
		return 0.5f;
	}
	if (type == Type::Wait) {
		return 1.f;
	}
	if (type == Type::Move) {
		return 1.f;
	}
	if (type == Type::Spawn) {
		return 0.0f;
	}
	if (type == Type::FoundedOn || type == Type::FoundedOff) {
		return 1.f;
	}
	if (type == Type::Reconstruct) {
		return 1.f;
	}
	if (type == Type::UpdateValue) {
		return 0.f;
	}
	return 1.f; //Default duration is 1 second
}