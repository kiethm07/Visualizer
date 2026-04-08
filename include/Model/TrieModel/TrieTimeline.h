#pragma once
#include <Model/Timeline/Timeline.h>
#include <Model/TrieModel/TrieState.h>
#include <Model/TrieModel/TrieOperation.h>
#include <Model/TrieModel/TrieRenderer.h>
#include <Animation/Trie/TrieRecorder.h>
#include <Animation/Trie/TrieAnimator.h>
#include <Animation/Trie/TrieAnimationState.h>

using TrieTimeline = Timeline<
    TrieState,
    TrieOperation,
    TrieRecorder,
    TrieAnimator,
    TrieRenderer,
    TrieAnimationState
>;