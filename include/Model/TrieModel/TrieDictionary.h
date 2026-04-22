#pragma once
#include <Model/TrieModel/TrieOperation.h>
#include <string>
#include <map>

//Store the pseudocode
struct TrieDictionary {
	TrieDictionary();
	std::map<TrieOperationType, std::vector<std::string>> pseudocode;
	std::map<TrieOperationType, std::string> titles;
};