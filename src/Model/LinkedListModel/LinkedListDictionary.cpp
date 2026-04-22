#include <Model/LinkedListModel/LinkedListDictionary.h>
#include <Model/LinkedListModel/ListOperation.h>

LinkedListDictionary::LinkedListDictionary() {
	using Type = ListOperationType;

	// 1. Insert Single Node
	pseudocode[Type::InsertSingle] = {
		"While (!in_right_position) continue_traversing()",
		"If (!valid_position) return",
		"Insert new node and update pointers",
		"Done!"
	};
	titles[Type::InsertSingle] = "Insert Single Node";

	// 2. Remove Node
	pseudocode[Type::Remove] = {
		"While (!in_right_position) continue_traversing()",
		"If (!valid_position) return",
		"Delete the node and update pointers",	
		"Done!"
	};
	titles[Type::Remove] = "Remove Node";

	// 3. Update Node Value
	pseudocode[Type::Update] = {
		"While (!in_right_position) continue_traversing()",
		"If (!valid_position) return",
		"Update the node's value",
		"Done!"
	};
	titles[Type::Update] = "Update Node Value";

	// 4. Search Node
	pseudocode[Type::Search] = {
		"While (current_node != nullptr)",
		"	 If (current_node.value == target)",
		"		 return current_node",
		"	 else Move to next node",
		"Return not_found",
		"Done!"
	};
	titles[Type::Search] = "Search Value";

	// 5. Reset / Clear List
	pseudocode[Type::Reset] = {
		"If (current_node == nullptr) return",
		"Recursively reset(current_node->next)",
		"Delete the current node and link",
		"Done!"
	};
	titles[Type::Reset] = "Clear Linked List";
}