#pragma once

class TimelineController {
public:
    bool isRunning() const {
        return running;
    }
    bool isAutoPlaying() const {
        return auto_play;
    }
    void setAutoPlay(bool value) {
        auto_play = value;
    }
    void setSpeed(float new_speed) {
        if (new_speed > MAX_SPEED) new_speed = MAX_SPEED;
        if (new_speed < MIN_SPEED) new_speed = MIN_SPEED;
        speed = new_speed;
    }
    void setDirection(int dir) {
        direction = dir;
    }
    void run() {
        running = true;
    }
    void pause() {
        running = false;
    }
    void reset() {
        next_phase_waiting = false;
        current_operation_index = 0;
        direction = 1;
        speed = 1.f;
        running = true;
        auto_play = true;
        current_time = 0.f;
    }
public:
    bool next_phase_waiting = false;
    int current_operation_index = 0;
    int direction = 1;
    float speed = 1.f;
    bool running = true;
    bool auto_play = true;
    float current_time = 0.f;

private:
    const float MIN_SPEED = 0.1f;
    const float MAX_SPEED = 10.f;
};