#include <Model/AVLModel/AVLDictionary.h>

AVLDictionary::AVLDictionary() {
	using Type = AVLOperationType;

	// 1. Insert Single Node
	pseudocode[Type::Insert] = {
		"If node == nullptr",
		"    Create new node then return",
		"If node->val == target",
		"    return (do nothing)",
		"If node->val < target",
		"    Traverse right",
		"Else Traverse left",
		"Balance subtree",
		"Done!"
	};
	titles[Type::Insert] = "Insert Single Node";

	// 2. Remove Node
	pseudocode[Type::Remove] = {
		"If node == nullptr then return",
		"If node->val == target",
		"    If is leaf or single chain",
		"        Remove node and update pointers, then return",
		"    Swap node with left-most in right subtree",
		"    Call remove(left-most) in right subtree",
		"If node->val < target",
		"    Traverse right",
		"Else Traverse left",
		"Balance subtree",
		"Done!"
	};
	titles[Type::Remove] = "Remove Node";

	// 3. Search Node
	pseudocode[Type::Search] = {
		"If node == nullptr",
		"    return not found",
		"If node->val == target",
		"    return found",
		"If node->val < target",
		"    Traverse right",
		"Else Traverse left",
		"Done!"
	};
	titles[Type::Search] = "Search Value";

	// 4. Reset / Clear List
	pseudocode[Type::Reset] = {
		"If (node == nullptr) return",
		"Recursively reset(node->left)",
		"Recursively reset(node->right)",
		"Delete the current node and link",
		"Done!"
	};
	titles[Type::Reset] = "Clear Linked List";
}