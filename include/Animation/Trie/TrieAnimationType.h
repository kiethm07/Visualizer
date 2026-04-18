#pragma once
#include <vector>

enum class TrieAnimationType { Reconstruct, HighlightOn, HighlightOff, FadeIn, FadeOut, Wait, Move, Spawn, FoundedOn, FoundedOff, Remove , SetEndMark, UnsetEndMark};
enum class TrieAnimationTarget { Node, Edge, All };