#pragma once
#include <Model/Timeline/Timeline.h>
#include <Model/HashmapModel/HashmapState.h>
#include <Model/HashmapModel/HashmapOperation.h>
#include <Model/HashmapModel/HashmapRenderer.h>
#include <Animation/Hashmap/HashmapRecorder.h>
#include <Animation/Hashmap/HashmapAnimator.h>
#include <Animation/Hashmap/HashmapAnimationState.h>

using HashmapTimeline = Timeline<
    HashmapState,
    HashmapOperation,
    HashmapRecorder,
    HashmapAnimator,
    HashmapRenderer,
    HashmapAnimationState
>;