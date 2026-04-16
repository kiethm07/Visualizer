#include <Animation/Trie/TrieAnimationCommand.h>

float getCommandDuration(const TrieAnimationType& type) {
	using Type = TrieAnimationType;
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
	if (type == Type::FoundedOn || type == Type::FoundedOff) {
		return 0.75f;
	}
	if (type == Type::Reconstruct) {
		return 0.75f;
	}
	if (type == Type::SetEndMark) {
		return 0.5f;
	}
	return 1.f; //Default duration is 1 second
}