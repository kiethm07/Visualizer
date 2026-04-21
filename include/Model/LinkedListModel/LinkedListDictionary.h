#pragma once
#include <Model/LinkedListModel/ListOperation.h>
#include <string>
#include <map>

//Store the pseudocode
struct LinkedListDictionary {
	LinkedListDictionary();
	std::map<ListOperationType, std::vector<std::string>> pseudocode;
	std::map<ListOperationType, std::string> titles;
};