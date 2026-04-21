#include <Model/LinkedListModel/LinkedListDictionary.h>
#include <Model/LinkedListModel/ListOperation.h>

LinkedListDictionary::LinkedListDictionary() {
	using Type = ListOperationType;

	// 1. Insert Single Node
	pseudocode[Type::InsertSingle] = {
		"While (!in_right_position) continue_traversing()",
		"If (!valid_position) return",
		"Insert new node and update pointers"
	};
	titles[Type::InsertSingle] = "Insert Single Node";

	// 2. Remove Node
	pseudocode[Type::Remove] = {
		"While (!in_right_position) continue_traversing()",
		"If (node_not_found) return",
		"Delete the node"
		"Update pointers to bypass the node",
	};
	titles[Type::Remove] = "Remove Node";

	// 3. Update Node Value
	pseudocode[Type::Update] = {
		"While (!in_right_position) continue_traversing()",
		"If (node_not_found) return",
		"Update the node's value"
	};
	titles[Type::Update] = "Update Node Value";

	// 4. Search Node
	pseudocode[Type::Search] = {
		"While (current_node != nullptr)",
		"    If (current_node.value == target) return node",
		"    current_node = current_node.next",
		"Return not_found"
	};
	titles[Type::Search] = "Search Value";

	// 5. Reset / Clear List
	pseudocode[Type::Reset] = {
		"While (head != nullptr)",
		"    temp = head",
		"    head = head.next",
		"    delete temp"
	};
	titles[Type::Reset] = "Clear Linked List";
}