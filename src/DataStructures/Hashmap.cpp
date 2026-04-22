#include <DataStructures/Hashmap.h>
#include <iostream>

void Hashmap::init(int n)
{
	bucket_count = n;
	next_ui_id = 0;
	buckets.clear();
	buckets.resize(n);
}

HashmapState Hashmap::getState() const
{
	HashmapState state;
	state.bucket_count = this->bucket_count;
	state.next_ui_id = this->next_ui_id;
	for (int i = 0; i < bucket_count; i++)
	{
		std::vector<int> value, ui_id;
		for (int j = 0; j < (int)buckets[i].size(); j++)
		{
			value.push_back(buckets[i][j].val);
			ui_id.push_back(buckets[i][j].ui_id);
		}
		state.value.push_back(value);
		state.ui_id.push_back(ui_id);
	}
	return state;
}

void Hashmap::loadState(const HashmapState& state)
{
	this->bucket_count = state.bucket_count;
	this->next_ui_id = state.next_ui_id;
	buckets.clear();
	buckets.resize(bucket_count);
	for (int i = 0; i < bucket_count; i++)
	{
		const std::vector<int>& value = state.value[i];
		const std::vector<int>& ui_id = state.ui_id[i];
		for (int j = 0; j < (int)value.size(); j++)
		{
			this->buckets[i].push_back(Node(value[j], ui_id[j]));
		}
	}
}

void Hashmap::rawInit(const int& bucket_count, const std::vector<int>& values)
{
	this->bucket_count = bucket_count;
	next_ui_id = 0;
	buckets.assign(bucket_count, {});
	for (const int& i : values)
	{
		int x = getHash(i);
		buckets[x].push_back(Node(i, next_ui_id++));
	}
}

void Hashmap::applyOperation(const HashmapOperation& operation, HashmapRecorder& recorder)
{
	using Command = HashmapAnimationCommand;
	using Type = HashmapAnimationType;
	using Target = HashmapAnimationTarget;
	if (operation.type == HashmapOperationType::Insert){
		int x = operation.value;
		insert(x, recorder);
		recorder.addNewPhase();
		recorder.setHighlightedLine(4);
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		return;
	}
	if (operation.type == HashmapOperationType::Remove){
		int x = operation.value;
		remove(x, recorder);
		recorder.addNewPhase();
		recorder.setHighlightedLine(5);
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		return;
	}
	if (operation.type == HashmapOperationType::Search){
		int x = operation.value;
		search(x, recorder);
		recorder.addNewPhase();
		recorder.setHighlightedLine(4);
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		return;
	}
	if (operation.type == HashmapOperationType::Reset){
		clear(recorder);
		recorder.addNewPhase();
		recorder.setHighlightedLine(3);
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		return;
	}
}

void Hashmap::insert(int x, HashmapRecorder& recorder)
{
	using Command = HashmapAnimationCommand;
	using Target = HashmapAnimationTarget;
	using Type = HashmapAnimationType;

	int k = getHash(x);
	int bucket_ui_id = -(k + 1);

	recorder.addNewPhase();
	recorder.setHighlightedLine(0);
	recorder.addCommand(Command(Target::Bucket, Type::HighlightOn, bucket_ui_id));

	int pre_id = -1;
	for (int i = 0; i < (int)buckets[k].size(); i++)
	{
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, buckets[k][i].ui_id));
		if (pre_id != -1)
		{
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
		}

		if (buckets[k][i].val == x)
		{
			recorder.addNewPhase();
			recorder.setHighlightedLine(2);
			recorder.addCommand(Command(Target::Node, Type::FoundedOn, buckets[k][i].ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::Node, Type::Wait, buckets[k][i].ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::Node, Type::FoundedOff, buckets[k][i].ui_id));
			recorder.addNewPhase();
			//recorder.addCommand(Command(Target::Node, Type::HighlightOff, buckets[k][i].ui_id));
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::Bucket, Type::HighlightOff, bucket_ui_id));
			return;
		}
		pre_id = buckets[k][i].ui_id;
	}

	Node tmp(x, next_ui_id++);
	int spawn_source = (pre_id != -1) ? pre_id : bucket_ui_id;

	recorder.addNewPhase();
	recorder.setHighlightedLine(3);
	recorder.addCommand(Command::createSpawnNodeCommand(tmp.ui_id, spawn_source, x, { 200, 0 }));
	recorder.addCommand(Command(Target::Node, Type::FadeIn, tmp.ui_id));

	recorder.addNewPhase();
	recorder.setHighlightedLineAsPrevious();
	recorder.addCommand(Command::createSpawnEdgeCommand(spawn_source, tmp.ui_id));
	recorder.addCommand(Command(Target::Edge, Type::FadeIn, spawn_source, tmp.ui_id));

	if (pre_id != -1)
	{
		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
	}

	recorder.addNewPhase();
	recorder.setHighlightedLineAsPrevious();
	recorder.addCommand(Command(Target::Bucket, Type::HighlightOff, bucket_ui_id));

	buckets[k].push_back(tmp);
}

