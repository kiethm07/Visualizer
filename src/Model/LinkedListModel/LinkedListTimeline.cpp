#include <Model/LinkedListModel/LinkedListTimeline.h>

void LinkedListTimeline::push(const LinkedListState& current_state, const ListOperation& operation) {
	list_states.push_back(current_state);
	list_operations.push_back(operation);
}