#include <Model/TrieModel/TrieDictionary.h>

TrieDictionary::TrieDictionary() {
	using Type = TrieOperationType;

	// 1. Insert Node
	pseudocode[Type::Insert] = {
		"Iterate each character",
		"If character not in trie",
		"    create new node",
		"Set ending mark",
		"Done!"
	};
	titles[Type::Insert] = "Insert String";

	// 2. Remove Node
	pseudocode[Type::Remove] = {
		"Find if string exists",
		"If not exists then return",
		"Recursively remove next nodes",
		"Decrease count of this node",
		"If count == 0",
		"    If node has no child",
		"        Remove this node and link",
		"    If node is ending mark",
		"        Unset ending mark",
		"Done!"
	};
	titles[Type::Remove] = "Remove String";

	// 3. Search Node
	pseudocode[Type::Search] = {
		"Iterate each character",
		"If character not in trie",
		"    return not found",
		"If is ending mark",
		"    return found",
		"Return not found",
		"Done!"
	};
	titles[Type::Search] = "Search String";

	// 4. Reset / Clear List
	pseudocode[Type::Reset] = {
		"If (node == nullptr)",
		"    return",
		"Recursively reset(child node)",
		"Delete the current node and link",
		"Done!"
	};
	titles[Type::Reset] = "Clear All";
}