void Hashmap::remove(int x, HashmapRecorder& recorder)
{
	using Command = HashmapAnimationCommand;
	using Target = HashmapAnimationTarget;
	using Type = HashmapAnimationType;

	int k = getHash(x);
	int bucket_ui_id = -(k + 1);

	recorder.addNewPhase();
	recorder.setHighlightedLine(0);
	recorder.addCommand(Command(Target::Bucket, Type::HighlightOn, bucket_ui_id));

	int pre_id = -1;
	for (int i = 0; i < (int)buckets[k].size(); i++)
	{
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, buckets[k][i].ui_id));
		if (pre_id != -1)
		{
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
		}

		if (buckets[k][i].val != x)
		{
			pre_id = buckets[k][i].ui_id;
			continue;
		}

		recorder.addNewPhase();
		recorder.setHighlightedLine(2);
		recorder.addCommand(Command(Target::Node, Type::FadeOut, buckets[k][i].ui_id));
		int from_id = (pre_id != -1) ? pre_id : bucket_ui_id;
		recorder.addCommand(Command(Target::Edge, Type::FadeOut, from_id, buckets[k][i].ui_id));
		int next_id = (i + 1 < (int)buckets[k].size()) ? buckets[k][i + 1].ui_id : -1;
		if (next_id != -1)
		{
			recorder.addCommand(Command(Target::Edge, Type::FadeOut, buckets[k][i].ui_id, next_id));
		}

		recorder.addNewPhase();
		recorder.setHighlightedLine(3);
		//if (pre_id != -1)
		//{
		//	recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
		//}
		for (int j = i + 1; j < (int)buckets[k].size(); j++)
		{
			recorder.addCommand(Command(Target::Node, Type::Move, HashmapMoveDirection::Left, buckets[k][j].ui_id));
		}

		if (next_id != -1)
		{
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command::createSpawnEdgeCommand(from_id, next_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeIn, from_id, next_id));
		}

		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Bucket, Type::HighlightOff, bucket_ui_id));

		buckets[k].erase(buckets[k].begin() + i);
		return;
	}

	if (pre_id != -1)
	{
		recorder.addNewPhase();
		recorder.setHighlightedLine(4);
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
	}
	recorder.addNewPhase();
	recorder.setHighlightedLineAsPrevious();
	recorder.addCommand(Command(Target::Bucket, Type::HighlightOff, bucket_ui_id));
}

void Hashmap::search(int x, HashmapRecorder& recorder)
{
	using Command = HashmapAnimationCommand;
	using Target = HashmapAnimationTarget;
	using Type = HashmapAnimationType;

	int k = getHash(x);
	int bucket_ui_id = -(k + 1);

	recorder.addNewPhase();
	recorder.setHighlightedLine(0);
	recorder.addCommand(Command(Target::Bucket, Type::HighlightOn, bucket_ui_id));

	int pre_id = -1;
	for (int i = 0; i < (int)buckets[k].size(); i++)
	{
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, buckets[k][i].ui_id));
		if (pre_id != -1)
		{
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
		}

		if (buckets[k][i].val == x)
		{
			recorder.addNewPhase();
			recorder.setHighlightedLine(2);
			recorder.addCommand(Command(Target::Node, Type::FoundedOn, buckets[k][i].ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::Node, Type::Wait, buckets[k][i].ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::Node, Type::FoundedOff, buckets[k][i].ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			//recorder.addCommand(Command(Target::Node, Type::HighlightOff, buckets[k][i].ui_id));
			recorder.addCommand(Command(Target::Bucket, Type::HighlightOff, bucket_ui_id));
			return;
		}
		pre_id = buckets[k][i].ui_id;
	}

	if (pre_id != -1)
	{
		recorder.addNewPhase();
		recorder.setHighlightedLine(3);
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
	}
	recorder.addNewPhase();
	recorder.setHighlightedLineAsPrevious();
	recorder.addCommand(Command(Target::Bucket, Type::HighlightOff, bucket_ui_id));
}

void Hashmap::clear(HashmapRecorder& recorder)
{
	using Command = HashmapAnimationCommand;
	using Target = HashmapAnimationTarget;
	using Type = HashmapAnimationType;

	for (int i = 0; i < bucket_count; i++)
	{
		int bucket_ui_id = -(i + 1);
//		if (buckets[i].empty()) continue;
		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command(Target::Bucket, Type::HighlightOn, bucket_ui_id));

		//for (int j = 0; j < (int)buckets[i].size(); j++)
		//{
		//	recorder.addNewPhase();
		//	recorder.addCommand(Command(Target::Node, Type::HighlightOn, buckets[i][j].ui_id));
		//}

		int n = buckets[i].size();
		for (int j = n - 1; j >= 0; j--) {
			recorder.addNewPhase();
			recorder.setHighlightedLine(1);
			//recorder.addCommand(Command(Target::Node, Type::Wait, -1));
			//recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::HighlightOn, buckets[i][j].ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLine(2);
			recorder.addCommand(Command(Target::Node, Type::FadeOut, buckets[i][j].ui_id));
			int from_id = (j == 0) ? bucket_ui_id : buckets[i][j - 1].ui_id;
			recorder.addCommand(Command(Target::Edge, Type::FadeOut, from_id, buckets[i][j].ui_id));
		}

		buckets[i].clear();

		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Bucket, Type::HighlightOff, bucket_ui_id));
	}
}