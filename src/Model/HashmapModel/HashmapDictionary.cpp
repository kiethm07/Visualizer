#include <Model/HashmapModel/HashmapDictionary.h>

HashmapDictionary::HashmapDictionary() {
	using Type = HashmapOperationType;

	// 1. Insert Node
	pseudocode[Type::Insert] = {
		"Get hash bucket",
		"If node->val == target",
		"    return because duplicate",
		"Insert target at the end",
		"Done!"
	};
	titles[Type::Insert] = "Insert Node";

	// 2. Remove Node
	pseudocode[Type::Remove] = {
		"Get hash bucket",
		"If node->val == target",
		"    remove node",
		"    update pointers and return",
		"Not found to remove",
		"Done!"
	};
	titles[Type::Remove] = "Remove Node";

	// 3. Search Node
	pseudocode[Type::Search] = {
		"Get hash bucket",
		"If node->val == target",
		"    return found",
		"return not found",
		"Done!"
	};
	titles[Type::Search] = "Search Value";

	// 4. Reset / Clear List
	pseudocode[Type::Reset] = {
		"Iterate bucket",
		"While bucket not empty",
		"    remove end node",
		"Done!"
	};
	titles[Type::Reset] = "Clear All";
}