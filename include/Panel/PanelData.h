#pragma once
#include<vector>
enum class PanelOperation {Empty, Random, Manual, File};

struct PanelData {
	PanelOperation operation;
	std::vector<std::string> values;
};