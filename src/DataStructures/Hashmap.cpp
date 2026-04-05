#include <DataStructures/Hashmap.h>
#include <iostream>

void Hashmap::init(int n) {
	bucket_count = n;
	next_ui_id = 0;
	buckets.clear();
	buckets.resize(n);
}

HashmapState Hashmap::getState() const {
	HashmapState state;
	state.bucket_count = this->bucket_count;
	state.next_ui_id = this->next_ui_id;
	for (int i = 0; i < bucket_count; i++) {
		std::vector<int> value, ui_id;
		for (int j = 0; j < buckets[i].size(); j++) {
			value.push_back(buckets[i][j].val);
			ui_id.push_back(buckets[i][j].ui_id);
		}
		state.value.push_back(value);
		state.ui_id.push_back(ui_id);
	}
	return state;
}

void Hashmap::loadState(const HashmapState& state) {
	this->bucket_count = state.bucket_count;
	this->next_ui_id = state.next_ui_id;
	buckets.clear();
	buckets.resize(bucket_count);
	for (int i = 0; i < bucket_count; i++) {
		const std::vector<int>& value = state.value[i];
		const std::vector<int>& ui_id = state.ui_id[i];
		for (int j = 0; j < value.size(); j++) {
			this->buckets[i].push_back(Node(value[j], ui_id[j]));
		}
	}
}

void Hashmap::rawInit(const int& bucket_count, const std::vector<int>& values) {
	this->bucket_count = bucket_count;
	next_ui_id = 0;
	buckets.assign(bucket_count, {});
	for (const int& i : values) {
		int x = getHash(i);
		buckets[x].push_back(Node(i, next_ui_id++));
	}
}

void Hashmap::applyOperation(const HashmapOperation& operation, HashmapRecorder& recorder) {

}

void Hashmap::insert(int x, HashmapRecorder& recorder) {
	int k = getHash(x);
	for (int i = 0; i < buckets[k].size(); i++) {
		if (buckets[k][i].val == x) {
			//Duplicate
			return;
		}
	}
	buckets[k].push_back(Node(x, next_ui_id++));
}

void Hashmap::remove(int x, HashmapRecorder& recorder) {
	int k = getHash(x);
	for (int i = 0; i < buckets[k].size(); i++) {
		if (buckets[k][i].val != x) continue;
		buckets[k].erase(buckets[k].begin() + i);
		return;
	}
	//Nothing to remove
}

void Hashmap::search(int x, HashmapRecorder& recorder) {
	int k = getHash(x);
	for (int i = 0; i < buckets[k].size(); i++) {
		if (buckets[k][i].val != x) continue;
		//Found x!
		//Process
		return;
	}
	//Not found
}

void Hashmap::clear(HashmapRecorder& recorder) {
	for (int i = 0; i < bucket_count; i++) {
		buckets[i].clear();
	}
}