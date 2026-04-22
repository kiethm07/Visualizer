#pragma once
#include <Model/HashmapModel/HashmapOperation.h>
#include <string>
#include <map>

//Store the pseudocode
struct HashmapDictionary {
	HashmapDictionary();
	std::map<HashmapOperationType, std::vector<std::string>> pseudocode;
	std::map<HashmapOperationType, std::string> titles;
};