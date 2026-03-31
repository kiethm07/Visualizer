#pragma once
#include <AssetManager/AssetManager.h>
#include <Model/Timeline/TimelineController.h>
#include <Model/Timeline/TimelineData.h>
#include <algorithm>

template <
    typename State,
    typename Operation,
    typename Record,
    typename Animator,
    typename Renderer,
    typename AnimationState
>
class Timeline {
public:
    Timeline(const AssetManager& a_manager)
        : renderer(a_manager) {
    }
    bool isRunning() const {
        return controller.isRunning();
    }
    bool isAutoPlaying() const {
        return controller.isAutoPlaying();
    }
    void setAutoPlay(bool auto_play) {
        controller.setAutoPlay(auto_play);
    }
    void setSpeed(float new_speed) {
        controller.setSpeed(new_speed);
    }
    void setDirection(int dir) {
        controller.setDirection(dir);
    }
    void push(const State& current_state, const Operation& operation, const Record& record) {
        data.push(current_state, operation, record);
    }
    void run() {
        controller.run();
    }
    void pause() {
        controller.pause();
    }
    void clear() {
        controller.reset();
        data.clear();
        animator.clear();
    }
    void generateAnimation(const State& initial_state, const Record& record) {
        animator.generateBaseStates(initial_state, record);
    }
    void update(const float& real_delta_time) {
        if (!controller.isRunning()) return;

        float dt = real_delta_time * controller.speed;
        float new_time = controller.current_time + dt * controller.direction;

        if (!controller.isAutoPlaying()) {
            if (animator.isPhaseBoundary(controller.current_time)) {
                if (!controller.next_phase_waiting) new_time = controller.current_time;
                controller.next_phase_waiting = false;
            }
            else {
                float left_bound = animator.getPreviousPhaseTimer(controller.current_time);
                float right_bound = animator.getNextPhaseTimer(controller.current_time);

                if (left_bound == -1) left_bound = 0;
                if (right_bound == -1) right_bound = animator.getTotalDuration();

                if (new_time > right_bound) new_time = right_bound;
                if (new_time < left_bound) new_time = left_bound;
            }
        }

        controller.current_time = new_time;

        if (controller.current_time > animator.getTotalDuration() &&
            controller.current_operation_index == static_cast<int>(data.size())) {
            controller.current_time = animator.getTotalDuration();
        }
        else if (controller.current_time > animator.getTotalDuration()) {
            controller.current_time -= animator.getTotalDuration();

            while (controller.current_operation_index < static_cast<int>(data.size())) {
                animator.generateBaseStates(
                    data.getState(controller.current_operation_index),
                    data.getRecord(controller.current_operation_index)
                );
                controller.current_operation_index++;

                if (controller.current_time < animator.getTotalDuration() ||
                    controller.current_operation_index == static_cast<int>(data.size())) {
                    break;
                }

                controller.current_time -= animator.getTotalDuration();
            }

            if (controller.current_operation_index == static_cast<int>(data.size())) {
                controller.current_time = std::min(controller.current_time, animator.getTotalDuration());
            }
        }
        else if (controller.current_time < 0 && controller.current_operation_index == 0) {
            controller.current_time = 0;
            animator.clear();
        }
        else if (controller.current_time < 0) {
            while (controller.current_operation_index > 0) {
                controller.current_operation_index--;

                if (controller.current_operation_index == 0) break;

                animator.generateBaseStates(
                    data.getState(controller.current_operation_index - 1),
                    data.getRecord(controller.current_operation_index - 1)
                );

                controller.current_time += animator.getTotalDuration();

                if (controller.current_time > 0 || controller.current_operation_index == 0) {
                    break;
                }
            }

            if (controller.current_operation_index == 0) {
                controller.current_time = 0;
                animator.clear();
            }
        }

        current_animation_state = animator.getStateAtTime(controller.current_time);
    }

    void onePhaseForward() {
        if (!controller.isRunning()) return;

        controller.direction = 1;

        if (animator.isPhaseBoundary(controller.current_time)) {
            controller.next_phase_waiting = true;
            return;
        }

        float next_phase_timer = animator.getNextPhaseTimer(controller.current_time);

        if (next_phase_timer == -1) {
            if (controller.current_operation_index == static_cast<int>(data.size())) {
                next_phase_timer = animator.getTotalDuration();
            }
            else {
                generateAnimation(
                    data.getState(controller.current_operation_index),
                    data.getRecord(controller.current_operation_index)
                );
                controller.current_operation_index++;
                next_phase_timer = 0;
            }

            controller.current_time = next_phase_timer;
        }
        else {
            controller.current_time = next_phase_timer;
        }
    }

    void onePhaseBackward() {
        if (!controller.isRunning()) return;

        controller.direction = -1;

        if (animator.isPhaseBoundary(controller.current_time)) {
            controller.next_phase_waiting = true;
            return;
        }

        float prev_phase_timer = animator.getPreviousPhaseTimer(controller.current_time);

        if (prev_phase_timer == -1) {
            if (controller.current_operation_index == 0) {
                prev_phase_timer = 0;
                animator.clear();
            }
            else {
                controller.current_operation_index--;
                generateAnimation(
                    data.getState(controller.current_operation_index),
                    data.getRecord(controller.current_operation_index)
                );
                prev_phase_timer = animator.getTotalDuration();
            }

            controller.current_time = prev_phase_timer;
        }
        else {
            controller.current_time = prev_phase_timer;
        }
    }

    void oneStepForward() {
        if (!controller.isRunning()) return;

        controller.direction = 1;
        controller.next_phase_waiting = false;

        if (controller.current_operation_index == static_cast<int>(data.size())) {
            controller.current_time = animator.getTotalDuration();
            return;
        }

        if (controller.current_time < animator.getTotalDuration()) {
            controller.current_time = animator.getTotalDuration();
        }
        else {
            generateAnimation(
                data.getState(controller.current_operation_index),
                data.getRecord(controller.current_operation_index)
            );
            controller.current_operation_index++;
            controller.current_time = animator.getTotalDuration();
        }
    }

    void oneStepBackward() {
        if (!controller.isRunning()) return;

        controller.direction = -1;
        controller.next_phase_waiting = false;

        if (controller.current_operation_index == 0) {
            controller.current_time = 0.f;
            animator.clear();
            return;
        }

        if (controller.current_time > 0.f) {
            controller.current_time = 0.f;
        }
        else {
            controller.current_operation_index--;

            if (controller.current_operation_index > 0) {
                generateAnimation(
                    data.getState(controller.current_operation_index - 1),
                    data.getRecord(controller.current_operation_index - 1)
                );
            }
            else {
                animator.clear();
            }

            controller.current_time = 0.f;
        }
    }

    void toLast() {
        if (!controller.isRunning()) return;
        if (data.empty()) return;

        controller.direction = 1;
        generateAnimation(
            data.getState(data.size() - 1),
            data.getRecord(data.size() - 1)
        );
        controller.current_operation_index = static_cast<int>(data.size());
        controller.current_time = animator.getTotalDuration();
    }

    void toInit() {
        if (!controller.isRunning()) return;

        controller.direction = -1;
        animator.clear();
        controller.current_operation_index = 0;
        controller.current_time = 0.f;
    }

    void draw(sf::RenderWindow& window, const sf::View& view) {
        renderer.loadState(current_animation_state);
        renderer.draw(window, view);
    }

private:
    TimelineController controller;
    TimelineData<State, Operation, Record> data;

    Animator animator;
    Renderer renderer;
    AnimationState current_animation_state;
};