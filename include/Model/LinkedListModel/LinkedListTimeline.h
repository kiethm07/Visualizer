#pragma once
#include <AssetManager/AssetManager.h>
#include <Model/LinkedListModel/LinkedListState.h>
#include <Model/LinkedListModel/ListOperation.h>
#include <Model/LinkedListModel/LinkedListRenderer.h>
#include <Animation/LinkedList/LinkedListRecorder.h>
#include <Animation/LinkedList/LinkedListAnimator.h>
#include <vector>
#include <algorithm>

class LinkedListTimeline {
public:
	LinkedListTimeline(const AssetManager& a_manager);
	bool isRunning() const{
		return running;
	}
	bool isAutoPlaying() const {
		return auto_play;
	}
	void setAutoPlay(bool auto_play) {
		this->auto_play = auto_play;
	}
	void setSpeed(float new_speed) {
		if (new_speed > MAX_SPEED) new_speed = MAX_SPEED;
		if (new_speed < MIN_SPEED) new_speed = MIN_SPEED;
		this->speed = new_speed;
	}
	void setDirection(int dir) {
		//-1: left
		//1: right
		this->direction = dir;
	}
	void onePhaseForward();
	void onePhaseBackward();
	void oneStepForward();
	void oneStepBackward();
	void toInit();
	void toLast();
	void push(const LinkedListState& current_state, const ListOperation& operation, const LinkedListRecorder& record);
	void run();
	void clear();
	void pause();
	void draw(sf::RenderWindow& window, const sf::View& view);
	void generateAnimation(const LinkedListState& initial_state, const LinkedListRecorder& record);
	void update(const float& real_delta_time);
	void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
private:
	const AssetManager& a_manager;
	LinkedListRenderer renderer;
	std::vector<ListOperation> operations;
	std::vector<LinkedListState> states;
	std::vector<LinkedListRecorder> records;

	LinkedListAnimator animator;
	LinkedListAnimationState current_animation_state; //used to draw
	bool next_phase_wating; //boundary case
	int current_operation_index = 0;
	//each operation consists of multiple phases,
	//each phase consists of multiple animation states
	int direction = 1; //1 for forward, -1 for backward
	float speed = 1.0f;
	const float MIN_SPEED = 0.1f;
	const float MAX_SPEED = 10.f;
	bool running = true;
	bool auto_play = true;
	float current_time = 0.f; //current time in the animation of the current operation
};