#pragma once
#include <Model/AVLModel/AVLOperation.h>
#include <string>
#include <map>

//Store the pseudocode
struct AVLDictionary {
	AVLDictionary();
	std::map<AVLOperationType, std::vector<std::string>> pseudocode;
	std::map<AVLOperationType, std::string> titles;
};