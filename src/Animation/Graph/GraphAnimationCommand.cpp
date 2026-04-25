#include <Animation/Graph/GraphAnimationCommand.h>

float getCommandDuration(const GraphAnimationType& type) {
	using Type = GraphAnimationType;
	if (type == Type::HighlightOn || type == Type::HighlightOff) {
		return 0.5f;
	}
	if (type == Type::FadeIn || type == Type::FadeOut) {
		return 0.5f;
	}
	if (type == Type::Wait) {
		return 0.5f;
	}
	if (type == Type::Spawn) {
		return 0.0f;
	}
	if (type == Type::UpdateValue) {
		return 1.f;
	}
	if (type == Type::InSPG) {
		return 1.5f;
	}
	return 1.f; //Default duration is 1 second
}