#pragma once
#include <Model/Timeline/Timeline.h>
#include <Model/AVLModel/AVLState.h>
#include <Model/AVLModel/AVLOperation.h>
#include <Model/AVLModel/AVLRenderer.h>
#include <Animation/AVL/AVLRecorder.h>
#include <Animation/AVL/AVLAnimator.h>
#include <Animation/AVL/AVLAnimationState.h>

using AVLTimeline = Timeline<
    AVLState,
    AVLOperation,
    AVLRecorder,
    AVLAnimator,
    AVLRenderer,
    AVLAnimationState
>;