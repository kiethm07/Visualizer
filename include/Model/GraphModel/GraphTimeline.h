#pragma once
#include <Model/Timeline/Timeline.h>
#include <Model/GraphModel/GraphState.h>
#include <Model/GraphModel/GraphOperation.h>
#include <Model/GraphModel/GraphRenderer.h>
#include <Animation/Graph/GraphRecorder.h>
#include <Animation/Graph/GraphAnimator.h>
#include <Animation/Graph/GraphAnimationState.h>

using GraphTimeline = Timeline<
    GraphState,
    GraphOperation,
    GraphRecorder,
    GraphAnimator,
    GraphRenderer,
    GraphAnimationState
>;