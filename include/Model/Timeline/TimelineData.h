#pragma once
#include <vector>

template <typename State, typename Operation, typename Record>
class TimelineData {
public:
    void setInitialState(const State& state) {
        initial_state = state;
    }
    State getInitialState() const {
        return initial_state;
    }
    void push(const State& state, const Operation& op, const Record& record) {
        states.push_back(state);
        operations.push_back(op);
        records.push_back(record);
    }

    void clear() {
        states.clear();
        operations.clear();
        records.clear();
    }

    bool empty() const {
        return states.empty();
    }

    std::size_t size() const {
        return states.size();
    }

    const State& getState(std::size_t index) const {
        return states[index];
    }

    const Operation& getOperation(std::size_t index) const {
        return operations[index];
    }

    const Record& getRecord(std::size_t index) const {
        return records[index];
    }

private:
    State initial_state;
    std::vector<State> states;
    std::vector<Operation> operations;
    std::vector<Record> records;
};