#pragma once
#include <Model/LinkedListModel/LinkedListState.h>
#include <Model/LinkedListModel/ListOperation.h>
#include <Animation/LinkedList/LinkedListRecorder.h>
#include <vector>

class LinkedListTimeline {
public:
	void push(const LinkedListState& current_state, const ListOperation& operation);
private:
	std::vector<ListOperation> list_operations;
	std::vector<LinkedListState> list_states;
	std::vector<LinkedListRecorder> records;
};