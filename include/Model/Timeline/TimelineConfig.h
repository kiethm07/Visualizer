#pragma once

enum class TimelineOperation{ ChangeSpeed, Play, OnePhaseForward, OnePhaseBackward, OneStepForward, OneStepBackward, LastState, InitState, AutoPlay, None };

struct TimelineConfig {
    TimelineOperation type = TimelineOperation::None;
    float speed = 0.f;

    static TimelineConfig autoPlay() {
        TimelineConfig cfg;
        cfg.type = TimelineOperation::AutoPlay;
        return cfg;
    }

    static TimelineConfig play() {
        TimelineConfig cfg;
        cfg.type = TimelineOperation::Play;
        return cfg;
    }

    static TimelineConfig onePhaseForward() {
        TimelineConfig cfg;
        cfg.type = TimelineOperation::OnePhaseForward;
        return cfg;
    }

    static TimelineConfig onePhaseBackward() {
        TimelineConfig cfg;
        cfg.type = TimelineOperation::OnePhaseBackward;
        return cfg;
    }

    static TimelineConfig oneStepForward() {
        TimelineConfig cfg;
        cfg.type = TimelineOperation::OneStepForward;
        return cfg;
    }

    static TimelineConfig oneStepBackward() {
        TimelineConfig cfg;
        cfg.type = TimelineOperation::OneStepBackward;
        return cfg;
    }

    static TimelineConfig toLast() {
        TimelineConfig cfg;
        cfg.type = TimelineOperation::LastState;
        return cfg;
    }

    static TimelineConfig toInit() {
        TimelineConfig cfg;
        cfg.type = TimelineOperation::InitState;
        return cfg;
    }

    static TimelineConfig changeSpeed(float speed) {
        TimelineConfig cfg;
        cfg.type = TimelineOperation::ChangeSpeed;
        cfg.speed = speed;
        return cfg;
    }
};