#pragma once
#include <Model/HashmapModel/HashmapState.h>
#include <Model/HashmapModel/HashmapOperation.h>
#include <Animation/Hashmap/HashmapRecorder.h>
#include <vector>
#include <fstream>

class Hashmap {
public:
	Hashmap() {}
	HashmapState getState() const;
	void init(int n);
	void loadState(const HashmapState& state);
	void applyOperation(const HashmapOperation& operation, HashmapRecorder& recorder);
	void saveToFile(const std::string& filepath) const;
	void loadFromFile(const std::string& file_path);
	int getHash(int x) const {
		x %= bucket_count;
		if (x < 0) x += bucket_count;
		return x;
	}
	void rawInit(const int& bucket_count, const std::vector<int>& values);
private:
	struct Node {
		int val;
		int ui_id;
		Node(int val, int ui_id) :
			val(val), ui_id(ui_id) {
		}
	};
	int bucket_count;
	std::vector<std::vector<Node>> buckets;
	int next_ui_id = 0;
	void insert(int x, HashmapRecorder& recorder);
	void remove(int x, HashmapRecorder& recorder);
	void clear(HashmapRecorder& recorder);
	void search(int x, HashmapRecorder& recorder);
};