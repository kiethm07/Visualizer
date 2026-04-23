#pragma once
#include <Model/GraphModel/GraphOperation.h>
#include <string>
#include <map>

//Store the pseudocode
struct GraphDictionary {
	GraphDictionary();
	std::map<GraphOperationType, std::vector<std::string>> pseudocode;
	std::map<GraphOperationType, std::string> titles;
};